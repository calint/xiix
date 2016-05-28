#pragma once
#include<cassert>
#include<unistd.h>
namespace xiix{class span{
protected:
	const char*pt{nullptr};
	size_t len{0};
public:
	inline span(const char*buf,const size_t size):pt{buf},len{size}{}
	inline span(const span&s):pt{s.pt},len{s.len}{}
	inline span&operator=(const span&s){pt=s.pt;len=s.len;return*this;}
	inline const char*ptr()const{return pt;}
	inline size_t length()const{return len;}
	inline bool is_empty()const{return len==0;}
	inline span subspan(const char*start,const size_t size)const{
		assert(start>=pt  and  (start+size)<=(pt+len));
		span s=span(start,size);
		return s;
	}
	inline span subspan_trim_left()const{
		const char*p{pt};
		const char*pe{pt+len};
		while(true){
			if(p==pe)break;
			if(!isspace(*p))break;
			p++;
		}
		return span(p,pe-p);
	}
	inline span&write_to(int fd){
		const ssize_t nn=write(fd,pt,len);
		if(nn<0)throw"write";
		if((unsigned)nn!=len)throw"writeincomplete";
		return*this;
	}
	inline bool unsafe__starts_with_str(const char*s){
		const char*p{pt};
		const char*pe{pt+len};
		while(true){
			if(!*s)return true;
			if(p==pe)return false;
			if(*s!=*p)return false;
			s++;
			p++;
		}
	}
};}
