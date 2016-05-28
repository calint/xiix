#pragma once
#include"ns.hpp"
#include"span.hpp"
#include"strbuf.hpp"
#include<string.h>
#include<netdb.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<errno.h>
#include<unistd.h>
#include<cassert>
#include<stdlib.h>
#include<stdio.h>

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
			buf.rst();
			const size_t nn=io_recv((void*)buf.pos(),sock_buf_size);
			if(nn==0)throw signal_connection_closed_by_server;
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
		int i{0};
	public:
		inline void rst(){i=0;}
		inline bool read(const char c){
			if(b[i]==c)i++;else i=0;
			return i==sizeof b;
		}
	}end_of_header_matcher;

	class{
		span sp{nullptr,0};
	public:
		inline void set_span(const span&s){sp=s;}
		inline span operator[](const char*key){return get_header_value(key);}
		inline void rst(){sp={nullptr,0};}
		inline span get_header_value(const char*key){
			const char*p=sp.ptr();
			while(true){
				const size_t pos=p-sp.ptr();
				if(pos>=sp.len())
					return span(nullptr,0);
				const char*ky{p};
				while(*p++!=':');//? unsafe
				span keysp(ky,p-ky);
				const char*value{p};
				while(*p++!='\n');//? unsafe
				if(keysp.startswithstr(key))
					return span(value,p-value);
			}
		}
	}header;

	class{
		size_t len{0};
		size_t pos{0};
	public:
		inline void init_for_recieve(const size_t nbytes){pos=0;len=nbytes;}
		inline void rst(){len=pos=0;}
		inline size_t rem()const{return len-pos;}
		inline size_t length()const{return len;}
		inline bool needs_read()const{return pos!=len;}
		inline void unsafe_pos_inc(const size_t nbytes){pos+=nbytes;}
	}content_range,chunk_range;

//	static const size_t bufsize{sock_buf_size};

	class{
		char b[sock_buf_size+1];//
		char*e{b};//position
		char*eob{b+sock_buf_size};//end of buffer
	public:
		inline bool needs_read()const{return e==eob;}
		inline void rst(){e=b;*b='\0';}
		inline size_t rem()const{return eob-e;}
		inline void set_len(const size_t len){eob=b+len;*eob='\0';}
		inline const char*pos()const{return e;}
		inline char unsafe_next_char(){return *e++;}
		inline void unsafe_pos_inc(const size_t nbytes){e+=nbytes;}
		inline span to_span()const{return span(b,e-b-1);}
	}buf;

	const char*header_start_ptr{nullptr};
	strbuf sb_chunk_size;

//	inline void clear_for_next_request(){
//		content_range.rst();
//		chunk_range.rst();
//		sb_chunk_size.rst();

//		if(!buf.needs_read())throw"bufp!=bufe";
//		buf.rst();

//		stc=state_chunked::size;
//
//		header.rst();
//		header_start_ptr={nullptr};
//		end_of_header_matcher.rst();
//	}
	inline void parse_buf(){loop(){
		if(st==waiting_to_send_next_request){
			if(is_first_request){
				is_first_request=false;
			}else{
				if(!repeat_request_after_done){
					disconnect();
					return;
				}
			}
//			clear_for_next_request();
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
					const span spn=buf.to_span();
					request.set_span(spn);
					header_start_ptr=buf.pos();
					header.rst();
					end_of_header_matcher.rst();

					st=reading_headers;
					header.rst();
					header_start_ptr=buf.pos();
					end_of_header_matcher.rst();
					break;
				}
			}
			if(buf.needs_read()){io_request_read();return;}
		}
		if(st==reading_headers){
			while(!buf.needs_read()){
				const char ch=buf.unsafe_next_char();
				if(!end_of_header_matcher.read(ch))
					continue;
				const span spn(header_start_ptr,buf.pos()-header_start_ptr-1);
				header.set_span(spn);
				span s=header["Content-Length"];
				if(!s.isempty()){
					content_range.init_for_recieve(atoi(s.ptr()));
					const size_t brem=buf.rem();
					const size_t len=content_range.length()<=brem?content_range.length():brem;
					if(len>0){
						on_content(buf.pos(),len,content_range.length());
						buf.unsafe_pos_inc(len);
						content_range.unsafe_pos_inc(len);
					}
					if(content_range.needs_read()){
						st=reading_content_sized;
						io_request_read();
						return;
					}
					st=waiting_to_send_next_request;
					if(st==waiting_to_send_next_request and !repeat_request_after_done)
						throw"close";

					s=header["Connection"].trimleft();
					if(s.startswithstr("close"))
						throw"close";
					break;
				}
				s=header["Transfer-Encoding"].trimleft();
				if(s.startswithstr("chunked")){
					st=reading_content_chunked;
					stc=state_chunked::size;
					sb_chunk_size.rst();
					break;
				}
				s=header["Connection"].trimleft();
				if(s.startswithstr("close")){
					const size_t brem=buf.rem();
					on_content(buf.pos(),brem,0);//? 0
					buf.unsafe_pos_inc(brem);
					st=reading_content_until_disconnect;
					break;
				}
				throw"unknowntransfertype";
			}
			if(st==waiting_to_send_next_request)continue;
			if(buf.needs_read()){io_request_read();return;}
		}
		if(st==reading_content_sized){
			const size_t crem=content_range.rem();
			const size_t brem=buf.rem();
			const size_t len=crem>brem?brem:crem;
			on_content(buf.pos(),len,content_range.length());
			content_range.unsafe_pos_inc(len);
			buf.unsafe_pos_inc(len);
			if(!content_range.needs_read()){
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
					sb_chunk_size.append(ch);
					if(ch=='\n'){// chunk header e.g. "f07\r\n"
						sb_chunk_size.backspace('\0');
						sb_chunk_size.trimright();
						sb_chunk_size.append('\0');
						char hex_str[32];
						char*errorptr;
						sb_chunk_size.copy_to(hex_str,sizeof(hex_str));
						const size_t chunklen=strtoul(hex_str,&errorptr,16);
						if(*errorptr)throw"chunksizefromhex";
						chunk_range.init_for_recieve(chunklen);
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
				const size_t brem=buf.rem();
				const size_t crem=chunk_range.rem();
				const size_t len=crem<=brem?crem:brem;
				on_content(buf.pos(),len,chunk_range.length());
				buf.unsafe_pos_inc(len);
				chunk_range.unsafe_pos_inc(len);
				if(chunk_range.needs_read()){
					if(buf.needs_read()){io_request_read();return;}
					throw"illegalstate";
				}
				stc=state_chunked::delimiter;
			}
			if(stc==state_chunked::delimiter){
				while(!buf.needs_read()){
					const char ch=buf.unsafe_next_char();
					if(ch=='\n'){// delimiter: "\r\n"
						stc=state_chunked::size;
						sb_chunk_size.rst();
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
			const size_t brem=buf.rem();
			on_content(buf.pos(),brem,0);//? maxsizet
			buf.unsafe_pos_inc(brem);
			io_request_read();
			return;
		}
	}}
	inline void on_content(/*scan*/const char*buf,size_t buflen,size_t contentlen){
//		printf("*** %zu  %zu   %s\n",buflen,totallen,buf);
		if(conf::print_content)write(1,buf,buflen);
	}
	inline size_t io_send(const void*buf,size_t len,bool throw_if_send_not_complete=false){
		const ssize_t c=send(sockfd,buf,len,MSG_NOSIGNAL);
		if(c<0){
			if(errno==EPIPE||errno==ECONNRESET)throw signal_connection_closed_by_server;
			throw"send";
		}
		meters::output+=c;
		if(conf::print_traffic)write(1,buf,c);
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
				throw signal_connection_closed_by_server;
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
