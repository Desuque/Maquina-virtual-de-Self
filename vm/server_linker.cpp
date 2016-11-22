
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

Slot* Linker::create_number(std::string number) {
	std::size_t found = number.find(".");
	if(found != std::string::npos) {
		return create_float(number);
	} else {
		return create_int(number);
	}
	return NULL;
}

Slot* Linker::create_float(std::string number) {
	std::stringstream buffer(number);
 	float temp;
 	buffer >> temp;

	Slot* X1 = vm->create_float(temp);
	return X1;
}

Slot* Linker::create_int(std::string number) {
	Slot* X1 = vm->create_int(atoi(number.c_str()));
	return X1;
}

Slot* Linker::create_string(std::string string) {
	Slot* X1 = vm->create_string(string);
	return X1;
}

Slot* Linker::create_unary_message(Slot* receiver, std::string name) {
	return vm->unary_message(receiver, name);
}

Slot* Linker::create_binary_message(Slot* receiver, std::string op, Slot* expCP) {
	return vm->binary_message(receiver, op, expCP);
}

Slot* Linker::create_keyword_message(Slot* receiver, std::string lower_or_cap, Slot* expCP) {
	return vm->keyword_message(receiver, lower_or_cap, expCP);
}

Slot* Linker::create_object() {
	return vm->create_object();
}

Slot* Linker::create_slot(Slot* object, std::string slot_name_extended, std::string op, Slot* exp) {
	Slot* X1 = vm->create_object();
	Slot* X2 = exp;
	if(op == "<-") {
		vm->immutable_object(X2);
	}
	vm->add_slot(X1, slot_name_extended, X2);
	vm->add_slot(object, "", X1);

	return object;
}

Slot* Linker::remove_slots(Slot* name) {
	        //lobby _RemoveSlots: (| prueba1. prueba2. |).
	
	Slot* X1 = vm->create_object();
	vm->add_slot(X1, "", name);

	return X1;    
	/**
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
**/
}


Slot* Linker::get_object_by_name(std::string name) {
	return vm->search_obj_by_name(name, atoi(id.c_str()));
}

Slot* Linker::get_context(std::string name) {
	return vm->search_obj(name);
}

Slot* Linker::collect() {
	return vm->collect();
}

Linker::~Linker() {
}
