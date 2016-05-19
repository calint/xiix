#pragma once
#include"args.hpp"
#include"meters.hpp"
#include"conf.hpp"
#include"sock.hpp"
#include<stdlib.h>
namespace xiix{class client final{
public:
	inline client(const int argc,const char**argv,const char**env){
		try{
			main(argc,argv);
		}catch(const char*msg){
			puts(msg);
		}catch(...){
			puts("exception caught");
		}
	}
private:
	inline int main(const int argc,const char**argv){
		if(argc<5){
			fprintf(stderr,"usage: %s -p [host=localhost] [port=8088] [uri=/] [connections=1]\n",argv[0]);
			return-1;
		}
		const char*hostname=argv[1];
		const int port=atoi(argv[2]);
		const char*uri=argv[3];
		const int nsocks=atoi(argv[4]);
		printf("\n   x i i x   m a r k\n\n");
		printf("      clients: %d\n",nsocks);
		if(port!=80)printf("          url: http://%s:%d%s\n",hostname,port,uri);
		else        printf("          url: http://%s%s\n",hostname,uri);

		args ag{argc,argv};
		conf::print_traffic=ag.hasoption('p');
		conf::print_stats=ag.hasoption('s');
		const int epfd=epoll_create(nsocks);
		if(epfd<0)throw"epoll_create";

		sock*socks[nsocks];
		for(auto&s:socks)s=new sock(epfd,hostname,port);
		for(auto s:socks)s->open();
		struct epoll_event events[nsocks];
		while(1){
			const int n=epoll_wait(epfd,events,nsocks,-1);
			for(int i=0;i<n;++i){
				meters::events++;
				struct epoll_event&ev=events[i];
				sock*sk=(sock*)ev.data.ptr;
				sk->on_epoll_event(ev);
				if(conf::print_stats)printf("   requests: %zu  events: %zu  writes: %zu    reads: %zu \r",meters::requests,meters::events,meters::writes,meters::reads);
			}
		}
	}
};}
