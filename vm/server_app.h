#ifndef __server_app_h__
#define __server_app_h__

#include <vector>
#include <string>
#include "server_virtual_machine.h"
#include "common_thread.h"
#include "common_proxyClient.h"
#include "server_parser.h"

typedef std::vector<string> strings;
typedef std::vector<int> ints;

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
		strings execute(string msg, ints& flags);
                string get_name();
		int execute_file(string file_name);
                void update_app(string str_parser_code_share);
                void run(int* fin);
                ProxyClient* get_proxy();
		~App();

	private:
                void rcv_borrar_morph();
                void rcv_pedir_morph();
                void rcv_pedir_lista();
                void rcv_upd_pos();
                void rcv_msg_get_slots();
                void rcv_msg_generic();
		void rcv_share_obj();
		void test_example();
};

#endif
