#pragma once
#include<cassert>
#include<unistd.h>
namespace xiix{class span{
protected:
	const char*pt{nullptr};
	size_t len{0};
public:
	inline span(const span&o):pt{o.pt},len{o.len}{}
	inline span&operator=(const span&o){pt=o.pt;len=o.len;return*this;}
	inline span(const char*buffer,const size_t size):pt{buffer},len{size}{}
	inline const char*ptr()const{return pt;}
	inline size_t length()const{return len;}
	inline bool is_empty()const{return len==0;}
	inline span subspan(const char*start,const size_t size_in_bytes)const{
		assert(start>=pt  and  (start+size_in_bytes)<(pt+len));
		span s=span(start,size_in_bytes);
		return s;
	}
	inline span&write_to(int fd){
		const ssize_t nn=write(fd,pt,len);
		if(nn<0)throw"write";
		if((unsigned)nn!=len)throw"writeincomplete";
		return*this;
	}
};}
