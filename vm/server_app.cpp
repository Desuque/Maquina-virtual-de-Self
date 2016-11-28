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
        string file_name = "data/"+name+".dat";
        std::ofstream file;
        string rm_msg = "rm -f " + file_name; 
        system(rm_msg.c_str());
        system("mkdir -p data");
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
        //test_example();
	while (true){
                try {    
			uint32_t codigoMensaje = proxy->recibirCodigoMensaje();
			switch (codigoMensaje){
				case cod_update_position: {
					//uint32_t tamMensaje = proxy->recibirTamMensaje(4);
					//std::string json = proxy->recibir(tamMensaje);
					std::string json = proxy->recibirJson();
					server -> update_lobby_data(this,cod_update_position , json, 0, 0);
					break;
				}
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
				case cod_pedir_lista_lobbys:{
					proxy->enviarCodigoMensaje(cod_pedir_lista_lobbys);
					string json = server->get_json_apps_name();
					proxy->enviarJson(json);
					break;
				}
				case cod_pedir_morph :{
					//uint32_t tamMensaje = proxy->recibirTamMensaje(4);
					//std::string json = proxy->recibir(tamMensaje);
					std::string json = proxy->recibirJson();
                    server -> update_lobby_data(this, cod_pedir_morph, json, 0, 0);
					/*proxy->enviar(cod_pedir_morph,1);
					proxy->enviarJson(json);*/
					break;
				}
				case cod_borrar_morph :{
					//uint32_t tamMensaje = proxy->recibirTamMensaje(4);
					//std::string json = proxy->recibir(tamMensaje);
					std::string json = proxy->recibirJson();
                    server -> update_lobby_data(this, cod_borrar_morph, json, 0, 0 );
					/*proxy->enviar(cod_pedir_morph,1);
					proxy->enviarJson(json);*/
					break;
				}
				default:
					std::cout << "error en default switch ejconconexion" << std::endl;
					std::cout << "recibio: " << codigoMensaje << std::endl;

					break;
			}
		} catch (const std::exception e){ break;}
	}
	/*Slot* gar =  vm.collect();
        string json = vm.get_slots(gar);
        // LA DECODIFICACION DEBE IR EN EL CLIENTE
        JsonReader reader;
        v_ids vec;
        reader.read_garbage_ids(json, vec);
        int size = vec.size();
        for (int i = 0; i< size; i++)
        std::cout << vec[i] << std::endl;*/
 	*fin = 1;
}

void App::rcv_msg_get_slots(){
        //uint32_t tamMensaje = proxy->recibirTamMensaje(4);
        std::string nombreObjeto = proxy->recibirJson();
        string string_to_send = get_slots(nombreObjeto);
        server -> update_lobby_data(this, cod_get_slots, string_to_send, 0, 0);
}

void App::rcv_msg_generic(){
		int accion = proxy->recibirCodigoMensaje();
		//uint32_t tamMensaje = proxy->recibirTamMensaje(4);
		//std::string nombreObjeto = proxy->recibir(tamMensaje);
		std::string nombreObjeto = proxy->recibirJson();
		
		//tamMensaje = proxy->recibirTamMensaje(4);
		//std::string codigoAEjecutar = proxy->recibir(tamMensaje);
		std::string codigoAEjecutar = proxy->recibirJson();
                ints flags;
		strings results = execute(codigoAEjecutar, flags);
                int flag = parser.getFlag();
                
                int size = results.size();
                for (int c = 0; c < size; c++)
                        server -> update_lobby_data(this, cod_generic, results[c], flags[c],  accion);
}

void App::rcv_share_obj(){
        //uint32_t tamMensaje = proxy->recibirTamMensaje(4);
        //std::string data = proxy->recibir(tamMensaje);
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
        parser.parsear(str_parser_code_share, "0", flags);
}

ProxyClient* App::get_proxy(){
        return proxy;
}

int App::execute_file(string code){
        ints flags;
        parser.parsear(code, "0", flags);	
        return 0;
}

string App::get_name(){
        return name;
}

App::~App(){
}

void App::test_example(){
	//vm.checkpoint();
	
	//lobby _AddSlots: (| y <- 8. |).
	Slot* X0 = vm.search_obj("lobby");
        Slot* cont = vm.create_object();
	Slot* X1 = vm.create_object();
	Slot* X2 = vm.create_int(8);
	vm.immutable_object(X2);
	vm.add_slot(X1, "y", X2);
        vm.add_slot(cont, "", X1);
        vm.keyword_message(X0, "_AddSlots", cont);
	
        vm.collect();
      /*
        //y print.
	Slot* X4 = vm.search_obj("y");
	vm.unary_message(X4, "print");*/
	
	//lobby _AddSlots: (| punto1 = (| y <- 4. |). |).
	Slot* X6 = vm.search_obj("lobby");
        Slot* cont1 = vm.create_object();
	Slot* X7 = vm.create_object();
	Slot* X8 = vm.create_object();
	Slot* X9 = vm.create_int(4);
	vm.add_slot(X8, "y", X9);
	vm.add_slot(X7, "punto1", X8);
        vm.add_slot(cont1, "", X7);
	vm.keyword_message(X6, "_AddSlots", cont1);
	/*
	//(punto1 y) print.
	Slot* X10 = vm.search_obj("punto1");
	Slot* X11 = vm.unary_message(X10, "y");
	vm.unary_message(X11, "print");*/
	/*
	//((punto1 y) + y) print.
	Slot* X12 = vm.search_obj("punto1");
	Slot* X13 = vm.unary_message(X12, "y");
	Slot* X14 = vm.search_obj("y");
	Slot* X15 = vm.binary_message(X13,"+",X14);
	vm.unary_message(X15, "print");*/
	/*
	//((punto1 y) != y) print.
	Slot* X16 = vm.search_obj("punto1");
	Slot* X17 = vm.unary_message(X16, "y");
	Slot* X18 = vm.search_obj("y");
	Slot* X19 = vm.binary_message(X17,"!=",X18);
	vm.unary_message(X19, "print");*/
	//vm.collect();
        
	//lobby _AddSlots: (| str = 'hello'. |).
	Slot* X20 = vm.search_obj("lobby");
	Slot* cont2 = vm.create_object();
        Slot* X21 = vm.create_object();
	Slot* X22 = vm.create_string("hello");
	vm.add_slot(X21, "str", X22);
	vm.add_slot(cont2,"",X21);
        vm.keyword_message(X20, "_AddSlots", cont2);
	/*
	//str print.
	Slot* X23 = vm.search_obj("str");
	vm.unary_message(X23, "print");*/
	
	
	//lobby _AddSlots: (| punto11 = (| x<-5. padre* = punto1 |)).
	Slot* X24 = vm.search_obj("lobby");
	Slot* cont3 = vm.create_object();
        Slot* X25 = vm.create_object();
	Slot* X26 = vm.search_obj("punto1");
	Slot* X27 = vm.create_object();
	Slot* X28 = vm.create_int(5);
	vm.add_slot(X27, "x", X28);
	vm.add_parent(X27, "padre", X26);
	vm.add_slot(X25, "punto11", X27);
        vm.add_slot(cont3,"",X25);
	vm.keyword_message(X24, "_AddSlots", cont3);
	/*
	//(punto11 y) print.
	Slot* X29 = vm.search_obj("punto11");
	Slot* X30 = vm.unary_message(X29, "y");
	vm.unary_message(X30, "print");*/
	//vm.collect();
	//lobby _AddSlots: (| punto = (| square_norm = (| | ((x*x)+(y*y))).
	//				 print = (| | print x.).
	//				 y <- 7.	    
	//			      |). 
	//		    |).
	
	Slot* X31 = vm.search_obj("lobby");
	Slot* cont4 = vm.create_object();
        Slot* X32 = vm.create_object();
	Slot* X33 = vm.create_object();
	//Slot* X34 = vm.create_string("( y * y ).");
	Slot* X34 = vm.create_string("(( y * y ) + ( x * x )).");
        Slot* X35 = vm.create_string("print x");
	//Slot* X3Y = vm.create_int(7);
	//vm.immutable_object(X3Y);
	vm.add_code(X33, "print", X35);
	vm.add_code(X33, "square_norm", X34);
	//vm.add_slot(X33, "y", X3Y);
	vm.add_slot(X32, "punto", X33);
        vm.add_slot(cont4,"",X32);
	vm.keyword_message(X31, "_AddSlots", cont4);
	
	//punto print.   #Print Method Object
	/*Slot* X36 = vm.search_obj("punto");
	vm.unary_message(X36, "print");*/
	
	Slot* X37 = vm.search_obj("punto1");
	Slot* X38 = vm.search_obj("punto");
	vm.add_parent(X37, "padre", X38);
	
	//punto11 square_norm.   #Print Method Object
	Slot* X39 = vm.search_obj("punto11");
	Slot* a = vm.unary_message(X39, "square_norm");
        std::cout << "SQUARE_NORM " << vm.get_slots(0, a) << std::endl;
	/*
        //(punto y) print.
	Slot* X40 = vm.search_obj("punto");
	Slot* X41 = vm.unary_message(X40, "y");
	vm.unary_message(X41, "print");*/
	/*
        //(punto1 y) print.
	Slot* X42 = vm.search_obj("punto1");
	Slot* X43 = vm.unary_message(X42, "y");
	vm.unary_message(X43, "print");
	*/
	Slot* X44 = vm.search_obj("lobby");
	vm.rm_slot(X44, "y");
	
        //lobby _AddSlots: (| prueba1 = 'hello'. prueba2 = 'hello2'. |).
	Slot* X90 = vm.search_obj("lobby");
	Slot* X91 = vm.create_object();
        Slot* X92 = vm.create_object();
	Slot* X93 = vm.create_string("hello");
	Slot* X94 = vm.create_object();
	Slot* X95 = vm.create_string("hello2");
        vm.add_slot(X92, "prueba1", X93);
        vm.add_slot(X94, "prueba2", X95);
        vm.add_slot(X91, "", X92);
	vm.add_slot(X91, "", X94);
        vm.keyword_message(X90, "_AddSlots", X91);
    
        //lobby _RemoveSlots: (| prueba1. prueba2. |).
	Slot* X96 = vm.search_obj("lobby");
	Slot* X97 = vm.create_object();
        Slot* X98 = vm.create_object();
	Slot* X99 = vm.create_string("prueba1");
	Slot* X100 = vm.create_object();
	Slot* X101 = vm.create_string("prueba2");
        vm.add_slot(X98, "prueba1", X99);
        vm.add_slot(X100, "prueba2", X101);
        vm.add_slot(X97, "", X98);
	vm.add_slot(X97, "", X100);
        vm.keyword_message(X96, "_RemoveSlots", X97);
        
        //lobby _AddSlots: (| funciones = (| multa:b: = (| :arg. :arg1. | arg * arg1.). |). |).
        Slot* XA1 = vm.search_obj("lobby");
	Slot* XAc = vm.create_object();
        Slot* XA2 = vm.create_object();
	Slot* XA3 = vm.create_object();
	Slot* XA4 = vm.create_string("( arg * arg1 ).");
	Slot* XA5 = vm.create_object();
        Slot* XA6 = vm.create_object();//
        vm.add_argument(XA4, "arg1", XA6);//
        vm.add_argument(XA4, "arg", XA5);
        vm.add_code(XA3, "multa:b:", XA4);
        vm.add_slot(XA2, "funciones", XA3);
        vm.add_slot(XAc,"",XA2);
	vm.keyword_message(XA1, "_AddSlots", XAc);
        
        /*std::cout << "XA2 " << vm.get_slots(0, XA2) << std::endl;
        std::cout << "XA3 " << vm.get_slots(0, XA3) << std::endl;
        std::cout << "XA4 " << vm.get_slots(0, XA4) << std::endl;*/
       
        //funciones multa:9 b:2.
        Slot* XE1 = vm.search_obj("funciones");
        Slot* XE2 = vm.create_object();
        Slot* XE3 = vm.create_int(9);
        Slot* XE4 = vm.create_int(2);//
        vm.add_parent(XE2, "2", XE4);//
        vm.add_parent(XE2, "9", XE3);
        Slot* B = vm.keyword_message(XE1, "multa:b:", XE2);
        std::cout << "KEY " << vm.get_slots(0, B) << std::endl;

        //funciones multa:8 b:3.
        Slot* XE11 = vm.search_obj("funciones");
        Slot* XE12 = vm.create_object();
        Slot* XE13 = vm.create_int(8);
        Slot* XE14 = vm.create_int(3);//
        vm.add_parent(XE12, "3", XE14);//
        vm.add_parent(XE12, "8", XE13);
        Slot* C = vm.keyword_message(XE11, "multa:b:", XE12);
        std::cout << "KEY " << vm.get_slots(0, C) << std::endl;
        
        //lobby _AddSlots: (| funciones_str = (| printa: = (| :arg. | arg print.). |). |).
        Slot* XA21 = vm.search_obj("lobby");
	Slot* XA2c = vm.create_object();
        Slot* XA22 = vm.create_object();
	Slot* XA23 = vm.create_object();
	Slot* XA24 = vm.create_string("arg print.");
	Slot* XA25 = vm.create_object();
        vm.add_argument(XA24, "arg", XA25);
        vm.add_code(XA23, "printa:", XA24);
        vm.add_slot(XA22, "funciones_str", XA23);
        vm.add_slot(XA2c,"",XA22);
	vm.keyword_message(XA21, "_AddSlots", XA2c);
        
        //funciones_str printa: 'prueba'.
        Slot* XE21 = vm.search_obj("funciones_str");
        Slot* XE22 = vm.create_object();
        Slot* XE23 = vm.create_string("prueba");
        vm.add_parent(XE22, "prueba", XE23);
        Slot* D = vm.keyword_message(XE21, "printa:", XE22);
        std::cout << "KEY " << vm.get_slots(0, D) << std::endl;
        
        //Slot* l = vm.search_obj_by_name("lobby",0);
        //std::cout << "LOBBY " << vm.get_slots(0,l) << std::endl;
        //Slot* clone = vm.clone_obj_by_name("punto", 0);
        //std::cout << "CLONE " << vm.get_slots(0,clone) << std::endl;
        //std::cout << "CLONE GET" << get_slots("1337") << std::endl;
        /*
        Slot* y = vm.search_obj_by_name("y",116);
        vm.unary_message(y, "print");
	
        
        vm.search_obj_by_name("y",494);
	*/
	//vm.revert();
        vm.collect();
}