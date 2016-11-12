#include "string.h"
#include <iostream>
#include "virtual_machine.h"

String::String(string value){
	this -> value = value;
}

string String::as_string(){
	return this -> value;
}

Slot* String::print(VM& vm){
	std::cout << this -> value << std::endl;
	return vm.create_string(this->value);
}

Slot* String::clone(VM& vm){
	Slot* new_sl = vm.create_string(this -> value); 
	return new_sl;
}
