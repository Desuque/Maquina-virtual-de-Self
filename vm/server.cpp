#include "server.h"

Server::Server(){
	this -> vm = new VM();
}

void Server::recv_messages(){
}

string Server::get_slots(string object){
	//Slot* obj = call parser to get object
	Slot* obj = vm->search_obj("lobby");
	string obj_str = vm->get_slots(obj); 
	return obj_str;
}

string Server::save_vm(){
	return "save";
}

string Server::execute(string context, string code){
	return "call parser to execute code";
}

VM* Server::get_vm(){
	return this -> vm;
}

Server::~Server(){
	delete this -> vm;
}
