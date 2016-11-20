#include "server_int.h"
#include <iostream>
#include "server_virtual_machine.h"

Int::Int(int id, int value):Object(id){
	this -> value = value;
}

int Int::as_int(){
	return this -> value;
}

Slot* Int::print(VM& vm){
	std::cout << this -> value << std::endl;
	return vm.create_int(this -> value);
}

string Int::as_string(){
	return std::to_string(this -> value);
}

Slot* Int::clone(VM& vm){
	Slot* new_sl = vm.create_int(this -> value);
	return new_sl;
}

void Int::get_self_slots(string& slots_self){
	slots_self += " " + as_string() + " .";
}
