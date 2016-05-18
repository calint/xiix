#include<stdio.h>
#include"xiix.hpp"
int main(const int argc,const char**argv,const char**env){
	puts("xiix - http client");
	xiix::client(argc,argv,env);
	return 0;
}



