#ifndef __server_server_h__
#define __server_server_h__

#include <vector>
#include <string>
#include "server_app.h"
#include "common_thread.h"
#include "../interfaz/common_proxyClient.h"
#include <dirent.h>

typedef std::string string;
typedef std::multimap<string, App*> map_apps;

class Server : public Thread{ 
	private:
		map_apps apps;
                ProxyClient proxyClient;

	public:
		Server();
		void bind(int port);
                void listen();
                void run(int* fin);
                void shutdown();
                int execute(string file_name);
                void update_lobby_data(App* or_app, string json);
                
	private:
		int execute_file(App* app, string file_name);
                void load_file_names();
                string get_json_apps_name();
		void join_threads();
};

#endif
