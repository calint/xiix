#pragma once
#include<stdlib.h>
namespace xiix{
	const size_t K=1024;
	const size_t sock_buf_size=4*K;
	const size_t strbuf_size=256;

	const char*default_host="localhost";
	const char*default_port="80";
	const char*default_path="/";
	const char*default_nclients="1";

	namespace meters{
		size_t socks{0};
		size_t events{0};
		size_t input{0};
		size_t output{0};
		size_t requests{0};
		size_t opens{0};
		size_t reads{0};
		size_t writes{0};
		size_t brks{0};
		size_t errors{0};
	}

	namespace conf{
		bool print_traffic{false};
		bool print_stats{false};
		bool print_content{true};
	}

	const char*signal_connreset="brk";
	const char*signal_close="close";
}
