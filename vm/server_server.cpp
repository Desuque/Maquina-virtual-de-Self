#include "server_server.h"
#include "../interfaz/client_json_reader.h"
#include "server_json_writer.h"
#include <iostream>
#include <fstream>

Server::Server(){
	this -> vm = new VM();
	parser.setVM(vm);
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
	string json = "";

	parser.parsear(code);

	/*****************SIMULACION******************/
	Slot* res = NULL;
	
	if (code.find("_AddSlots:") != std::string::npos){
		Slot* X0 = vm->search_obj_id(std::stoi(id));
		Slot* cont = vm->create_object();
                Slot* X1 = vm->create_object();
		Slot* X2 = vm->create_int(8);
		vm->add_slot(X1, "p", X2);
		vm->add_slot(cont, "", X1);
                res = vm->keyword_message(X0, "_AddSlots:", cont);
		json = vm->get_slots(res);
	}else if (code.find("_RemoveSlots:") != std::string::npos){
		Slot* X0 = vm->search_obj_id(std::stoi(id));
		res = vm -> rm_slot(X0, "x");
		json = vm->get_slot(res);
	}else{
		Slot* X0 = vm->create_object();
		Slot* X1 = vm->create_string("hello self!");
		vm->add_slot(X0, "hello self!", X1);
		res = vm->unary_message(X0, "print");
		json = vm->get_slots(res);
	}
	/*******************************************/
	
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
			uint32_t codigoMensaje = proxy.recibirCodigoMensaje(1);
			switch (codigoMensaje){
				case 2:{
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
					break;
				}
				case 3:{
					uint32_t tamMensaje = proxy.recibirTamMensaje(4);

					std::string nombreObjeto = proxy.recibir(tamMensaje);

					std::cout << "id del objeto: " << nombreObjeto << std::endl;
					
					tamMensaje = proxy.recibirTamMensaje(4);
					std::string codigoAEjecutar = proxy.recibir(tamMensaje);
					std::cout << "codigo A Ejecutar: " << codigoAEjecutar << std::endl;

					JsonWriter writer;
					string json = writer.write_code(nombreObjeto, codigoAEjecutar);

					string result = execute(json);

					std::cout << "devolucion: " << result << std::endl;
					
					proxy.enviarSlots(result);

					break;
				}
				default:
					std::cout << "error en default switch ejconconexion" << std::endl;
					std::cout << "recibio: " << codigoMensaje << std::endl;

					break;
			}
				
		} catch (const std::exception e){ 
			break;
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
