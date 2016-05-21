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
			if(bufi!=bufnn)
				throw"incompleteparse";
			if(st==state::recv_content_chuncked and stc==state_chunked::size and chunk_size_hex_str==bufp){
				chunk_size_hex_str=buf;
//				throw"!!!";
			}
			bufnn=io_recv(buf,bufsize);
			bufp=buf;
			bufe=buf+bufnn;
			*(bufe+1)='\0';
			bufi=0;
		}
		parse_buf();
	}
	inline void disconnect(){
		close(sockfd);
	}

private:
	enum state{next_req,send_req,recv_response_protocol,recv_response_code,recv_response_text,recv_header_key,recv_header_value,recv_content,recv_content_sized,recv_content_chuncked};
	enum state_chunked{size,data,delimiter,delimiter_end};
	state st{next_req};
	static const size_t bufsize{4*1024};
	char buf[bufsize+1];
	size_t bufi{0};
	size_t bufnn{0};
	char*bufp{nullptr};
	char*bufe{nullptr};
	char*response_code{nullptr};
	char*response_text{nullptr};
	lul<const char*>headers{true,true};
	char*headerp{nullptr};
//	char*header_value{nullptr};
	char*header_key{nullptr};
	size_t content_length{0};
	size_t content_index{0};
	bool repeat_request_after_done{false};
	bool first_request{true};
	state_chunked stc{size};
	char*chunk_size_hex_str{nullptr};
	size_t chunk_size_in_bytes{0};
	size_t chunk_pos_in_bytes{0};

	inline void clear_for_next_request(){
		response_code=response_text=nullptr;
		content_length=content_index=0;
		stc=state_chunked::size;
		chunk_size_hex_str=nullptr;
		chunk_size_in_bytes=0;
		headerp=nullptr;
		response_code=response_text=nullptr;
		headers.clear();
		if(bufi!=bufnn)throw"bufi!=bufnn";
		bufi=bufnn=0;
		*buf=0;
		bufp=buf;
		bufe=buf+1;
	}
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
			clear_for_next_request();
			meters::requests++;
			char s[1024];
			const size_t n=(port!=80)?
				snprintf(s,sizeof s,"GET %s HTTP/1.1\r\nHost: %s:%d\r\n\r\n",uri,hostname,port):
				snprintf(s,sizeof s,"GET %s HTTP/1.1\r\nHost: %s\r\n\r\n",uri,hostname);
			io_send(s,n,true);
//			headers=lul<const char*>{true,true};//? does not invoke delete on previous
//			headers.clear();
			st=recv_response_protocol;
			io_request_read();
			return;
		}
		if(st==recv_response_protocol){
			while(bufi<bufnn){
				const char ch=*bufp++;
				bufi++;
				if(ch==' '){
					st=recv_response_code;
					response_code=bufp;
					break;
				}
			}
			if(bufi==bufnn){
				io_request_read();
				return;
			}
		}
		if(st==recv_response_code){
			while(bufi<bufnn){
				const char ch=*bufp++;
				bufi++;
				if(ch==' '){
					*(bufp-1)=0;
					const int code=atoi(response_code);
					if(code!=200)
						throw"responsecode";
					response_text=bufp;
					st=recv_response_text;
					break;
				}
				if(ch=='\n'){
					*(bufp-1)=0;
					const int code=atoi(response_code);
					response_code=nullptr;//? y
					if(code!=200)
						throw"responsecode";
					st=recv_header_key;
					headerp=bufp;
					break;
				}
			}
			if(bufi==bufnn){
				io_request_read();
				return;
			}
		}
		if(st==recv_response_text){
			while(bufi<bufnn){
				const char ch=*bufp++;
				bufi++;
				if(ch=='\n'){
					*(bufp-1)=0;
					response_text=strtrmleft(response_text, bufp-1);
					strtrmright(response_text,bufp-2);
					st=recv_header_key;
					headerp=bufp;
					break;
				}
			}
			if(bufi==bufnn){
				io_request_read();
				return;
			}
		}
		if(st==recv_header_key){
			while(bufi<bufnn){
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
//					headerp=nullptr;//? y
					st=recv_header_value;
					headerp=bufp;
					break;
				}
				if(ch=='\n'){headerp=nullptr;
					st=recv_content;
					break;
				}
			}
			if(bufi==bufnn){
				io_request_read();
				return;
			}
		}
		if(st==recv_header_value){
			while(bufi<bufnn){
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
			if(bufi==bufnn){
				io_request_read();
				return;
			}
		}
		if(st==recv_content){
			const char*s=headers["content-length"];
			if(s){
				content_length=atoi(s);//? strtol
				content_index=0;
				const size_t rem=bufnn-bufi;
				const size_t read_size=content_length<=rem?content_length:rem;
				if(conf::print_content)
					write(1,bufp,read_size);
				on_content(bufp,read_size,content_length);
				bufi+=read_size;
				bufp+=read_size;
				content_index+=read_size;
				if(content_index!=content_length){
					st=recv_content_sized;
					if(bufi==bufnn)io_request_read();//? if
					return;
				}
				st=next_req;
				if(st==next_req and !repeat_request_after_done)
					throw"close";//? return or throw
				continue;
			}else{
				const char*t=headers["transfer-encoding"];
				if(t and !strcmp(t,"chunked")){
					st=recv_content_chuncked;
					stc=state_chunked::size;
					chunk_size_hex_str=bufp;
					if(!isxdigit(*chunk_size_hex_str))
						throw"malformed";

				}else throw"unknowntransfertype";
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
			if(bufi==bufnn){
				io_request_read();
				return;
			}
		}else if(st==recv_content_chuncked){
			if(stc==state_chunked::size){
				while(bufi<bufnn){
					const char ch=*bufp++;
					bufi++;
					if(ch=='\n'){// chunk header e.g. "f07\r\n"
						*(bufp-1)=0;
						if(!isxdigit(*chunk_size_hex_str))
							throw"malformed";
					    char*p;
					    strtrmright(chunk_size_hex_str,bufp-2);
					    chunk_size_in_bytes=strtoul(chunk_size_hex_str,&p,16);
						if(*p)
							throw"chunksizefromhex";
						chunk_pos_in_bytes=0;		chunk_size_hex_str=nullptr;
						if(chunk_size_in_bytes==0)
							stc=state_chunked::delimiter_end;
						else
							stc=state_chunked::data;
					    break;
					}
				}
				if(bufi==bufnn){
					io_request_read();
					return;
				}
			}
			if(stc==state_chunked::data){
				const size_t rem_buf=bufnn-bufi;
				const size_t rem_chunk=chunk_size_in_bytes-chunk_pos_in_bytes;
				const size_t read_size=rem_chunk<=rem_buf?rem_chunk:rem_buf;
				if(conf::print_content)
					write(1,bufp,read_size);
				on_content(bufp,read_size,chunk_size_in_bytes);
				bufi+=read_size;
				bufp+=read_size;
				chunk_pos_in_bytes+=read_size;
				if(chunk_pos_in_bytes!=chunk_size_in_bytes){
					if(bufi==bufnn)
						io_request_read();//? if
					return;
				}
				stc=state_chunked::delimiter;
			}
			if(stc==state_chunked::delimiter){
				while(bufi<bufnn){
					const char ch=*bufp++;
					bufi++;
					if(ch=='\n'){// delimiter: "\r\n"
						chunk_size_hex_str=bufp;
						stc=state_chunked::size;
					    break;
					}
				}
				if(bufi==bufnn){
					io_request_read();
					return;
				}
				continue;
			}
			if(stc==state_chunked::delimiter_end){
				while(bufi<bufnn){
					const char ch=*bufp++;
					bufi++;
					if(ch=='\n'){// delimiter: "\r\n"

						st=next_req;
						if(st==next_req and !repeat_request_after_done)
							throw"close";//? return or throw
					    break;
					}
				}
				if(st!=next_req and bufi==bufnn){
					io_request_read();
					return;
				}
				continue;
			}
		}
	}}

	inline void on_content(/*scan*/const char*buf,size_t buflen,size_t totallen){
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
