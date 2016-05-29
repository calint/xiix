#pragma once
#include<cassert>
#include<unistd.h>
namespace xiix{template<class T>class range{
protected:
	const T b{nullptr};
	const T e{nullptr};
public:
	inline range(){}
	inline range(T&s):b{s.b},e{s.e}{}
	inline range(const T begin,const T end):b{begin},e{end}{assert(end>=begin);}
	inline T&operator=(const T&s){b=s.b;e=s.e;return*this;}
	inline bool operator==(const T&s){return b==s.b and e==s.e;}
	inline T begin()const{return b;}
	inline T end()const{return e;}
	inline size_t len()const{return e-b;}
	inline bool isempty()const{return b==e;}
	inline span trimleft()const{
		const char*p{b};
		while(true){
			if(p==e)break;
			if(!isspace(*p))break;
			p++;
		}
		return span{p,e};
	}
	inline span&write_to(int fd){
		const size_t ln=len();
		const ssize_t nn=write(fd,b,ln);
		if(nn<0)throw"write";
		if((unsigned)nn!=ln)throw"writeincomplete";
		return*this;
	}
	inline bool startswithstr(const char*s){
		const char*p{b};
		while(true){
			if(!*s)return true;
			if(p==e)return false;
			if(*s!=*p)return false;
			s++;
			p++;
		}
	}
};}
