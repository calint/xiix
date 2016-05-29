#pragma once
namespace xiix{class strbuf{
	char bb[256];
	const size_t s{sizeof(bb)};
	char*e{bb};
	char*b{bb};
	char*end{bb+s};
public:
	inline void rst(){
		e=b=bb;
		end=bb+s;
		*e=0;
	}
	inline size_t len()const{return e-b;}
	inline void append(const char&ch){
		if((size_t)(e-bb)==s)throw"overflow";
		*e++=ch;
	}
	inline void copy_to(char*buf,size_t buflen){
		const size_t len=e-b;
		strncpy(buf,b,len);
	}
	inline void trimright(){
		while(1){
			if(e==b)return;
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
	inline void backspace(const char replacement_char){
		if(e==b)throw"underflow";
		e--;
		*e=replacement_char;
	}
};}
