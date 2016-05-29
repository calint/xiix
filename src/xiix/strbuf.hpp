#pragma once
#include<string.h>
#include<cassert>
namespace xiix{class strbuf{
	char bb[strbuf_size];
	char*b{bb};
	char*e{bb};
	const char*end{bb+strbuf_size};
public:
	inline void rst(){e=b=bb;*e=0;}
	inline size_t len()const{return e-b;}
	inline void append(const char&ch){assert(e!=end);*e++=ch;}
	inline void copy_to(char*buf,size_t sizeof_buf){
		const size_t len=e-b;
		const size_t ln=len>sizeof_buf?sizeof_buf:len;
		strncpy(buf,b,ln);
	}
	inline void trimright(){
		while(e!=b){
			const char ch=*(e-1);
			if(!isspace(ch))break;
			*--e='\0';
		}
	}
	inline void trimleft(){
		while(b!=end){
			const char ch=*b;
			if(isspace(ch)){b++;continue;}
			break;
		}
	}
	inline void tolowercase(){
		char*p=b;
		while(p!=end){
			const char ch=*p;
			const char chl=tolower(ch);
			*p=chl;
			p++;
		}
	}
	inline void backspace(const char replacement_char){assert(e!=b);e--;*e=replacement_char;}
};}
