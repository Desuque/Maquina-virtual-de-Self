#ifndef __server_server_h__
#define __server_server_h__

#include <vector>
#include <string>
#include "server_app.h"
#include "common_thread.h"
#include "../interfaz/common_proxyClient.h"

typedef std::map<int, App*> map_apps;

class Server : public Thread{ 
	private:
		map_apps apps;
                ProxyClient proxyClient;
		
	public:
		void bind(int port);
                int execute_file(string file_name);
		void listen();
                void run(int* fin);
                void shutdown();

	private:
		void join_threads();
};

#endif
