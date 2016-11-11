#include "server_server.h"
#include "../interfaz/client_json_reader.h"
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

void Server::bind(int port){
	this -> proxyClient.bindAndListen(port);
}

void Server::listen(){
	ProxyClient proxy; 
	proxy = proxyClient.aceptarCliente();
	std::cout << proxy.recibirCodigoMensaje(1) << std::endl;
	while (true){
		// me solicitan los slots de lobby
		try {
			std::cout << proxy.recibirCodigoMensaje(1) << std::endl;

			uint32_t tamMensaje = proxy.recibirTamMensaje(4);

			std::string nombreObjeto = proxy.recibir(tamMensaje);

			std::cout << nombreObjeto << std::endl;

			/*** SIMULACION PEDIDO DE SLOTS DE LOBBY(CLIENTE)***/
			string string_to_send = get_slots(nombreObjeto);

			proxy.enviarSlots(string_to_send);

			/***DECODIFICACION DEL JSON (CLIENTE)***/
			std::vector<InterfaceSlot*> i_slots;
			JsonReader slots_reader;
			slots_reader.read(i_slots, string_to_send);

			/***VEO SI LA LECTURA FUE CORRECTA***/
			int size = i_slots.size();
			for (int i = 0; i < size ; i++)
				i_slots[i] -> print_attr();

			for (std::vector<InterfaceSlot*>::iterator it = i_slots.begin(); it != i_slots.end();){  
				delete* it;  
				it = i_slots.erase(it);
			}
		} catch (const std::exception e){ 

		}
	}

}

int Server::execute_file(string file_name){
	std::ifstream file;
	file.open(file_name, std::ifstream::in);
	if (!file.is_open())
		return 1;
	
	std::string str;
	std::string file_contents;
	while (std::getline(file, str)){
		file_contents += str;
		file_contents.push_back('\n');
	}  
	//Call Parser with file_contents
	return 0;
}

Server::~Server(){
	delete this -> vm;
}
