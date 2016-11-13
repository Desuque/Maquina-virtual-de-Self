#include "server_float.h"
#include <iostream>
#include "server_virtual_machine.h"

Float::Float(int id, float value):Object(id){
	this -> value = value;
}

int Float::as_int(){
	return this -> value;
}

Slot* Float::print(VM& vm){
	std::cout << this -> value << std::endl;
	return vm.create_float(this -> value);
}

string Float::as_string(){
	return std::to_string(this -> value);
}

Slot* Float::clone(VM& vm){
	Slot* new_sl = vm.create_float(this -> value);
	return new_sl;
}

void Float::get_self_slots(string& slots_self){
	slots_self += as_string() + ".";
}

