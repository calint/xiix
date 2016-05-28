#pragma once
#include<cassert>
#include<unistd.h>
namespace xiix{class span{
protected:
	const char*bgn{nullptr};
	const char*end{nullptr};
public:
	inline span(){}
	inline span(const char*buf,const size_t sizeofbuf):bgn{buf},end{buf+sizeofbuf}{}
	inline span(const span&s):bgn{s.bgn},end{s.end}{}
	inline span(const char*begin,const char*end):bgn{begin},end{end}{assert(end>=begin);}
	inline span&operator=(const span&s){bgn=s.bgn;end=s.end;return*this;}
	inline const char*ptr()const{return bgn;}
	inline size_t len()const{return end-bgn;}
	inline bool isempty()const{return bgn==end;}
//	inline span sub(const char*start,const size_t size)const{
//		assert(start>=pt and (start+size)<=(pt+ln));
//		span s=span(start,size);
//		return s;
//	}
	inline span trimleft()const{
		const char*p{bgn};
		while(true){
			if(p==end)break;
			if(!isspace(*p))break;
			p++;
		}
		return span(p,end);
	}
	inline span&write_to(int fd){
		const size_t ln=len();
		const ssize_t nn=write(fd,bgn,ln);
		if(nn<0)throw"write";
		if((unsigned)nn!=ln)throw"writeincomplete";
		return*this;
	}
	inline bool startswithstr(const char*s){
		const char*p{bgn};
		while(true){
			if(!*s)return true;
			if(p==end)return false;
			if(*s!=*p)return false;
			s++;
			p++;
		}
	}
};}
