#include<stdio.h>
#include"xiix.hpp"
int main(int argc,char**argv,char**env){
	puts("xiix - http client");
	xiix::client(argc,argv,env);
	return 0;
}



