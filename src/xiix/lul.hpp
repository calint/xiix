#pragma once
#include<stdlib.h>
#include<string.h>
#include<functional>
namespace xiix{template<class T>class lul final{
	private:
		unsigned int length{0};
		bool delete_keys{false};//. bitset
		bool delete_data{false};
		//-- --  - -- -- - --  --  - - - - - --           - -- - -- --- ---  - - - - - -
		class el{
		public:
			char*key{nullptr};
			T data{nullptr};
			el*nxt{nullptr};
			inline el(char*key,T data):key{key},data{data}{}
		};
		//-- --  - -- -- - --  --  - - - - - --           - -- - -- --- ---  - - - - - -
		el*fel{nullptr};
	public:
		inline lul(const bool del_data,const bool and_del_keys):delete_keys(and_del_keys),delete_data(del_data){}
		inline~lul(){clear();}
		inline T operator[](const char*key){
			el*e=fel;
			while(true){
				if(!e)break;
				if(!strcmp(e->key,key))return e->data;
				e=e->nxt;
			}
			return nullptr;
		}
		inline void put(char*key,T data,bool allow_overwrite=true){
			if(fel==nullptr){
				fel=new el(key,data);
				return;
			}
			el*e=fel;
			while(true){
				if(!e)break;
				if(!strcmp(e->key,key)){
					if(!allow_overwrite)throw"overwrite";
					if(delete_data)delete e->data;
					e->data=data;
					return;
				}
				if(!e->nxt){
					e->nxt=new el(key,data);
					return;
				}
				e=e->nxt;
			}
			throw"illegalstate";
		}
		inline void foreach(const std::function<bool(const char*,T)>&func){
			el*e=fel;
			while(true){
				if(!e)return;
				if(!f(e->data))return;
				e=e->nxt;
			}
		}
		inline void clear(){
			while(true){
				if(!fel)return;
				el*nxt=fel->nxt;
				if(delete_keys)free(fel->key);//?unrolltofourcases?
				if(delete_data)free((void*)fel->data);//? crash
				delete fel;
				fel=nxt;
			}
		}
	};
}
