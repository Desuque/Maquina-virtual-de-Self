#ifndef __server_server_h__
#define __server_server_h__

#include <vector>
#include <string>
#include "server_app.h"
#include "common_thread.h"
#include "../interfaz/common_proxyClient.h"
#include <dirent.h>

typedef std::string string;
typedef std::map<string, App*> map_apps;

class Server : public Thread{ 
	private:
		map_apps apps;
                ProxyClient proxyClient;

	public:
		Server();
		void bind(int port);
                int execute_file(string file_name);
		void listen();
                void run(int* fin);
                void shutdown();

	private:
		void load_file_names();
                string get_json_apps_name();
		void join_threads();
};

#endif
