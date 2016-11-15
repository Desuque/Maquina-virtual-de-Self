
#include "Linker.h"
#include "server_virtual_machine.h"
#include <iostream>

Linker::Linker() : vm(NULL) {
	//vm->checkpoint();
}

void Linker::setVM(VM* vm) {
	this->vm = vm;
}

Slot* Linker::get_last_slot() {
	return get_slot(get_last_created_pos());
}

int Linker::get_last_created_pos() {
	return slots.size()-1;
}

Slot* Linker::get_slot(int pos) {
	return slots.at(pos);
}

void Linker::create_unary_message(std::string msg) {
	std::cout<<"Aca entro.asdasdasds.."<<std::endl;
	Slot* slot = vm->unary_message(get_slot(get_last_created_pos()), msg);
	slots.push_back(slot);
}

void Linker::create_unary_message(std::string name, std::string msg) {
	Slot* slot = get_object_by_name(name);
	Slot* last_slot = vm->unary_message(slot, msg);
	slots.push_back(last_slot);
}

void Linker::create_int(std::string number) {
	//Slot* X0 = vm->create_object();
	Slot* X1 = vm->create_int(atoi(number.c_str()));
	//vm->add_slot(X0, number, X1);
	slots.push_back(X1); // slots.push_back(X0);
}

void Linker::create_string(std::string string) {
	//Slot* X0 = vm->create_object();
	Slot* X1 = vm->create_string(string);
	//vm->add_slot(X0, string, X1);
	slots.push_back(X1);// slots.push_back(X0);
}

void Linker::create_binary_message(std::string op) {
	Slot* slot = vm->binary_message(get_slot(get_last_created_pos()-1), op, get_slot(get_last_created_pos()));
	slots.push_back(slot);
}

void Linker::create_keyword_message(std::string obj, std::string lower_key) {
	//lobby _AddSlots: (| y <- 8. |).
	Slot* X0 = vm->search_obj(obj);

	//Slot* X1 = vm.create_object();
	//Slot* X2 = vm.create_int(8);
	//vm.add_slot(X1, "y", X2);

	vm->keyword_message(X0, lower_key, get_slot(get_last_created_pos()));
}

void Linker::remove_slots(std::string context, std::string slot) {
	Slot* X0 = vm->search_obj(context);
	Slot* X1 = vm->create_object();
	Slot* X2 = vm->create_object();
	Slot* X3 = vm->create_string(slot);
	vm->add_slot(X2, slot, X3);
	vm->add_slot(X1, "", X2);
	slots.push_back(vm->keyword_message(X0, "_RemoveSlots:", X1));
}

void Linker::create_slot(std::string slot) {
	Slot* X1 = vm->create_object();
	Slot* X2 = get_slot(get_last_created_pos());
	slots.push_back(X1);

	vm->add_slot(X1, slot, X2);
}

Slot* Linker::get_object_by_name(std::string name) {
	return vm->search_obj(name);
}
















/**


//ESTE METODO NO SE USA!
void Linker::unary_message(std::string message) {
	//Lo tabeado esta terminado!!!
		//lobby _AddSlots: (| y <- 8. |).
		Slot* X0 = vm.search_obj("lobby");
		Slot* X1 = vm.create_object();
		Slot* X2 = vm.create_int(8);
		vm.add_slot(X1, "y", X2);
		vm.keyword_message(X0, "_AddSlots:", X1);
		vm.collect();
		//y print.
		Slot* X4 = vm.search_obj("y");
		vm.unary_message(X4, "print");

	//lobby _AddSlots: (| punto1 = (| y <- 4. |). |).
	Slot* X6 = vm.search_obj("lobby");
	Slot* X7 = vm.create_object();
	Slot* X8 = vm.create_object();
	Slot* X9 = vm.create_int(4);
	vm.add_slot(X8, "y", X9);
	vm.add_slot(X7, "punto1", X8);
	vm.keyword_message(X6, "_AddSlots:", X7);

	//(punto1 y) print.
	Slot* X10 = vm.search_obj("punto1");
	Slot* X11 = vm.unary_message(X10, "y");
	vm.unary_message(X11, "print");

	//((punto1 y) + y) print.
	Slot* X12 = vm.search_obj("punto1");
	Slot* X13 = vm.unary_message(X12, "y");
	Slot* X14 = vm.search_obj("y");
	Slot* X15 = vm.binary_message(X13,"+",X14);
	vm.unary_message(X15, "print");

	//((punto1 y) != y) print.
	Slot* X16 = vm.search_obj("punto1");
	Slot* X17 = vm.unary_message(X16, "y");
	Slot* X18 = vm.search_obj("y");
	Slot* X19 = vm.binary_message(X17,"!=",X18);
	vm.unary_message(X19, "print");
	vm.collect();

	//lobby _AddSlots: (| str = 'hello'. |).
		Slot* X20 = vm.search_obj("lobby");
		Slot* X21 = vm.create_object();
		Slot* X22 = vm.create_string("hello");
		vm.add_slot(X21, "str", X22);
		vm.keyword_message(X20, "_AddSlots:", X21);

		//str print.
		Slot* X23 = vm.search_obj("str");
		vm.unary_message(X23, "print");


	//lobby _AddSlots: (| punto11 = (| x<-5. padre* = punto1 |)).
	Slot* X24 = vm.search_obj("lobby");
	Slot* X25 = vm.create_object();
	Slot* X26 = vm.search_obj("punto1");
	Slot* X27 = vm.create_object();
	Slot* X28 = vm.create_int(5);
	vm.add_slot(X27, "x", X28);
	vm.add_parent(X27, "padre", X26);
	vm.add_slot(X25, "punto11", X27);
	vm.keyword_message(X24, "_AddSlots:", X25);

	//(punto11 y) print.
	Slot* X29 = vm.search_obj("punto11");
	Slot* X30 = vm.unary_message(X29, "y");
	vm.unary_message(X30, "print");
	vm.collect();
	//lobby _AddSlots: (| punto = (| square_norm = (| | ((x*x)+(y*y))).
	//				 print = (| | print x.).
	//				 y <- 7.
	//			      |).
	//		    |).

	Slot* X31 = vm.search_obj("lobby");
	Slot* X32 = vm.create_object();
	Slot* X33 = vm.create_object();
	Slot* X34 = vm.create_string("((x*x)+(y*y))");
	Slot* X35 = vm.create_string("print x.");
	Slot* X3Y = vm.create_int(7);
	vm.add_code(X33, "print", X35);
	vm.add_code(X33, "square_norm", X34);
	vm.add_slot(X33, "y", X3Y);
	vm.add_slot(X32, "punto", X33);
	vm.keyword_message(X31, "_AddSlots:", X32);

	//punto print.   #Print Method Object
	Slot* X36 = vm.search_obj("punto");
	vm.unary_message(X36, "print");

	Slot* X37 = vm.search_obj("punto1");
	Slot* X38 = vm.search_obj("punto");
	vm.add_parent(X37, "padre", X38);

	//punto1 square_norm.   #Print Method Object
	Slot* X39 = vm.search_obj("punto1");
	vm.unary_message(X39, "square_norm");

	//(punto y) print.
	Slot* X40 = vm.search_obj("punto");
	Slot* X41 = vm.unary_message(X40, "y");
	vm.unary_message(X41, "print");

	//(punto1 y) print.
	Slot* X42 = vm.search_obj("punto1");
	Slot* X43 = vm.unary_message(X42, "y");
	vm.unary_message(X43, "print");

	Slot* X44 = vm.search_obj("lobby");
	vm.rm_slot(X44, "y");

	vm.revert();
	vm.collect();

}
**/

Linker::~Linker() {
	// TODO Auto-generated destructor stub
}
