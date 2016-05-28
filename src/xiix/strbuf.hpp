#pragma once
#include"defines.hpp"
#include<string.h>
namespace xiix{class strbuf{
	char bb[strbuf_size];
//	const size_t s{strbuf_size};
	char*p{bb};
	char*b{bb};
	char*e{bb+strbuf_size};
public:
	inline void rst(){
		p=b=bb;
		e=bb+strbuf_size;
		*p=0;
	}
	inline size_t len()const{return p-b;}
	inline void append(const char&ch){
		assert( p!=e );
//		if((size_t)(p-bb)==strbuf_size)throw"overflow";
		*p++=ch;
	}
	inline void copy_to(char*buf,size_t buflen){
		const size_t len=p-b;
		strncpy(buf,b,len);
	}
	inline void trimright(){
		while(1){
			if(p==b)return;
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
	inline void backspace(const char replacement_char){
		if(p==b)throw"underflow";
		p--;
		*p=replacement_char;
	}
};}
