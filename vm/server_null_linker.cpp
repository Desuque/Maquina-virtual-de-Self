
#include "server_null_linker.h"
#include "server_virtual_machine.h"
#include <iostream>
#include <sstream>

nLinker::nLinker() : vm(NULL), id("-1") {
}

void nLinker::setVM(VM* vm) {
	this->vm = vm;
}

void nLinker::setID(std::string id) {
	this->id = id;
}

Slot* nLinker::create_number(std::string number) {
	return NULL;
}

Slot* nLinker::create_float(std::string number) {
	return NULL;
}

Slot* nLinker::create_int(std::string number) {
	return NULL;
}

Slot* nLinker::create_string(std::string string) {
	return NULL;
}

Slot* nLinker::create_unary_message(Slot* receiver, std::string name) {
	return NULL;
}

Slot* nLinker::create_binary_message(Slot* receiver, std::string op, Slot* expCP) {
	return NULL;
}

Slot* nLinker::create_keyword_message(Slot* receiver, std::string lower_or_cap, Slot* expCP) {
	return NULL;
}

Slot* nLinker::create_object() {
	return NULL;
}

Slot* nLinker::create_slot(Slot* object, std::string slot_name_extended, std::string op, Slot* exp) {
	return NULL;
}

Slot* nLinker::create_slot(Slot* object, std::string slot_name_extended) {
	return NULL;
}

Slot* nLinker::create_nil() {
	return NULL;
}

Slot* nLinker::remove_slots(Slot* slot, std::string slot_to_erase) {
	return NULL;
}

Slot* nLinker::only_name(std::string name, Slot* slot) {
	return NULL;
}

Slot* nLinker::get_object_by_name(std::string name) {
	return NULL;
}

Slot* nLinker::get_context(std::string name) {
	return NULL;
}

Slot* nLinker::collect() {
	return NULL;
}

nLinker::~nLinker() {
}
