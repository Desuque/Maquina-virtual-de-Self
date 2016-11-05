#ifndef __server_h__
#define __derver_h__

#include <vector>
#include <string>
#include "virtual_machine.h"

class Server{
	private:
		VM* vm;
		//Parser parser;
	public:
		Server();
		void recv_messages();
		VM* get_vm();
		string get_slots(string context);
		~Server();
		
	private:
		string execute(string context, string code);
		string save_vm();
};

#endif
