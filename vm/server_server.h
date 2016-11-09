#ifndef __server_server_h__
#define __server_server_h__

#include <vector>
#include <string>
#include "server_virtual_machine.h"

class Server{
	private:
		VM* vm;
		//Parser parser;
	public:
		Server();
		void recv_messages();
		VM* get_vm();
		string get_slots(string id);
		string save_vm();
		~Server();
		
	private:
		string execute(string context, string code);
		
};

#endif
