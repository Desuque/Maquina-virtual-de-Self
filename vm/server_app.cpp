#include "server_app.h"
#include "../interfaz/client_json_reader.h"
#include "server_json_writer.h"
#include <iostream>
#include <cstdlib>
#include <fstream>

App::App(){
        this -> proxy = NULL;
	parser.setVM(&vm);
}

App::App(ProxyClient* proxy){
	parser.setVM(&vm);
        this -> proxy = proxy;
}

string App::get_slots(string id){
	//Slot* obj = call parser to get object
	//Slot* obj = vm.search_obj(object);
	Slot* obj = vm.search_obj_id(std::stoi(id));
	string obj_str = vm.get_slots(obj); 
	return obj_str;
}

string App::save_vm(){
	std::ofstream file;
        system("mkdir -p data");
	file.open ("data/vm.dat");
	string data = vm.save();
	file << data << std::endl;
	file.close();
	return data;
}

string App::execute(string msg){
	JsonReader read;
	string id, code;
	read.read_code(msg, id, code);
	string json = "";

	Slot* res = NULL;
	res = parser.parsear(code, id);
	json = vm.get_slots(res);
	
	return json;
}

VM* App::get_vm(){
	return &vm;
}

void App::run(int* fin){
        test_example();
	while (true){
		// me solicitan los slots de lobby
		try {
			uint32_t codigoMensaje = proxy->recibirCodigoMensaje(1);
			switch (codigoMensaje){
				case 2:{
					uint32_t tamMensaje = proxy->recibirTamMensaje(4);

					std::string nombreObjeto = proxy->recibir(tamMensaje);

					std::cout << nombreObjeto << std::endl;

					string string_to_send = get_slots(nombreObjeto);

					proxy->enviarSlots(string_to_send);

					std::vector<InterfaceSlot*> i_slots;
					JsonReader slots_reader;
					slots_reader.read(i_slots, string_to_send);

					int size = i_slots.size();
					for (int i = 0; i < size ; i++)
						i_slots[i] -> print_attr();

					for (std::vector<InterfaceSlot*>::iterator it = i_slots.begin(); it != i_slots.end();){  
						delete* it;  
						it = i_slots.erase(it);
					}	
					break;
				}
				case 5:{
					uint32_t tamMensaje = proxy->recibirTamMensaje(4);

					std::string nombreObjeto = proxy->recibir(tamMensaje);

					std::cout << "id del objeto: " << nombreObjeto << std::endl;
					
					tamMensaje = proxy->recibirTamMensaje(4);
					std::string codigoAEjecutar = proxy->recibir(tamMensaje);
					std::cout << "codigo A Ejecutar: " << codigoAEjecutar << std::endl;

					JsonWriter writer;
					string json = writer.write_code(nombreObjeto, codigoAEjecutar);

					string result = execute(json);

					// ver como hacer con esto, mando mensaje "generic" pero 
					// podria ser cualquier cosa, ejemplo : agregarSlot remove

					int flag = parser.getFlag();
					if (flag == 0) {
						proxy->enviar(flag, 1);
					} else if (flag == 3) {
				  		proxy->enviar(flag, 1);
					} else if (flag == 4) {
						proxy->enviar(flag, 1);
					} else {
						//hardcodeo el 5 porque devuelve -1 por defecto para tener un caso no seteado
						proxy->enviar(5, 1);
					}

					std::cout << "devolucion: " << result << std::endl;
					
					if(flag != 0) {
						proxy->enviarSlots(result);
					}
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
	*fin = 1;
}

int App::execute_file(string code){
        parser.parsear(code, "0");	
        return 0;
}

App::~App(){
        if (this -> proxy)
            delete this -> proxy;
}

void App::test_example(){
	vm.checkpoint();
	
	//lobby _AddSlots: (| y <- 8. |).
	Slot* X0 = vm.search_obj("lobby");
        Slot* cont = vm.create_object();
	Slot* X1 = vm.create_object();
	Slot* X2 = vm.create_int(8);
	vm.immutable_object(X2);
	vm.add_slot(X1, "y", X2);
        vm.add_slot(cont, "", X1);
	vm.keyword_message(X0, "_AddSlots:", cont);
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
	vm.keyword_message(X6, "_AddSlots:", cont1);
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
	vm.collect();
        
	//lobby _AddSlots: (| str = 'hello'. |).
	Slot* X20 = vm.search_obj("lobby");
	Slot* cont2 = vm.create_object();
        Slot* X21 = vm.create_object();
	Slot* X22 = vm.create_string("hello");
	vm.add_slot(X21, "str", X22);
	vm.add_slot(cont2,"",X21);
        vm.keyword_message(X20, "_AddSlots:", cont2);
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
	vm.keyword_message(X24, "_AddSlots:", cont3);
	/*
	//(punto11 y) print.
	Slot* X29 = vm.search_obj("punto11");
	Slot* X30 = vm.unary_message(X29, "y");
	vm.unary_message(X30, "print");*/
	vm.collect();
	//lobby _AddSlots: (| punto = (| square_norm = (| | ((x*x)+(y*y))).
	//				 print = (| | print x.).
	//				 y <- 7.	    
	//			      |). 
	//		    |).
	
	Slot* X31 = vm.search_obj("lobby");
	Slot* cont4 = vm.create_object();
        Slot* X32 = vm.create_object();
	Slot* X33 = vm.create_object();
	Slot* X34 = vm.create_string("((x*x)+(y*y))");
	Slot* X35 = vm.create_string("print x");
	Slot* X3Y = vm.create_int(7);
	vm.immutable_object(X3Y);
	vm.add_code(X33, "print", X35);
	vm.add_code(X33, "square_norm", X34);
	vm.add_slot(X33, "y", X3Y);
	vm.add_slot(X32, "punto", X33);
        vm.add_slot(cont4,"",X32);
	vm.keyword_message(X31, "_AddSlots:", cont4);
	/*
	//punto print.   #Print Method Object
	Slot* X36 = vm.search_obj("punto");
	vm.unary_message(X36, "print");
	*/
	Slot* X37 = vm.search_obj("punto1");
	Slot* X38 = vm.search_obj("punto");
	vm.add_parent(X37, "padre", X38);
	/*
	//punto1 square_norm.   #Print Method Object
	Slot* X39 = vm.search_obj("punto1");
	vm.unary_message(X39, "square_norm");*/
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
        vm.keyword_message(X96, "_RemoveSlots:", X97);
        /*
        Slot* y = vm.search_obj_by_name("y",116);
        vm.unary_message(y, "print");
	
        
        vm.search_obj_by_name("y",494);
	*/
	vm.revert();
        vm.collect();
}
