#pragma once
#include"meters.hpp"
#include"span.hpp"
#include"spanb.hpp"
#include"stringbuf.hpp"
#include<string.h>
#include<netdb.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<errno.h>
#include<unistd.h>
#include<cassert>
#include<stdlib.h>
namespace xiix{class sock final{
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
//			if(!buf.needs_read())
//				throw"incompleteparse";
			buf.clr();
			const size_t nn=io_recv((void*)buf.pos(),bufsize);
			if(nn==0)throw"brk";
			buf.set_len(nn);
		}
		parse_buf();
	}
	inline void disconnect(){close(sockfd);}

private:
	int epollfd{0};
	int sockfd{0};
	const char*hostname{nullptr};
	const int port{0};
	const char*uri{"/"};
	struct epoll_event ev;

	enum state{waiting_to_send_next_request,reading_firstline,reading_headers,reading_content_sized,reading_content_chunked,reading_content_until_disconnect,uploading,sending_content};
	state st{waiting_to_send_next_request};
	enum state_chunked{size,data,delimiter,delimiter_end};
	state_chunked stc{size};

	bool repeat_request_after_done{false};
	bool is_first_request{true};

	class{
		span sp{nullptr,0};
	public:
		inline void set_span(const span&s){sp=s;}
		inline bool assert_protocol(){return false;}
		inline span get_result_code()const{return span(nullptr,0);}
		inline span get_result_text()const{return span(nullptr,0);}
	}request;


	class{
		char b[4]{'\r','\n','\r','\n'};
		int i;
	public:
		inline void rst(){i=0;}
		inline bool put(const char c){
			if(b[i]==c)i++;else i=0;
			return i==sizeof b;
		}
	}matcher;

	class{
		span sp{nullptr,0};
	public:
		inline void set_span(const span&s){sp=s;}
		inline span operator[](const char*key){return get_header_value(key);}
		inline void clr(){sp={nullptr,0};}
		inline span get_header_value(const char*key){
			const char*p=sp.ptr();
			while(true){
				const char ch=*p;
				const size_t pos=p-sp.ptr();
				if(pos>=sp.length())
					return span(nullptr,0);
//				if(ch=='\n')return span(nullptr,0);
				const char*ky{p};
				while(*p++!=':');//? unsafe
				span keysp(ky,p-ky);
				const char*value{p};
				while(*p++!='\n');//? unsafe
				if(keysp.unsafe__starts_with_str(key)){
					return span(value,p-value);
				}
			}
		}
	}header;

	class{
		size_t len{0};
		size_t pos{0};
	public:
		inline void init_for_recieve(const size_t nbytes){pos=0;len=nbytes;}
		inline void clr(){len=pos=0;}
		inline size_t rem()const{return len-pos;}
		inline size_t length()const{return len;}
		inline bool needs_read()const{return pos!=len;}
		inline void unsafe_pos_inc(const size_t nbytes){pos+=nbytes;}
	}content;

	class{
		size_t len{0};
		size_t pos{0};
	public:
		inline void init_for_receive(const size_t chunksize){len=chunksize;pos=0;}
		inline void clr(){len=pos=0;}
		inline size_t rem()const{return len-pos;}
		inline size_t length()const{return len;}
		inline bool needs_read()const{return pos!=len;}
		inline void unsafe_pos_inc(const size_t nbytes){pos+=nbytes;}
	}chunk;

	static const size_t bufsize{4*1024};
	class{
		char b[bufsize+1];
		char*e{b};//position
		char*eob{b};//end of buffer
	public:
		inline bool needs_read()const{return e==eob;}
		inline void clr(){e=b;*b='\0';}
		inline size_t rem()const{return eob-e;}
		inline void set_len(const size_t len){eob=b+len;*eob='\0';}
		inline const char*pos()const{return e;}
		inline char unsafe_next_char(){return *e++;}
		inline void unsafe_pos_inc(const size_t nbytes){e+=nbytes;}
		inline span get_span()const{return span(b,e-b-1);}

	}buf;

	const char*header_start_ptr{nullptr};
	stringbuf sb;

	inline void clear_for_next_request(){
		content.clr();
		chunk.clr();
		sb.rst();
		if(!buf.needs_read())throw"bufp!=bufe";
		buf.clr();
		stc=state_chunked::size;
		header_start_ptr={nullptr};
		matcher.rst();
	}
	inline void parse_buf(){char prevch{0};loop(){
		if(st==waiting_to_send_next_request){
			if(is_first_request){
				is_first_request=false;
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
			st=reading_firstline;
			io_request_read();
			return;
		}
		if(st==reading_firstline){
			while(!buf.needs_read()){
				const char ch=buf.unsafe_next_char();
				if(ch=='\n'){
					const span spn=buf.get_span();
					request.set_span(spn);
					header_start_ptr=buf.pos();
					header.clr();
					matcher.rst();
					st=reading_headers;
					break;
				}
			}
			if(buf.needs_read()){io_request_read();return;}
		}
		if(st==reading_headers){
			while(!buf.needs_read()){
				const char ch=buf.unsafe_next_char();
				if(!matcher.put(ch))
					continue;
				const span spn(header_start_ptr,buf.pos()-header_start_ptr-1);
				header.set_span(spn);

				span s=header["Content-Length"];
				if(s.length()>0){
					content.init_for_recieve(atoi(s.ptr()));
					const size_t rem=buf.rem();
					const size_t read_size=content.length()<=rem?content.length():rem;
					if(read_size>0){
						if(conf::print_content)
							write(1,buf.pos(),read_size);
						on_content(buf.pos(),read_size,content.length());
						buf.unsafe_pos_inc(read_size);
						content.unsafe_pos_inc(read_size);
					}
					if(content.needs_read()){
						st=reading_content_sized;
						io_request_read();
						return;
					}
					st=waiting_to_send_next_request;
					if(st==waiting_to_send_next_request and !repeat_request_after_done)
						throw"close";//? return or throw
					break;
				}else{
					span t=header["Transfer-Encoding"];
					while(isspace(*t.ptr()))//? subspan_trim()
						t=t.subspan(t.ptr()+1,t.length()-1);
					if(t.unsafe__starts_with_str("chunked")){
//					if(!t.is_empty() and !strncmp(t.ptr(),"chunked",sizeof "chunked")){
						st=reading_content_chunked;
						stc=state_chunked::size;
						sb.rst();
						break;

					}else{
						span s=header["Connection"];
						if(!s.is_empty() and !strcmp("close",s.ptr())){
							const size_t rem=buf.rem();
							if(conf::print_content)
								write(1,buf.pos(),rem);
							on_content(buf.pos(),rem,0);//? 0
							buf.unsafe_pos_inc(rem);
							st=reading_content_until_disconnect;
						}else
							throw"unknowntransfertype";
					}
				}
			}
			if(st==waiting_to_send_next_request)continue;
			if(buf.needs_read()){io_request_read();return;}
		}
		if(st==reading_content_sized){
			const size_t rem=content.rem();
			const size_t read_in_buffer=buf.rem();
			const size_t read_size=rem>read_in_buffer?read_in_buffer:rem;
			if(conf::print_content)
				write(1,buf.pos(),read_size);
			on_content(buf.pos(),read_size,content.length());
			content.unsafe_pos_inc(read_size);
			buf.unsafe_pos_inc(read_size);
			if(!content.needs_read()){
				st=waiting_to_send_next_request;
				if(st==waiting_to_send_next_request and !repeat_request_after_done)
					throw"close";
				continue;
			}
			if(buf.needs_read()){io_request_read();return;}
		}else if(st==reading_content_chunked){
			if(stc==state_chunked::size){
				while(!buf.needs_read()){
					const char ch=buf.unsafe_next_char();
					sb.append(ch);
					if(ch=='\n'){// chunk header e.g. "f07\r\n"
						sb.backspace('\0');
						sb.trimright();
						sb.append('\0');
					    char hex_str[32];
					    char*errorptr;
					    sb.copy_to(hex_str,sizeof(hex_str));
					    const size_t chunklen=strtoul(hex_str,&errorptr,16);
						if(*errorptr)throw"chunksizefromhex";
					    chunk.init_for_receive(chunklen);
						if(chunklen==0)
							stc=state_chunked::delimiter_end;
						else
							stc=state_chunked::data;
					    break;
					}
				}
				if(buf.needs_read()){io_request_read();return;}
			}
			if(stc==state_chunked::data){
				const size_t rem_buf=buf.rem();
				const size_t rem_chunk=chunk.rem();
				const size_t read_size=rem_chunk<=rem_buf?rem_chunk:rem_buf;
				if(conf::print_content)
					write(1,buf.pos(),read_size);
				on_content(buf.pos(),read_size,chunk.length());
				buf.unsafe_pos_inc(read_size);
				chunk.unsafe_pos_inc(read_size);
				if(chunk.needs_read()){
					if(buf.needs_read()){io_request_read();return;}
					throw"illegalstate";
				}
				stc=state_chunked::delimiter;
			}
			if(stc==state_chunked::delimiter){
				while(!buf.needs_read()){
					const char ch=buf.unsafe_next_char();
					if(ch=='\n'){// delimiter: "\r\n"
						sb.rst();
						stc=state_chunked::size;
					    break;
					}
				}
				if(buf.needs_read()){io_request_read();return;}
				continue;
			}else if(stc==state_chunked::delimiter_end){
				while(!buf.needs_read()){
					const char ch=buf.unsafe_next_char();
					if(ch=='\n'){// delimiter: "\r\n"
						st=waiting_to_send_next_request;
						if(st==waiting_to_send_next_request and !repeat_request_after_done)
							throw"close";
					    break;
					}
				}
				if(st!=waiting_to_send_next_request and buf.needs_read()){io_request_read();return;}
				continue;
			}
		}else if(st==reading_content_until_disconnect){
			const size_t rem=buf.rem();
			if(conf::print_content)
				write(1,buf.pos(),rem);
			on_content(buf.pos(),rem,0);//? maxsizet
			buf.unsafe_pos_inc(rem);
			io_request_read();
			return;
		}
	}}
	inline void on_content(/*scan*/const char*buf,size_t buflen,size_t totallen){
//		printf("*** %zu  %zu   %s\n",buflen,totallen,buf);
	}
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
