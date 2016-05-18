#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<errno.h>
#include<memory>
namespace xiix{

namespace meters{
	size_t events{0};
	size_t input{0};
	size_t output{0};
	size_t requests{0};
	size_t opens{0};
	size_t reads{0};
	size_t writes{0};
	size_t brks{0};
	size_t errors{0};
}

class sock final{
	int epollfd{0};
	int sockfd{0};
	const char*hostname{nullptr};
	const int port{0};
	const char*uri{"/"};
	struct epoll_event ev;
public:
	inline sock(const int epollfd,const char*hostname,const int port):epollfd{epollfd},hostname{hostname},port{port}{}
	inline int fd()const{return sockfd;}
	inline void open(){
		meters::opens++;
		sockfd=socket(AF_INET,SOCK_STREAM,0);
		if(sockfd<0)throw"socket";

		struct hostent*he=gethostbyname(hostname);
		if(!he)throw"nosuchhost";

		struct sockaddr_in sa;
		bzero((void*)&sa,sizeof sa);
		bcopy(he->h_addr,&sa.sin_addr,he->h_length);
		sa.sin_family=AF_INET;
		sa.sin_port=htons(port);
		if(connect(sockfd,(struct sockaddr*)&sa,sizeof sa)<0)throw"connect";

	    int flag=fcntl(sockfd,F_GETFL,0);
	    if(flag<0)throw"getfl";
	    if(fcntl(sockfd,F_SETFL,flag|O_NONBLOCK)==-1)throw"fcntl";

		memset(&ev,0,sizeof(struct epoll_event));
		ev.events=EPOLLOUT|EPOLLET;
		ev.data.fd=sockfd;
		ev.data.ptr=this;
		if(epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev)<0)throw"epoll_ctl";
	}
	inline void on_epoll_event(struct epoll_event&ev){
		if(ev.events&EPOLLOUT){
			char buf[1024];
			const size_t len=snprintf(buf,sizeof buf,"GET %s HTTP/1.1\r\nHost: %s:%d\r\n\r\n",uri,hostname,port);
			io_send(buf,len,true);
			io_request_read();
			return;
		}
		if(ev.events&EPOLLIN){
			char buf[1024];
			io_recv(buf,sizeof buf);
			io_request_write();
			meters::requests++;
			return;
//			printf("read_len=%d\n",read_len);
//			struct epoll_event ev;
//			epoll_ctl(epfd,EPOLL_CTL_DEL,client,&ev);
//			close(client);
//
//			int newSock=getSocket(host,port);
//			setNonblocking(newSock);
//
//			memset(&ev,0,sizeof ev);
//			ev.events= EPOLLIN|EPOLLOUT|EPOLLET;
//			ev.data.fd=newSock;
//			if(epoll_ctl(epfd,EPOLL_CTL_ADD,newSock,&ev)<0)throw"epoll_ctl";
		}
	}
private:
	inline size_t io_send(const void*buf,size_t len,bool throw_if_send_not_complete=false){
		const ssize_t c=send(sockfd,buf,len,MSG_NOSIGNAL);
		if(c<0){
			if(errno==EPIPE||errno==ECONNRESET)throw"brk";
			throw"send";
		}
		meters::output+=c;
		if(throw_if_send_not_complete&&(size_t)c!=len)throw"sendnotcomplete";

		return(size_t)c;
	}
	inline size_t io_recv(void*buf,size_t maxlen){
		const ssize_t c=recv(sockfd,buf,maxlen,0);
		if(c==0)return 0;// closed by client
		if(c<0){//error
			if(errno==EAGAIN||errno==EWOULDBLOCK){
				io_request_read();
			}else if(errno==ECONNRESET){
				throw"closed2";
			}
			throw"err";
		}
		meters::input+=c;
		return c;
	}
	inline void io_request_read(){
		ev.events=EPOLLIN|EPOLLET;
		if(epoll_ctl(epollfd,EPOLL_CTL_MOD,sockfd,&ev))throw"epollmodread";
	}
	inline void io_request_write(){
		ev.events=EPOLLOUT|EPOLLET;
		if(epoll_ctl(epollfd,EPOLL_CTL_MOD,sockfd,&ev))throw"epollmodwrite";
	}
};

class client{
public:
	client(int argc,char**argv,char**env){
		try{
			main(argc,argv);
		}catch(const char*msg){
			puts(msg);
		}catch(...){
			puts("exception caught");
		}
	}
private:
	int	main(int argc,char**argv ){
		if(argc!=5){
			fprintf(stderr,"usage: %s [host] [port] [uri] [connections]\n",argv[0]);
			return -1;
		}
		const char*hostname=argv[1];
		const int port=atoi(argv[2]);
		const char*uri=argv[3];
		const int nsocks=atoi(argv[4]);
		printf("\n   x i i x   m a r k\n\n");
		printf("      clients: %d\n",nsocks);
		if(port!=80)printf("          url: http://%s:%d%s\n",hostname,port,uri);
		else        printf("          url: http://%s%s\n",hostname,uri);


		const int epfd=epoll_create(nsocks);
		if(epfd<0)throw"epoll_create";

		sock*socks[nsocks];
		for(auto&s:socks)s=new sock(epfd,hostname,port);
		for(auto s:socks)s->open();
//		for(int i=0;i<nsocks;i++)socks[i]=new sock(epfd,hostname,port);
		struct epoll_event events[nsocks];
		while(1){
			const int n=epoll_wait(epfd,events,nsocks,-1);
			for(int i=0;i<n;++i){
				meters::events++;
				struct epoll_event&ev=events[i];
				if(ev.events&EPOLLOUT)meters::writes++;
				if(ev.events&EPOLLIN)meters::reads++;
				sock*sk=(sock*)ev.data.ptr;
				sk->on_epoll_event(ev);
				printf("   requests: %zu  events: %zu  writes: %zu    reads: %zu \r",meters::requests,meters::events,meters::writes,meters::reads);
			}
		}
	}
};
}
