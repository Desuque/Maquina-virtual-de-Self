#include "client_interface_slot.h"
#include <iostream>

InterfaceSlot::InterfaceSlot(){
	this -> name = "";
	this -> value = "";
	this -> type = false;
	this -> code = false;
}

InterfaceSlot::InterfaceSlot(string name, string value, bool code, bool type, int id, int id_base){
	this -> name = name;
	this -> value = value;
	this -> type = type;
	this -> code = code;
	this -> id = id;  
        this -> id_base = id_base;
}

void InterfaceSlot::print_attr(){
	std::cout << "ID_Base " << this -> id_base
                  << " ID " << this -> id
		  << " Name " << this -> name
		  << " Value " << this -> value
		  << " Type " << this -> type
		  << " Code " << this -> code << std::endl;
}

std::string InterfaceSlot::get_name(){
	return this->name;
}

std::string InterfaceSlot::get_value(){
	return this->value;
}

bool InterfaceSlot::has_code(){
	return this->code;
}

bool InterfaceSlot::get_type(){
	return this->type;
}

int InterfaceSlot::get_id(){
	return this->id;
}

int InterfaceSlot::get_id_base(){
	return this->id_base;
}
