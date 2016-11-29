#include "server_app.h"
#include "../interfaz/client_json_reader.h"
#include "server_json_writer.h"
#include "server_server.h"
#include <iostream>
#include <cstdlib>
#include <fstream>

static const int cod_get_slots = 2;
static const int cod_generic = 5;
static const int cod_update_position = 8;
static const int cod_pedir_morph = 11;
static const int cod_borrar_morph = 12;
static const int cod_share_obj = 13;
static const int cod_garbage = 14;
static const int cod_pedir_lista_lobbys = 15;
static const int cod_get_it = 16;
static const int cod_do_it = 17;
static string user = getenv("USER"); 
static string dir = "/home/"+user+"/Documents/data/";
static string ext = ".dat";
static string lobby_id = "0";

App::App(){
	parser.setVM(&vm);
}

App::App(string name, Server* server, ProxyClient* proxy){
	parser.setVM(&vm);
        this -> proxy = proxy;
        this -> server = server;
        this -> name = name;
}

string App::get_slots(string id){
	Slot* obj = vm.search_obj_id(std::stoi(id));
	string obj_str = vm.get_slots(std::stoi(id),obj); 
	return obj_str;
}

string App::save_vm(string name){
        string user = getenv("USER"); 
        string file_name = dir+name+ext;
        std::ofstream file;
        string rm_msg = "rm -f " + file_name; 
        system(rm_msg.c_str());
        string cmd = "mkdir -p "+dir; 
        system(cmd.c_str());
	file.open (file_name, std::ios::app);
	string data = vm.save();
	file << data << std::endl;
	file.close();
	return data;
}

strings App::execute(string msg, ints& flags){
	JsonReader read;
	string id, code;
	read.read_code(msg, id, code);
	string json = "";
        
	std::vector<Slot*> res = parser.parsear(code, id, flags);
        strings jsons;
        
        int size = res.size();
        for (int i = 0; i < size; i++){
                jsons.push_back(vm.get_slots(std::stoi(id), res[i]));
        }
	
	return jsons;
}

VM* App::get_vm(){
	return &vm;
}

void App::run(int* fin){
	while (true){
                try {    
			uint32_t codigoMensaje = proxy->recibirCodigoMensaje();
			switch (codigoMensaje){
				case cod_update_position:
                                        rcv_upd_pos();
					break;
				case cod_get_slots:
                                        rcv_msg_get_slots();
					break;
				case cod_generic:
                                        rcv_msg_generic();
                                        save_vm(this->name);
					break;
				case cod_share_obj:
					rcv_share_obj();
					break;
				case cod_pedir_lista_lobbys:
                                        rcv_pedir_lista();
					break;
				case cod_pedir_morph:
					rcv_pedir_morph();
                                        break;
				case cod_borrar_morph:
                                        rcv_borrar_morph();
					break;
				default:
					std::cout << "error en default switch ejconconexion" << std::endl;
					std::cout << "recibio: " << codigoMensaje << std::endl;

					break;
			}
		} catch (const std::exception e){ break;}
	}
 	*fin = 1;
}

void App::rcv_borrar_morph(){
        string json = proxy->recibirJson();
        server -> update_lobby_data(this, cod_borrar_morph, json, 0, 0 );
}

void App::rcv_pedir_morph(){
        string json = proxy->recibirJson();
        server -> update_lobby_data(this, cod_pedir_morph, json, 0, 0);
}

void App::rcv_pedir_lista(){
        proxy->enviarCodigoMensaje(cod_pedir_lista_lobbys);
        string json = server->get_json_apps_name();
        proxy->enviarJson(json);
}

void App::rcv_upd_pos(){
        string json = proxy->recibirJson();
        server -> update_lobby_data(this,cod_update_position , json, 0, 0);
}

void App::rcv_msg_get_slots(){
        std::string nombreObjeto = proxy->recibirJson();
        string string_to_send = get_slots(nombreObjeto);
        server -> update_lobby_data(this, cod_get_slots, string_to_send, 0, 0);
}

void App::rcv_msg_generic(){
        int accion = proxy->recibirCodigoMensaje();
        string nombreObjeto = proxy->recibirJson();
		
        string codigoAEjecutar = proxy->recibirJson();
        ints flags;
        strings results = execute(codigoAEjecutar, flags);
        int flag = parser.getFlag();
                
        int size = results.size();
        for (int c = 0; c < size; c++)
            server -> update_lobby_data(this, cod_generic, results[c], flags[c],  accion);
}

void App::rcv_share_obj(){
        std::string data = proxy->recibirJson();
        int id;
        string lobby_des;
        JsonReader reader;
        reader.read_share_data(data, id, lobby_des);
        string str_parser_share = vm.get_slot_to_share(id);
        string json_share = vm.get_json_id(id);
        server -> share_obj_to(str_parser_share, json_share, lobby_des);
}

void App::update_app(string str_parser_code_share){
        ints flags;
        parser.parsear(str_parser_code_share, lobby_id, flags);
}

ProxyClient* App::get_proxy(){
        return proxy;
}

int App::execute_file(string code){
        ints flags;
        parser.parsear(code, lobby_id, flags);	
        return 0;
}

string App::get_name(){
        return name;
}

App::~App(){
}
