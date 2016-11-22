
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

Slot* Linker::get_object_by_name(std::string name) {
	return vm->search_obj_by_name(name, atoi(id.c_str()));
}

Slot* Linker::get_context(std::string name) {
	return vm->search_obj(name);
}

Slot* Linker::collect() {
	return vm->collect();
}

bool Linker::remove_slots(std::string context, std::string slot) {
	/**
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
	**/
}

Linker::~Linker() {
}
