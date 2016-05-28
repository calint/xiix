#pragma once
#include"ns.hpp"
#include<string.h>
namespace xiix{class strbuf{
	char bb[strbuf_size];
	char*p{bb};
	char*b{bb};
	const char*e{bb+strbuf_size};
public:
	inline void rst(){p=b=bb;*p=0;}
	inline size_t len()const{return p-b;}
	inline void append(const char&ch){assert(p!=e);*p++=ch;}
	inline void copy_to(char*buf,size_t sizeof_buf){
		const size_t len=p-b;
		const size_t ln=len>sizeof_buf?sizeof_buf:ln;
		strncpy(buf,b,ln);
	}
	inline void trimright(){
		while(p!=b){
			const char ch=*(p-1);
			if(!isspace(ch))break;
			*--p='\0';
		}
	}
	inline void trimleft(){
		while(b!=e){
			const char ch=*b;
			if(isspace(ch)){b++;continue;}
			break;
		}
	}
	inline void tolowercase(){
		char*p=b;
		while(p!=e){
			const char ch=*p;
			const char chl=tolower(ch);
			*p=chl;
			p++;
		}
	}
	inline void backspace(const char replacement_char){assert(p!=b);p--;*p=replacement_char;}
};}
