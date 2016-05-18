#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/epoll.h>

namespace xiix{

// example from http://asistobe851.web.fc2.com/C_and_Cpp/epoll-client.html

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
			fprintf(stderr,"Usage: %s [host] [port] [uri] [connections]\n",argv[0]);
			return -1;
		}
		const char*hostname=argv[1];
		const int port=atoi(argv[2]);
		const char*uri=argv[3];
		const int nsocks=atoi(argv[4]);
		printf("\n   x i i x   m a r k\n\n");
		printf("clients: %d\n",nsocks);
		if(port!=80)printf("http://%s:%d%s\n",hostname,port,uri);
		else printf("http://%s%s\n",hostname,uri);


		const int epfd=epoll_create(nsocks);
		if(epfd<0)throw"epoll_create";

		int sock[nsocks];
		for(int i=0;i<nsocks;i++){
			const int sk=socket(AF_INET,SOCK_STREAM,0);
			if(sk<0)throw"socket";

			struct hostent*he;
			if((he=gethostbyname(hostname))==NULL)throw"nosuchhost";

			struct sockaddr_in addr;
			bzero((char*)&addr,sizeof addr);
			bcopy(he->h_addr,&addr.sin_addr,he->h_length);
			addr.sin_family=AF_INET;
			addr.sin_port=htons(port);

			if(connect(sk,(struct sockaddr*)&addr,sizeof addr)<0)throw"connect";

		    int flag=fcntl(sk,F_GETFL,0);
		    if(fcntl(sk,F_SETFL,flag|O_NONBLOCK)==-1)throw"fcntl";

			sock[i]=sk;
		}
		struct epoll_event events[nsocks];
		for(int i=0;i<nsocks;i++){
			struct epoll_event ev;
			memset(&ev,0,sizeof ev);
			ev.events= EPOLLIN|EPOLLOUT|EPOLLET;
			ev.data.fd=sock[i];
			if(epoll_ctl(epfd,EPOLL_CTL_ADD,sock[i],&ev)<0)throw"epoll_ctl";
		}
		size_t reads{0};
		size_t writes{0};
		while(1){
			const int nfd=epoll_wait(epfd,events,nsocks,-1);
			for(int i=0;i<nfd;++i){
				int client=events[i].data.fd;
				if(events[i].events&EPOLLOUT){
//					puts("  *** write");
					writes++;
					char message[1024];
					snprintf(message,sizeof message,"GET %s HTTP/1.1\r\nHost: %s:%d\r\n\r\n",uri,hostname,port);
					const int write_len=write(client,message,strlen(message));
//					printf("write_len=%d\n",write_len);
//					printf("%s",message);
				}
				if(events[i].events&EPOLLIN){
//					puts("  *** read");
					reads++;
					char message[1024];
					memset(message,0,sizeof message);
					int read_len=read(client,message,sizeof message);
//					printf("read_len=%d\n",read_len);
//					struct epoll_event ev;
//					epoll_ctl(epfd,EPOLL_CTL_DEL,client,&ev);
//					close(client);
//
//					int newSock=getSocket(host,port);
//					setNonblocking(newSock);
//
//					memset(&ev,0,sizeof ev);
//					ev.events= EPOLLIN|EPOLLOUT|EPOLLET;
//					ev.data.fd=newSock;
//					if(epoll_ctl(epfd,EPOLL_CTL_ADD,newSock,&ev)<0)throw"epoll_ctl";
				}
				printf(" writes: %zu    reads: %zu \r",writes,reads);
			}
		}
		return 0;
	}
//private:
//	inline int new_socket(const char*hostname,const int portNumber){
//		int sock=socket(AF_INET,SOCK_STREAM,0);
//		if(sock<0)throw"socket";
//
//		struct sockaddr_in addr;
//		bzero((char*)&addr,sizeof addr);
//
//		struct hostent*he;
//		if((he=gethostbyname(hostname))==NULL)throw"nosuchhost";
//		bcopy(he->h_addr,&addr.sin_addr,he->h_length);
//		addr.sin_family=AF_INET;
//		addr.sin_port=htons(portNumber);
//
//		if(connect(sock,(struct sockaddr*)&addr,sizeof addr)<0)throw"connect";
//
//	    int flag=fcntl(sock,F_GETFL,0);
//	    fcntl(sock,F_SETFL,flag|O_NONBLOCK);
//	    //? verifyok
//
//	    return sock;
//	}
};
}
