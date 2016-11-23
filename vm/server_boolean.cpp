#include "server_boolean.h"
#include <iostream>
#include "server_virtual_machine.h"

Boolean::Boolean(int id, bool value):Object(id){
	this -> value = value;
}

Slot* Boolean::print(VM& vm){
	std::cout << this -> value << std::endl;
        string rep = "false";
        if (value)
            rep = "true";
	return vm.create_string(rep);
}

string Boolean::as_string(){
        string rep = "false";
        if (value)
            rep = "true";
	return rep;
}

Slot* Boolean::clone(VM& vm){
	Slot* new_sl = vm.create_boolean(this -> value);
	return new_sl;
}

void Boolean::get_self_slots(string& slots_self){
	slots_self += as_string() + ". ";
}
