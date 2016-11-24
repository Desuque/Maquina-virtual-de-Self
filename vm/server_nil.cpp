#include "server_nil.h"
#include <iostream>
#include "server_virtual_machine.h"

static const string name = "nil";
Nil::Nil(int id):Object(id){
	this -> value = name;
}

string Nil::as_string(){
	return this -> value;
}

Slot* Nil::print(VM& vm){
	std::cout << this -> value << std::endl;
	return vm.create_nil();
}

Slot* Nil::clone(VM& vm){
	Slot* new_sl = vm.create_nil(); 
	return new_sl;
}

void Nil::get_self_slots(string& slots_self){
	slots_self += " " + as_string() + " .";
}
