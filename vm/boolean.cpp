#include "boolean.h"
#include <iostream>
#include "virtual_machine.h"

Boolean::Boolean(bool value){
	this -> value = value;
}

Slot* Boolean::print(VM& vm){
	std::cout << this -> value << std::endl;
	return vm.create_boolean(this -> value);
}

string Boolean::as_string(){
	return std::to_string(this -> value);
}

Slot* Boolean::clone(VM& vm){
	Slot* new_sl = vm.create_boolean(this -> value);
	return new_sl;
}

