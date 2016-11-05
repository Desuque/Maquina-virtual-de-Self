#include "int.h"
#include <iostream>
#include "virtual_machine.h"

Int::Int(int value){
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

