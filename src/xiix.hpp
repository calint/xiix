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
		auto p=argv;
		for(int i=0;i<argc;i++)
			puts(*p++);
		try{
			main(argc,argv);
		}catch(const char*msg){
			puts(msg);
		}catch(...){
			puts("exception caught");
		}
	}

	int	main(int argc,char**argv ){
		if(argc!=5){
			fprintf(stderr,"Usage: %s [host] [port] [requestURI] [maxClients]\n",argv[0]);
			return -1;
		}
		const char*host=argv[1];
		const int port=atoi(argv[2]);
		const char*requestURI=argv[3];
		const int maxClients=atoi(argv[4]);

		const int epfd=epoll_create(maxClients);
		if(epfd<0){
			perror("epoll_create");
			return -2;
		}

		int sock[maxClients];
		for(int i=0;i<maxClients;i++){
			sock[i]=getSocket(host,port);
			setNonblocking(sock[i]);
//	        fprintf ( stderr, "... socket handle created ... \n" );
		}
		struct epoll_event events[maxClients];
		for(int i=0;i<maxClients;i++){
			struct epoll_event ev;
			memset(&ev,0,sizeof ev);
			ev.events= EPOLLIN|EPOLLOUT|EPOLLET;
			ev.data.fd=sock[i];
			if(epoll_ctl(epfd,EPOLL_CTL_ADD,sock[i],&ev)<0)throw"epoll_ctl";
		}
		size_t reads{0};
		size_t writes{0};
		while(1){
			const int nfd=epoll_wait(epfd,events,maxClients,-1);
			for(int i=0;i<nfd;++i){
				int client=events[i].data.fd;
				if(events[i].events&EPOLLOUT){
//					puts("  *** write");
					writes++;
					char message[1024];
					snprintf(message,sizeof message,"GET %s HTTP/1.1\r\nHost: %s:%d\r\n\r\n",requestURI,host,port);
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
private:
	int getSocket(const char*hostname,const int portNumber){
		int sock=socket(AF_INET,SOCK_STREAM,0);
		if(sock<0)throw"socket";

		struct sockaddr_in addr;
		bzero((char*)&addr,sizeof addr);

		struct hostent*he;
		if((he=gethostbyname(hostname))==NULL)throw"nosuchhost";
		bcopy(he->h_addr,&addr.sin_addr,he->h_length);
		addr.sin_family=AF_INET;
		addr.sin_port=htons(portNumber);

		if(connect(sock,(struct sockaddr*)&addr,sizeof addr)<0)throw"connect";

		return sock;
	}
	void setNonblocking(const int sock){
	    int flag=fcntl(sock,F_GETFL,0);
	    fcntl(sock,F_SETFL,flag|O_NONBLOCK );
	    //? verifyok
	}


};
}
