#ifndef __server_app_h__
#define __server_app_h__

#include <vector>
#include <string>
#include "server_virtual_machine.h"
#include "common_thread.h"
#include "../interfaz/common_proxyClient.h"
#include "server_parser.h"

class App : public Thread{
	private:
		VM vm;
		Parser parser;
		ProxyClient* proxy;
		
	public:
                App();
		App(ProxyClient* proxy);
		VM* get_vm();
		string get_slots(string id);
		string save_vm(string name);
		string execute(string msg);
		int execute_file(string file_name);
                void run(int* fin);
		~App();

	private:
		void test_example();
};

#endif
