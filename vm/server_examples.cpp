#include "server_virtual_machine.h"
#include "server_slot.h"
#include "client_interface_slot.h"
#include "client_json_reader.h"
#include "server_json_writer.h"
#include "server_server.h"
#include <iostream>

int main(int argc, char** argv){
	Server server;
	VM* vm = server.get_vm();
	vm->checkpoint();
	
	//lobby _AddSlots: (| y <- 8. |).
	Slot* X0 = vm->search_obj("lobby");
	Slot* X1 = vm->create_object();
	Slot* X2 = vm->create_int(8);
	vm->immutable_object(X2);
	vm->add_slot(X1, "y", X2);
	vm->keyword_message(X0, "_AddSlots:", X1);
	vm->collect();
	//y print.
	Slot* X4 = vm->search_obj("y");
	vm->unary_message(X4, "print");
	
	//lobby _AddSlots: (| punto1 = (| y <- 4. |). |).
	Slot* X6 = vm->search_obj("lobby");
	Slot* X7 = vm->create_object();
	Slot* X8 = vm->create_object();
	Slot* X9 = vm->create_int(4);
	vm->add_slot(X8, "y", X9);
	vm->add_slot(X7, "punto1", X8);
	vm->keyword_message(X6, "_AddSlots:", X7);
	
	//(punto1 y) print.
	Slot* X10 = vm->search_obj("punto1");
	Slot* X11 = vm->unary_message(X10, "y");
	vm->unary_message(X11, "print");
	
	//((punto1 y) + y) print.
	Slot* X12 = vm->search_obj("punto1");
	Slot* X13 = vm->unary_message(X12, "y");
	Slot* X14 = vm->search_obj("y");
	Slot* X15 = vm->binary_message(X13,"+",X14);
	vm->unary_message(X15, "print");
	
	//((punto1 y) != y) print.
	Slot* X16 = vm->search_obj("punto1");
	Slot* X17 = vm->unary_message(X16, "y");
	Slot* X18 = vm->search_obj("y");
	Slot* X19 = vm->binary_message(X17,"!=",X18);
	vm->unary_message(X19, "print");
	vm->collect();
	//lobby _AddSlots: (| str = 'hello'. |).
	Slot* X20 = vm->search_obj("lobby");
	Slot* X21 = vm->create_object();
	Slot* X22 = vm->create_string("hello");
	vm->add_slot(X21, "str", X22);
	vm->keyword_message(X20, "_AddSlots:", X21);
	
	//str print.
	Slot* X23 = vm->search_obj("str");
	vm->unary_message(X23, "print");
	
	
	//lobby _AddSlots: (| punto11 = (| x<-5. padre* = punto1 |)).
	Slot* X24 = vm->search_obj("lobby");
	Slot* X25 = vm->create_object();
	Slot* X26 = vm->search_obj("punto1");
	Slot* X27 = vm->create_object();
	Slot* X28 = vm->create_int(5);
	vm->add_slot(X27, "x", X28);
	vm->add_parent(X27, "padre", X26);
	vm->add_slot(X25, "punto11", X27);
	vm->keyword_message(X24, "_AddSlots:", X25);
	
	//(punto11 y) print.
	Slot* X29 = vm->search_obj("punto11");
	Slot* X30 = vm->unary_message(X29, "y");
	vm->unary_message(X30, "print");
	vm->collect();
	//lobby _AddSlots: (| punto = (| square_norm = (| | ((x*x)+(y*y))).
	//				 print = (| | print x.).
	//				 y <- 7.	    
	//			      |). 
	//		    |).
	
	Slot* X31 = vm->search_obj("lobby");
	Slot* X32 = vm->create_object();
	Slot* X33 = vm->create_object();
	Slot* X34 = vm->create_string("((x*x)+(y*y))");
	Slot* X35 = vm->create_string("print x");
	Slot* X3Y = vm->create_int(7);
	vm->immutable_object(X3Y);
	vm->add_code(X33, "print", X35);
	vm->add_code(X33, "square_norm", X34);
	vm->add_slot(X33, "y", X3Y);
	vm->add_slot(X32, "punto", X33);
	vm->keyword_message(X31, "_AddSlots:", X32);
	
	//punto print.   #Print Method Object
	Slot* X36 = vm->search_obj("punto");
	vm->unary_message(X36, "print");
	
	Slot* X37 = vm->search_obj("punto1");
	Slot* X38 = vm->search_obj("punto");
	vm->add_parent(X37, "padre", X38);
	
	//punto1 square_norm.   #Print Method Object
	Slot* X39 = vm->search_obj("punto1");
	vm->unary_message(X39, "square_norm");
	
	//(punto y) print.
	Slot* X40 = vm->search_obj("punto");
	Slot* X41 = vm->unary_message(X40, "y");
	vm->unary_message(X41, "print");
	
	//(punto1 y) print.
	Slot* X42 = vm->search_obj("punto1");
	Slot* X43 = vm->unary_message(X42, "y");
	vm->unary_message(X43, "print");
	
	Slot* X44 = vm->search_obj("lobby");
	vm->rm_slot(X44, "y");
	
	vm->revert();
	
	/*** SIMULACION PEDIDO DE SLOTS DE LOBBY(CLIENTE)***/
	std::cout << "pido slots de lobby" << std::endl;
	//string string_to_send = server.get_slots("lobby");
	string string_to_send = server.get_slots("0");
	
	/***DECODIFICACION DEL JSON (CLIENTE)***/
	std::vector<InterfaceSlot*> i_slots;
	JsonReader slots_reader;
	slots_reader.read(i_slots, string_to_send);
	
	/***VEO SI LA LECTURA FUE CORRECTA***/
	int size = i_slots.size();
	for (int i = 0; i < size ; i++)
		i_slots[i] -> print_attr();
	
	for (std::vector<InterfaceSlot*>::iterator it = i_slots.begin(); it != i_slots.end();++it){  
	
		/*** SIMULACION PEDIDO DE SLOTS DE LOBBY(CLIENTE)***/
		std::vector<InterfaceSlot*> i_slots2;
		std::cout << "pido slots de " << (*it) -> get_name() << std::endl;
		string_to_send = server.get_slots(std::to_string((*it) -> get_id()));
	
		/***DECODIFICACION DEL JSON (CLIENTE)***/
		slots_reader.read(i_slots2, string_to_send);
	
		/***VEO SI LA LECTURA FUE CORRECTA***/
		size = i_slots2.size();
		for (int i = 0; i < size ; i++)
			i_slots2[i] -> print_attr();
		
		for (std::vector<InterfaceSlot*>::iterator itr = i_slots2.begin(); itr != i_slots2.end();){  
			delete* itr;  
			itr = i_slots2.erase(itr);
		}
	}
	
	for (std::vector<InterfaceSlot*>::iterator it = i_slots.begin(); it != i_slots.end();){  
		delete* it;  
		it = i_slots.erase(it);
	}
	
	/*** SIMULACION EJECUCION CODIGO(CLIENTE)***/
	JsonWriter writer;
	string json = writer.write_code("470", "punto11 _AddSlots: (| p=8. |).");
	
	string result = server.execute(json);
	
	std::cout << result << std::endl;
	
	json = writer.write_code("900", "'hello self!' print.");
	
	result = server.execute(json);
	
	std::cout << result << std::endl;
	/*******************************************/
	
	vm->collect();
	server.save_vm();
	return 0;
}
