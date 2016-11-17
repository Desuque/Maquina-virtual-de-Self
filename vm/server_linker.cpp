
#include "server_linker.h"
#include "server_virtual_machine.h"
#include <iostream>
#include <sstream>

Linker::Linker() : vm(NULL), id("-1") {
}

void Linker::setVM(VM* vm) {
	this->vm = vm;
}

void Linker::setID(std::string id) {
	this->id = id;
}

Slot* Linker::get_last_slot() {
	return get_slot(get_last_created_pos());
}

int Linker::get_last_created_pos() {
	return slots.size()-1;
}

Slot* Linker::get_slot(int pos) {
	Slot* aux = slots.at(pos);
	slots.erase(slots.begin()+pos-1);
	return aux;
}

void Linker::create_unary_message(std::string msg) {
	Slot* slot = vm->unary_message(get_slot(get_last_created_pos()), msg);
	slots.push_back(slot);
}

Slot* Linker::get_object_by_name(std::string name, int id) {
	return vm->search_obj_by_name(name, id);
}

bool Linker::create_unary_message(std::string name, std::string msg) {
	Slot* slot = get_object_by_name(name, atoi(id.c_str()));

	if(slot == NULL) {
		slots.push_back(slot);
		return false;	
	}

	Slot* last_slot = vm->unary_message(slot, msg);
	slots.push_back(last_slot);	
	return true;
}

void Linker::create_number(std::string number) {
	std::size_t found = number.find(".");
	if(found != std::string::npos) {
		create_float(number);
	} else {
		create_int(number);
	}
}

void Linker::create_float(std::string number) {
	std::stringstream buffer(number);
 	float temp;
 	buffer >> temp;

	Slot* X1 = vm->create_float(temp);
	slots.push_back(X1);
}

void Linker::create_int(std::string number) {
	Slot* X1 = vm->create_int(atoi(number.c_str()));
	slots.push_back(X1);
}

void Linker::create_string(std::string string) {
	Slot* X1 = vm->create_string(string);
	slots.push_back(X1);
}

void Linker::create_binary_message(std::string op) {
	Slot* slot = vm->binary_message(get_slot(get_last_created_pos()-1), op, get_slot(get_last_created_pos()));
	slots.push_back(slot);
}

void Linker::create_keyword_message(std::string obj, std::string lower_key) {
	Slot* X0 = vm->search_obj(obj);

	for(unsigned int i=0; i<slots_names.size(); i++) {
		//Cargo al objeto todos los slots que se cargaron
		Slot* X1 = vm->create_object();
		vm->add_slot(X1, "", slots_names.at(i));
		vm->keyword_message(X0, lower_key, X1);
	}

}

bool Linker::remove_slots(std::string context, std::string slot) {
	Slot* X0 = vm->search_obj(context);
	Slot* X1 = vm->create_object();
	Slot* X2 = vm->create_object();
	Slot* X3 = vm->create_string(slot);
	vm->add_slot(X2, slot, X3);
	vm->add_slot(X1, "", X2);
	slots.push_back(vm->keyword_message(X0, "_RemoveSlots", X1));

	if(slots.at(get_last_created_pos()) == NULL) {
		return false;
	} else {
		return true;
	}

}

void Linker::create_slot(std::string slot) {

    //lobby _AddSlots: (| prueba1 = 'hello'. prueba2 = 'hello2'. |).
	/**Slot* X90 = vm->search_obj("lobby");
	Slot* X91 = vm->create_object();
    Slot* X92 = vm->create_object();
    Slot* X93 = vm->create_string("hello");
    Slot* X94 = vm->create_object();
    Slot* X95 = vm->create_string("hello2");
    vm->add_slot(X92, "prueba1", X93);
    vm->add_slot(X94, "prueba2", X95);
    vm->add_slot(X91, "", X92);
    vm->add_slot(X91, "", X94);
**/


	Slot* X1 = vm->create_object();
	Slot* X2 = get_slot(get_last_created_pos());

	vm->add_slot(X1, slot, X2);
	slots_names.push_back(X1);
}

Linker::~Linker() {
}
