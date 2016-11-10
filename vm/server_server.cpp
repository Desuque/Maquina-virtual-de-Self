#include "server_server.h"
#include "client_json_reader.h"
#include <iostream>
#include <fstream>

Server::Server(){
	this -> vm = new VM();
}

void Server::recv_messages(){
}

string Server::get_slots(string id){
	//Slot* obj = call parser to get object
	//Slot* obj = vm->search_obj(object);
	Slot* obj = vm->search_obj_id(std::stoi(id));
	string obj_str = vm->get_slots(obj); 
	return obj_str;
}

string Server::save_vm(){
	std::ofstream file;
	file.open ("vm.dat");
	string data = vm -> save();
	file << data << std::endl;
	file.close();
	return data;
}

string Server::execute(string msg){
	JsonReader read;
	string id, code;
	read.read_code(msg, id, code);
	
	/*****************SIMULACION******************/
	Slot* res = NULL;
	string pat = "_AddSlots:";
	std::size_t found_add = code.find(pat);
	if (found_add != std::string::npos){
		Slot* X0 = vm->search_obj_id(std::stoi(id));
		Slot* X1 = vm->create_object();
		Slot* X2 = vm->create_int(8);
		vm->add_slot(X1, "p", X2);
		res = vm->keyword_message(X0, "_AddSlots:", X1);
	}else{
		Slot* X0 = vm->create_object();
		Slot* X1 = vm->create_string("hello self!");
		vm->add_slot(X0, "hello self!", X1);
		res = vm->unary_message(X0, "print");
	}
	/*******************************************/
	
	string json = vm->get_slots(res);
	return json;
}

VM* Server::get_vm(){
	return this -> vm;
}

Server::~Server(){
	delete this -> vm;
}
