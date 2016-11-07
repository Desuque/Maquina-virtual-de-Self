#include "server_slot.h"
#include "server_int.h"
#include "server_string.h"
#include "server_sum.h"
#include "server_mult.h"
#include "server_sub.h"
#include "server_div.h"
#include "server_equal.h"
#include "server_not_equal.h"
#include "server_boolean.h"
#include <iostream>

static const char* sum = "+";
static const char* mult = "*";
static const char* sub = "-";
static const char* dv = "/";
static const char* eq = "==";
static const char* neq = "!=";

Slot::Slot(string slot_name){
	this -> name = slot_name;
	this -> parent.first = false;
	this -> parent.second = "";
	this -> type = false;
	this -> code = false;
	this -> value = NULL;
}

bool Slot::is_check(){
	return false;
}

Object* Slot::get_value(){
	return this -> value;
}

void Slot::set_obj_value(){
	if (this -> type)
		throw ErrorType(this -> name);
 	this -> value = new Object();
}
 
void Slot::set_int_value(int value){
	if (this -> type)
		throw ErrorType(this -> name);
 	this -> value = new Int(value);
}
 
void Slot::set_string_value(string value){
	if (this -> type)
		throw ErrorType(this -> name);
 	this -> value = new String(value);
}
 
void Slot::set_boolean_value(bool value){
	if (this -> type)
		throw ErrorType(this -> name);
	this -> value = new Boolean(value);
}
 
void Slot::set_int_method_value(string name){
	if (name == sum)
		this -> value = new Sum();
	else if (name == mult)
		this -> value = new Mult();
	else if (name == sub)
		this -> value = new Sub();
	else if (name == dv)
		this -> value = new Div();
	else if (name == eq)
		this -> value = new Equal();
	else if (name == neq)
		this -> value = new NotEqual();
}

void Slot::set_name(string name){
	this -> name = name;
}

void Slot::add_slot(Slot* slot){
	if (this -> type)
		throw ErrorType(this -> name);
 	this -> value -> add_slot(slot);
}

bool Slot::name_is(string name){
	return (this -> name == name);
}

Slot* Slot::execute(VM& vm, p_objects& args){
	return this -> value -> clone(vm);
}

void Slot::set_parent(bool val, string name_parent){
	this -> parent.first = val;
	this ->parent.second = name_parent;
}

void Slot::set_code(bool val){
	this -> code = val;
}

void Slot::set_type(bool val){
	this -> type = val;
}

string Slot::get_name(){
	return this -> name;
}

string Slot::parent_name(){
	return this -> parent.second;
}

bool Slot::is_parent(){
	return this -> parent.first;
}

bool Slot::is_code(){
	return this -> code;
}

bool Slot::is_immutable(){
	return this -> type;
}

Slot::~Slot(){
	delete this -> value;
}