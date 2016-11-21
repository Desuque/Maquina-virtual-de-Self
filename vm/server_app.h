#ifndef __server_app_h__
#define __server_app_h__

#include <vector>
#include <string>
#include "server_virtual_machine.h"
#include "common_thread.h"
#include "../interfaz/common_proxyClient.h"
#include "server_parser.h"


class Server;
class App : public Thread{
	private:
		VM vm;
		Parser parser;
		Server* server;
		ProxyClient* proxy;
                string name;
		
	public:
                App();
		App(string name, Server* server, ProxyClient* proxy);
		VM* get_vm();
		string get_slots(string id);
		string save_vm(string name);
		string execute(string msg);
                string get_name();
		int execute_file(string file_name);
                void run(int* fin);
		void send_msg_get_slots(string json);
                ProxyClient* get_proxy();
		~App();

	private:
                void rcv_msg_get_slots();
                void rcv_msg_generic();
		void test_example();
};

#endif
