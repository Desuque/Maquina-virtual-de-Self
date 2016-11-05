#include "interface_slot.h"
#include <iostream>

InterfaceSlot::InterfaceSlot(){
	this -> name = "";
	this -> value = "";
	this -> type = false;
	this -> code = false;
}

InterfaceSlot::InterfaceSlot(string name, string value, bool code, bool type){
	this -> name = name;
	this -> value = value;
	this -> type = type;
	this -> code = code;
  
}

void InterfaceSlot::print_attr(){
	std::cout << "Name " << this -> name
		  << " Value " << this -> value
		  << " Type " << this -> type
		  << " Code " << this -> code << std::endl;
}
