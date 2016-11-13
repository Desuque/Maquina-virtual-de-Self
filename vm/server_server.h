#ifndef __server_server_h__
#define __server_server_h__

#include <vector>
#include <string>
#include "server_virtual_machine.h"
#include "../interfaz/common_proxyClient.h"
#include "Parser.h"

class Server{
	private:
		VM* vm;
		Parser parser;
		ProxyClient proxyClient;
		
	public:
		Server();
		void recv_messages();
		VM* get_vm();
		string get_slots(string id);
		string save_vm();
		string execute(string msg);
		int execute_file(string file_name);
		void bind(int port);
		void listen();
		~Server();
};

#endif
