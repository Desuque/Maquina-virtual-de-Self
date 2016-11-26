
#include "server_null_linker.h"

nLinker::nLinker() {
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

Slot* nLinker::create_keyword_message(std::string lower_or_cap, Slot* expCP) {
	return NULL;
}

Slot* nLinker::create_object() {
	return NULL;
}

Slot* nLinker::create_slot(Slot* object, std::string slot_name_extended, std::string op, Slot* exp, bool code_flag) {
	return NULL;
}

Slot* nLinker::create_slot(Slot* object, std::string slot_name_extended) {
	return NULL;
}

Slot* nLinker::create_nil() {
	return NULL;
}

Slot* nLinker::create_error(std::string msg_error) {
	return NULL;
}

Slot* nLinker::remove_slots(Slot* slot, std::string slot_to_erase) {
	return NULL;
}

Slot* nLinker::set_object_script(Slot* slot, std::string script, std::string msg_slot_name_extended) {
	return NULL;
}

Slot* nLinker::clone_obj_by_name(std::string receiver) {
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
