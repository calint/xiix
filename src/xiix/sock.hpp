#pragma once
#include"meters.hpp"
#include"lul.hpp"
#include<string.h>
#include<netdb.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<cassert>
namespace xiix{










class span final{
	char*p{nullptr};
	size_t n;
public:
	inline span(const span&o){
		p=o.p;
		n=o.n;
	}
	inline span(const char*buffer,const size_t size):p{(char*)buffer},n{size}{}
	inline const char*ptr()const{return p;}
	inline size_t size_in_bytes()const{return n;}
	inline const span subspan(const char*start,const size_t size_in_bytes)const{
//		printf(" %d     %d   \n",start>=p,(start+size_in_bytes)<(p+n));
		assert(start>=p  and  (start+size_in_bytes)<(p+n));
		span s=span(start,size_in_bytes);
		return s;
	}
	inline span&write_to(int fd){
		const ssize_t nn=write(fd,p,n);
		if(nn<0)throw"write";
		if((unsigned)nn!=n)throw"writeincomplete";
		return*this;
	}


	//	inline span&operator=(const span&o){p=o.p;s=o.s;return*this;}

};

class buffic final{
	span s;// span
	char*bb{nullptr};// begin of string
	char*be{nullptr};// end of string + 1
public:
	inline buffic(const span&sp):s(sp){
		bb=be=(char*)s.ptr();
	}
	inline buffic&use(const span&sp){
		s=sp;
		bb=be=(char*)s.ptr();
		return*this;
	}
	inline buffic&reset(){
		bb=be=(char*)s.ptr();
		return*this;
	}
	inline size_t string_size_in_bytes(){
		return be-bb;
	}
	inline buffic&p(const char ch){
		assert((be-s.ptr())<s.size_in_bytes());
		*be++=ch;
		return*this;
	}
	inline buffic&p(const span&o){
		const size_t sn=o.size_in_bytes();
		assert((be-s.ptr()+sn)<s.size_in_bytes());
		memcpy(bb,o.ptr(),sn);
		be+=sn;
		return*this;
	}
	inline buffic&write_to(int fd){
		const ssize_t ln=be-bb;
		const ssize_t n=write(fd,bb,ln);
		if(n<0)throw"write";
		if((unsigned)n!=ln)throw"writeincomplete";
		return*this;
	}
	inline const span string_span(){//? const
		const span sp=s.subspan((const char*)bb,string_size_in_bytes());
		return sp;
	}
};

enum class states{waiting_to_send_next_request,reading_firstline,reading_headers,reading_content_sized,read_content_chunked,reading_content_until_disconnect,uploading_cont,sending_content_cont};

class firstline final{
	char buf[4096];
	buffic b{span{buf,sizeof buf}};
public:
	inline firstline(){*buf='\0';}
	inline const span&assert_protocol(){return std::move(span{buf,2});}
	inline const span&get_result_code()const{return std::move(span{buf,2});}
	inline const span&get_result_text()const{return std::move(span{buf,2});}
	inline void copy_from(const span&spn){b.p(spn);}
};

class headers final{
	char buf[4096];
	buffic b{span{buf,sizeof buf}};
public:
	inline headers(){*buf='\0';}
	inline const char*operator[](const char*key){
		return get_header_value(key);
	}
	inline const char*get_header_value(const char*key){
		return "close";
	}
	inline void copy_from(const span&spn){
		b.p(spn);
	}
};


















class sock final{
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
//		char bb[5];
//		span s(bb,sizeof bb);
//		buffic b(s);
//		b.p('a');
//		b.p('b');
//		b.p('c');
//		b.write_to(1);
////		span s2=s.subspan(s.ptr()+1,8);
//		span s2=s.subspan(s.ptr()+1,2);
//		s2.write_to(1);
//
//		char bb2[32];
//		buffic b2(span(bb2,sizeof bb2));
//		b2.p(b.string_span());
//		b2.p('\0');
//		b2.write_to(1);
//





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
			bufnn=io_recv(buf,bufsize);
			if(bufnn==0)
				throw"brk";
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
	enum state{next_req,send_req,recv_response_protocol,recv_response_code,recv_response_text,recv_header_key,recv_header_value,recv_content,recv_content_sized,recv_content_chuncked,recv_content_until_connection_closed};
	enum state_chunked{size,data,delimiter,delimiter_end};
	state st{next_req};
	static const size_t bufsize{4*1024};
	char buf[bufsize+1];
	size_t bufi{0};
	size_t bufnn{0};
	char*bufp{nullptr};
	char*bufe{nullptr};
	char*response_code{nullptr};
//	char*response_text{nullptr};
	lul<const char*>headers{true,true};
//	char*headerp{nullptr};
//	char*header_value{nullptr};
	char*header_key{nullptr};
	size_t content_length{0};
	size_t content_index{0};
	bool repeat_request_after_done{false};
	bool first_request{true};
	state_chunked stc{size};
//	char*chunk_size_hex_str{nullptr};
	size_t chunk_size_in_bytes{0};
	size_t chunk_pos_in_bytes{0};

	char strbuf[1024];
	const size_t strbufsize{sizeof(strbuf)};
	char*strbufp{strbuf};
	char*strbufb{strbuf};
	char*strbufe{strbuf+strbufsize};
	inline void strbuf_rst(){
		strbufp=strbufb=strbuf;
		strbufe=strbuf+strbufsize;
		*strbufp=0;
	}
	inline size_t strbuf_len()const{
		return strbufp-strbufb;
	}
	inline void strbuf_add(const char&ch){
		if((size_t)(strbufp-strbuf)==strbufsize)throw"overflow";
		*strbufp++=ch;
	}
	inline void strbuf_copy_to(char*buf,size_t buflen){
		const size_t len=strbufp-strbufb;
		strncpy(buf,strbufb,len);
	}
	inline void strbuf_trimright(){
		while(1){
			if(strbufp==strbufb)return;
			const char ch=*(strbufp-1);
//			if(!ch){strbufp--;continue;}
			if(!isspace(ch))
				break;
			strbufp--;
			*strbufp='\0';
		}
	}
	inline void strbuf_trimleft(){
		while(strbufb!=strbufe){
			const char ch=*strbufb;
			if(isspace(ch)){strbufb++;continue;}
			break;
		}
	}
	inline void strbuf_tolowercase(){
		char*p=strbufb;
		while(p!=strbufe){
			const char ch=*p;
//			printf("\n%c\n",ch);
			const char chl=tolower(ch);
			*p=chl;
			p++;
		}
	}
	inline void strbuf_endofstring(){*strbufp='\0';}
	inline void strbuf_backspace(const char replacement_char){
		if(strbufp==strbufb)throw"underflow";
		strbufp--;
		*strbufp=replacement_char;
	}



	inline void clear_for_next_request(){
//		response_code=response_text=nullptr;
		response_code=nullptr;
		content_length=content_index=0;
		stc=state_chunked::size;
//		chunk_size_hex_str=nullptr;
		chunk_size_in_bytes=0;
		strbuf_rst();
//		headerp=nullptr;
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
//					response_text=bufp;
					st=recv_response_text;
					break;
				}
				if(ch=='\n'){
					*(bufp-1)=0;
					const int code=atoi(response_code);
					response_code=nullptr;//? y
					if(code!=200)
						throw"responsecode";
					strbuf_rst();
					st=recv_header_key;
//					headerp=bufp;
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
//					*(bufp-1)=0;
//					response_text=strtrmleft(response_text, bufp-1);
//					strtrmright(response_text,bufp-2);
					strbuf_rst();
					st=recv_header_key;
//					headerp=bufp;
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
				strbuf_add(ch);
				bufi++;
				if(ch==':'){
					// copy key //? assumes request headers read in one read
					strbuf_backspace('\0');
					strbuf_trimleft();
					strbuf_trimright();
					strbuf_tolowercase();
					strbuf_add('\0');
					const size_t len=strbuf_len();
					header_key=(char*)malloc(len);
					strbuf_copy_to(header_key,len);
					strbuf_rst();
					st=recv_header_value;
					break;
				}
				if(ch=='\n'){
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
				strbuf_add(ch);
				bufi++;
				if(ch=='\n'){
					strbuf_backspace('\0');
					strbuf_trimleft();
					strbuf_trimright();
					strbuf_add('\0');
					char*value=(char*)malloc(strbuf_len());
					strbuf_copy_to(value,strbuf_len());
					headers.put(/*gives*/header_key,/*gives*/value);
					header_key=nullptr;
					strbuf_rst();
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
					strbuf_rst();

				}else{
					const char*s=headers["connection"];
					if(s and !strcmp("close",s)){
						const size_t rem=bufnn-bufi;
						if(conf::print_content)
							write(1,bufp,rem);
						on_content(bufp,rem,0);//? 0
						bufi+=rem;
						bufp+=rem;
						st=recv_content_until_connection_closed;
					}else
						throw"unknowntransfertype";
				}
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
					strbuf_add(ch);
					bufi++;
					if(ch=='\n'){// chunk header e.g. "f07\r\n"
					    strbuf_backspace('\0');
					    strbuf_trimright();
					    strbuf_add('\0');
					    char hex_str[32];
					    char*errorptr;
					    strbuf_copy_to(hex_str,sizeof(hex_str));
					    chunk_size_in_bytes=strtoul(hex_str,&errorptr,16);
						if(*errorptr)throw"chunksizefromhex";
						chunk_pos_in_bytes=0;
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
						strbuf_rst();
						stc=state_chunked::size;
					    break;
					}
				}
				if(bufi==bufnn){
					io_request_read();
					return;
				}
				continue;
			}else if(stc==state_chunked::delimiter_end){
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
		}else if(st==state::recv_content_until_connection_closed){
			const size_t rem=bufnn-bufi;
			if(conf::print_content)
				write(1,bufp,rem);
			on_content(bufp,rem,0);//? maxsizet
			bufi+=rem;
			bufp+=rem;
			io_request_read();
			return;
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
};}
