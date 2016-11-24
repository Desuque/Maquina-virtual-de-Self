#include "server_error.h"
#include <iostream>
#include "server_virtual_machine.h"

Error::Error(int id, string value):Object(id){
	this -> value = value;
}

string Error::as_string(){
	return this -> value;
}

Slot* Error::print(VM& vm){
	std::cout << this -> value << std::endl;
	return vm.create_error(value);
}

Slot* Error::clone(VM& vm){
	Slot* new_sl = vm.create_error(value); 
	return new_sl;
}

void Error::get_self_slots(string& slots_self){
	slots_self += " " + as_string() + " .";
}
