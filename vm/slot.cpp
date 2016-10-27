#include "slot.h"
#include "int.h"
#include "string.h"
#include "sum.h"
#include "mult.h"
#include "sub.h"
#include "div.h"
#include "equal.h"
#include "not_equal.h"
#include "boolean.h"
#include <iostream>

static const char* sum = "+";
static const char* mult = "*";
static const char* sub = "-";
static const char* dv = "/";
static const char* eq = "==";
static const char* neq = "!=";

Slot::Slot(string slot_name){
	this -> name = slot_name;
	this -> parent = false;
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
	this -> value = new Object();
}

void Slot::set_int_value(int value){
	this -> value = new Int(value);
}

void Slot::set_string_value(string value){
	this -> value = new String(value);
}

void Slot::set_boolean_value(bool value){
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
	this -> value -> add_slot(slot);
}

bool Slot::name_is(string name){
	return (this -> name == name);
}

Slot* Slot::execute(VM& vm, p_objects& args){
	return this -> value -> clone(vm);
}

void Slot::set_parent(bool val){
	this -> parent = val;
}

void Slot::set_code(bool val){
	this -> code = val;
}

string Slot::get_name(){
	return this -> name;
}

bool Slot::is_parent(){
	return this -> parent;
}

bool Slot::is_code(){
	return this -> code;
}

Slot::~Slot(){
	delete this -> value;
}
