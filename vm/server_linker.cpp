
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
	std::cout<<"Entro al unary"<<std::endl;
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

Slot* Linker::create_slot(Slot* object, std::string slot_name_extended) {
	Slot* X1 = vm->create_object();
	Slot* X2 = vm->create_nil();
	vm->add_slot(X1, slot_name_extended, X2);
	vm->add_slot(object, "", X1);

	return object;
}

Slot* Linker::create_nil() {
	return vm->create_nil();
}

Slot* Linker::create_error(std::string msg_error) {
	return vm->create_error(msg_error);
}

Slot* Linker::remove_slots(Slot* slot, std::string slot_to_erase) {
	if(slot == NULL) {
		slot = vm->create_object();
	}
	Slot* X2 = vm->create_object();
	Slot* X3 = vm->create_string(slot_to_erase);
	vm->add_slot(X2, slot_to_erase, X3);
	vm->add_slot(slot, "", X2);
	return slot;
}

Slot* Linker::set_object_script(Slot* slot, std::string script) {
	Slot* X34 = vm->create_string(script);
	

	return vm->create_string(script);
/**

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
	vm.keyword_message(X31, "_AddSlots", cont4);
**/}

Slot* Linker::only_name(std::string name, Slot* slot) {
	return get_object_by_name(name);
	//return create_unary_message(X1, "print");
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
