#pragma once
#include"meters.hpp"
#include"lul.hpp"
#include<string.h>
#include<netdb.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<errno.h>
#include<unistd.h>
namespace xiix{class sock final{
	int epollfd{0};
	int sockfd{0};
	const char*hostname{nullptr};
	const int port{0};
	const char*uri{"/"};
	struct epoll_event ev;
	lul<const char*>headers{true,true};
public:
	inline sock(const int epollfd,const char*hostname,const int port):epollfd{epollfd},hostname{hostname},port{port}{meters::socks++;}
	inline~sock(){meters::socks--;}
	inline int fd()const{return sockfd;}
	inline sock&seturi(/*refs*/const char*s){uri=s;return*this;}
	inline sock&setrepeatmode(const bool b){repeat_request_after_done=b;return*this;}
	inline void connect(){
		meters::opens++;
		sockfd=socket(AF_INET,SOCK_STREAM,0);
		if(sockfd<0)throw"socket";

		struct hostent*he=gethostbyname(hostname);
		if(!he)throw"gethostbyname";

		struct sockaddr_in sa;
		bzero((void*)&sa,sizeof sa);
		bcopy(he->h_addr,&sa.sin_addr,he->h_length);
		sa.sin_family=AF_INET;
		sa.sin_port=htons(port);
		if(::connect(sockfd,(struct sockaddr*)&sa,sizeof sa)<0)throw"connect";

	    int flag=fcntl(sockfd,F_GETFL,0);
	    if(flag<0)throw"getfl";
	    if(fcntl(sockfd,F_SETFL,flag|O_NONBLOCK)==-1)throw"nonblock";

		memset(&ev,0,sizeof(struct epoll_event));
		ev.events=EPOLLOUT|EPOLLET;
		ev.data.fd=sockfd;
		ev.data.ptr=this;
		if(epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev)<0)throw"epolladd";
	}
	inline void on_epoll_event(struct epoll_event&ev){
		if(ev.events&EPOLLIN){
			meters::reads++;
			if(bufi!=buflen)throw"incompleteparse";
			buflen=io_recv(buf,bufsize);
			bufi=0;
			bufp=buf;
		}
		parse_buf();
	}
	inline void disconnect(){
		close(sockfd);
	}

private:
	enum state{next_req,send_req,recv_response_protocol,recv_response_code,recv_response_text,recv_header_key,recv_header_value,recv_content,recv_content_sized,recv_content_chuncked};
	state st{next_req};
	static const size_t bufsize{4*1024};
	char buf[bufsize];
	size_t bufi{0};
	size_t buflen{0};
	char*bufp{nullptr};
	char*response_code{nullptr};
	char*response_text{nullptr};
	char*headerp{nullptr};
//	char*header_value{nullptr};
	char*header_key{nullptr};
	size_t content_length{0};
	size_t content_index{0};
	bool repeat_request_after_done{false};
	bool first_request{true};
	inline void parse_buf(){loop(){
		if(st==next_req){
			if(first_request){
				first_request=false;
			}else{
				if(!repeat_request_after_done){
					disconnect();
					return;
				}
			}
			meters::requests++;
			char s[1024];
			const size_t n=(port!=80)?
				snprintf(s,sizeof s,"GET %s HTTP/1.1\r\nHost: %s:%d\r\n\r\n",uri,hostname,port):
				snprintf(s,sizeof s,"GET %s HTTP/1.1\r\nHost: %s\r\n\r\n",uri,hostname);
			io_send(s,n,true);
			io_request_read();
//			headers=lul<const char*>{true,true};//? does not invoke delete on previous
			headers.clear();
			st=recv_response_protocol;
			return;
		}
		if(st==recv_response_protocol){
			while(bufi<buflen){
				const char ch=*bufp++;
				bufi++;
				if(ch==' '){
					st=recv_response_code;
					response_code=bufp;
					break;
				}
			}
			if(bufi==buflen){
				io_request_read();
				return;
			}
		}
		if(st==recv_response_code){
			while(bufi<buflen){
				const char ch=*bufp++;
				bufi++;
				if(ch==' '){
					*(bufp-1)=0;
					strtrm(response_code,bufp-2);
					response_text=bufp;
					st=recv_response_text;
					break;
				}
				if(ch=='\n'){
					*(bufp-1)=0;
					strtrm(response_code,bufp-2);
					st=recv_header_key;
					headerp=bufp;
					break;
				}
			}
			if(bufi==buflen){
				io_request_read();
				return;
			}
		}
		if(st==recv_response_text){
			while(bufi<buflen){
				const char ch=*bufp++;
				bufi++;
				if(ch=='\n'){
					*(bufp-1)=0;
					strtrm(response_code,bufp-2);
					st=recv_header_key;
					headerp=bufp;
					break;
				}
			}
			if(bufi==buflen){
				io_request_read();
				return;
			}
		}
		if(st==recv_header_key){
			while(bufi<buflen){
				const char ch=*bufp++;
				bufi++;
				if(ch==':'){
					// copy key //? assumes request headers read in one read
					const size_t lenz=(size_t)(bufp-headerp);
					header_key=(char*)malloc(lenz);
					memcpy(header_key,headerp,lenz-1);
					*(header_key+lenz-1)=0;
//					header_key=strtrm(header_key,header_key+lenz-2);
					strlwr(header_key);
					st=recv_header_value;
					headerp=bufp;
					break;
				}
				if(ch=='\n'){
					st=recv_content;
					break;
				}
			}
			if(bufi==buflen){
				io_request_read();
				return;
			}
		}
		if(st==recv_header_value){
			while(bufi<buflen){
				const char ch=*bufp++;
				bufi++;
				if(ch=='\n'){
					*(bufp-1)=0;
					headerp=strtrmleft(headerp,bufp-1);
					const char*bufp_end=strtrmright(headerp,bufp-1);
					const size_t lenz=(size_t)(bufp_end-headerp+2);// including \0
					char*s=(char*)malloc(lenz);
					memcpy(s,headerp,lenz);
					headers.put(header_key,s);
					header_key=nullptr;
					headerp=bufp;
					st=recv_header_key;
					break;
				}
			}
			if(bufi==buflen){
				io_request_read();
				return;
			}
		}
		if(st==recv_content){
			const char*s=headers["content-length"];
			if(s){
				content_length=atoi(s);//? strtol
				content_index=0;
				const size_t rem=buflen-bufi;
				const size_t read_size=content_length<=rem?content_length:rem;
				if(conf::print_content)
					write(1,bufp,read_size);
				on_content(bufp,read_size,content_length);
				bufi+=read_size;
				content_index+=read_size;
				if(content_index!=content_length){
					st=recv_content_sized;
					if(bufi==buflen)io_request_read();//? if
					return;
				}
				st=next_req;
				if(st==next_req and !repeat_request_after_done)
					throw"close";//? return or throw
				continue;
			}else{
				const char*t=headers["transmission"];
				if(t&&!strcmp(t,"chunked"))
					st=recv_content_chuncked;
			}
		}
		if(st==recv_content_sized){
			const size_t rem=content_length-content_index;
			const size_t read_size=rem>bufsize?bufsize:rem;
			if(conf::print_content)
				write(1,buf,read_size);
			on_content(buf,read_size,content_length);
			bufi+=read_size;
			content_index+=read_size;
			if(content_index==content_length){
				st=next_req;
				if(st==next_req and !repeat_request_after_done)
					throw"close";//? return or throw
				continue;
			}
			if(bufi==buflen){
				io_request_read();
				return;
			}
		}else if(st==recv_content_chuncked){
			// chunk header
				// 0 length end of send
			// read loop
			// chunk delimiter

		}
	}}
	void on_content(/*scan*/const char*buf,size_t buflen,size_t totallen){
//		printf("*** %zu  %zu   %s\n",buflen,totallen,buf);
	}
private:
	inline size_t io_send(const void*buf,size_t len,bool throw_if_send_not_complete=false){
		const ssize_t c=send(sockfd,buf,len,MSG_NOSIGNAL);
		if(c<0){
			if(errno==EPIPE||errno==ECONNRESET)throw"brk";
			throw"send";
		}
		meters::output+=c;
		if(conf::print_traffic){
			write(1,buf,c);
		}
		if(throw_if_send_not_complete&&(size_t)c!=len)throw"incompletesend";
		return(size_t)c;
	}
	inline size_t io_recv(void*buf,size_t maxlen){
		const ssize_t c=recv(sockfd,buf,maxlen,0);
		if(c==0)return 0;// closed by client
		if(c<0){// error
			if(errno==EAGAIN||errno==EWOULDBLOCK){
				io_request_read();
			}else if(errno==ECONNRESET){
				throw"closed2";
			}
			throw"recverr";
		}
		meters::input+=c;
		if(conf::print_traffic)write(1,buf,c);
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
	inline static char*strtrm(char*p,char*e){
		while(p!=e&&isspace(*p))p++;
		while(p!=e&&isspace(*e))*e--=0;
		return p;
	}
	inline static char*strtrmleft(char*p,char*e){
		while(p!=e&&isspace(*p))p++;
		return p;
	}
	inline static char*strtrmright(char*p,char*e){
		while(p!=e){
			const char ch=*e;
			if(!ch){e--;continue;}
			if(!isspace(ch))break;
			*e=0;
			e--;
		}
		return e;
	}
	inline static void strlwr(char*p){
		while(*p){
			*p=(char)tolower(*p);
			p++;
		}
	}

};}
