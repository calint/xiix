#pragma once
namespace xiix{

class client{
public:
	client(const int argc,const char**argv,const char**env){
		auto p=argv;
		for(int i=0;i<argc;i++)
			puts(*p++);
	}
};

}
