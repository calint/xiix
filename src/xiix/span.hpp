#pragma once
#include<cassert>
#include<unistd.h>
namespace xiix{class span{
protected:
	const char*pt{nullptr};
	size_t ln{0};
public:
	inline span(const char*buf,const size_t size):pt{buf},ln{size}{}
	inline span(const span&s):pt{s.pt},ln{s.ln}{}
	inline span&operator=(const span&s){pt=s.pt;ln=s.ln;return*this;}
	inline const char*ptr()const{return pt;}
	inline size_t len()const{return ln;}
	inline bool isempty()const{return ln==0;}
	inline span sub(const char*start,const size_t size)const{
		assert(start>=pt and (start+size)<=(pt+ln));
		span s=span(start,size);
		return s;
	}
	inline span trimleft()const{
		const char*p{pt};
		const char*pe{pt+ln};
		while(true){
			if(p==pe)break;
			if(!isspace(*p))break;
			p++;
		}
		return span(p,pe-p);
	}
	inline span&write_to(int fd){
		const ssize_t nn=write(fd,pt,ln);
		if(nn<0)throw"write";
		if((unsigned)nn!=ln)throw"writeincomplete";
		return*this;
	}
	inline bool startswithstr(const char*s){
		const char*p{pt};
		const char*pe{pt+ln};
		while(true){
			if(!*s)return true;
			if(p==pe)return false;
			if(*s!=*p)return false;
			s++;
			p++;
		}
	}
};}
