#pragma once
#include"args.hpp"
#include"meters.hpp"
#include"conf.hpp"
#include"sock.hpp"
#include"lst.hpp"
#include<stdlib.h>
#include<memory.h>
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
		args ag{argc,argv};
		if(ag.hasoption('h')){
			fprintf(stderr,"usage: %s [host=localhost] [port=8088] [uri=/] [connections=1] -psqv\n",argv[0]);
			return 0;
		}
		const char*hostname=ag.getarg(1,"localhost");
		const int port=atoi(ag.getarg(2,"80"));
		const char*uri=ag.getarg(3,"/");
		const int nsocks=atoi(ag.getarg(4,"1"));
		conf::print_traffic=ag.hasoption('p');
		conf::print_stats=ag.hasoption('s');
		if(ag.hasoption('q'))conf::print_content=false;
		if(ag.hasoption('v')){
			printf("\n   x i i x   m a r k\n\n");
			printf("      clients: %d\n",nsocks);
			if(port!=80)printf("          url: http://%s:%d%s\n",hostname,port,uri);
			else        printf("          url: http://%s%s\n",hostname,uri);
		}
		const int epollfd=epoll_create(nsocks);
		if(epollfd<0)throw"epoll_create";

		lst<sock*>socks;
		for(int i=0;i<nsocks;i++)socks.add(new sock(epollfd,hostname,port));
		const bool repeatmode=ag.hasoption('r');
		socks.foreach2([repeatmode,uri](sock*s){s->seturi(uri).setrepeatmode(repeatmode).connect();return true;});
		struct epoll_event evs[nsocks];
		while(1){
			const int n=epoll_wait(epollfd,evs,nsocks,-1);
			for(int i=0;i<n;++i){
				meters::events++;
				struct epoll_event&ev=evs[i];
				sock*sk=(sock*)ev.data.ptr;
				try{
					sk->on_epoll_event(ev);
				}catch(const char*msg){
					if(!strcmp("close",msg)||!strcmp("responsecode",msg)){
						delete sk;
						if(meters::socks==0)
							return 0;
					}
					puts("*** exception caught");
					puts(msg);
				}catch(...){
					puts("*** exception caught");
					return-1;
				}
				if(conf::print_stats)printf("   requests: %zu  events: %zu  writes: %zu    reads: %zu \r",meters::requests,meters::events,meters::writes,meters::reads);
			}
		}
	}
};}
