#include "server_slot.h"
#include "server_int.h"
#include "server_float.h"
#include "server_string.h"
#include "server_sum.h"
#include "server_mult.h"
#include "server_sub.h"
#include "server_div.h"
#include "server_nil.h"
#include "server_error.h"
#include "server_equal.h"
#include "server_not_equal.h"
#include "server_boolean.h"
#include "server_json_writer.h"
#include <iostream>

static const char* sum = "+";
static const char* multip = "*";
static const char* subs = "-";
static const char* dv = "/";
static const char* eq = "==";
static const char* neq = "!=";

Slot::Slot(int id, string slot_name):Object(id){
	this -> name = slot_name;
	std::get<0>(this -> parent) = false;
	std::get<1>(this -> parent) = "";
	std::get<2>(this -> parent) = 0;
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

void Slot::set_obj_value(int id){
	if (this -> type)
		throw ErrorType(this -> name);
 	this -> value = new Object(id);
}

void Slot::set_nil_value(int id){
        if (this -> type)
		throw ErrorType(this -> name);
 	this -> value = new Nil(id);
}

void Slot::set_int_value(int id, int value){
	if (this -> type)
		throw ErrorType(this -> name);
 	this -> value = new Int(id, value);
}

void Slot::set_float_value(int id, float value){
	if (this -> type)
		throw ErrorType(this -> name);
 	this -> value = new Float(id, value);
}
 
void Slot::set_string_value(int id, string value){
	if (this -> type)
		throw ErrorType(this -> name);
 	this -> value = new String(id, value);
}

void Slot::set_error_value(int id, string value){
	if (this -> type)
		throw ErrorType(this -> name);
 	this -> value = new Error(id, value);
}

void Slot::set_boolean_value(int id, bool value){
	if (this -> type)
		throw ErrorType(this -> name);
	this -> value = new Boolean(id, value);
}
 
void Slot::set_int_method_value(int id, string name){
	if (name == sum)
		this -> value = new Sum(id);
	else if (name == multip)
		this -> value = new Mult(id);
	else if (name == subs)
		this -> value = new Sub(id);
	else if (name == dv)
		this -> value = new Div(id);
	else if (name == eq)
		this -> value = new Equal(id);
	else if (name == neq)
		this -> value = new NotEqual(id);
}

void Slot::set_name(string name){
	this -> name = name;
}

void Slot::add_slot(Slot* slot, p_slots& v_sl){
	if (this -> type)
		throw ErrorType(this -> name);
 	this -> value -> add_slot(slot, v_sl);
}

bool Slot::name_is(string name){
	return (this -> name == name);
}

Slot* Slot::execute(VM& vm, p_objects& args){
	return this -> value -> clone(vm);
}

void Slot::set_parent(bool val, string name_parent, int id_parent){
	std::get<0>(this -> parent) = val;
	std::get<1>(this -> parent) = name_parent;
	std::get<2>(this -> parent) = id_parent;
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
	return std::get<1>(this -> parent);
}

bool Slot::is_parent(){
	return std::get<0>(this -> parent);
}

bool Slot::is_code(){
	return this -> code;
}

bool Slot::is_immutable(){
	return this -> type;
}

void Slot::set_value(Object* obj){
	this -> value = obj;
}

int Slot::get_parent_id(){
	return std::get<2>(this -> parent);
}

void Slot::add_argument(string arg){
        args.push_back(arg);
}

void Slot::get_arguments(std::vector<string>& arguments){
        arguments = this -> args;
}

string Slot::json(int id_base){
	JsonWriter json_writer;
	string json = json_writer.write_slot(id_base, this);
	return json;
}

Slot::~Slot(){
	if ( (std::get<0>(this -> parent) == false) || is_code() || (this -> name == "_Self") )
		delete this -> value;
}
