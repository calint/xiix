#pragma once
#include<cassert>
#include<string.h>
namespace xiix{class spanb:span{
	char*bb{nullptr};// begin of string
	char*be{nullptr};// cursor  >bb and <len
public:
	inline spanb(char*buf,const size_t len):span{(const char*)buf,len},bb{buf},be{buf}{}
	inline spanb&p(const char ch){
		assert((be-bgn)<(signed)len);
		*be++=ch;
		return*this;
	}
	inline spanb&p(const span&s){
		const size_t sn=s.len();
		assert((be-bgn+sn)<len);
		memcpy(bb,s.ptr(),sn);
		be+=sn;
		return*this;
	}
	inline spanb&p(const char*str){
		const size_t ln=strlen(str);
		assert(len-(be-bgn)-ln);
		strncpy(be,str,ln);
		be+=ln;
		return*this;
	}
	inline spanb&write_to(int fd){
		const ssize_t ln=be-bb;
		const ssize_t n=write(fd,bb,ln);
		if(n<0)throw"write";
		if((unsigned)n!=ln)throw"writeincomplete";
		return*this;
	}
	inline size_t string_size()const{
		return be-bb;
	}
	inline const span string_span()const{
		const span o=sub((const char*)bb,string_size());
		return o;
	}
};}
