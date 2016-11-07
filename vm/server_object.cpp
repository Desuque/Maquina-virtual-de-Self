#include "server_object.h"
#include "server_slot.h"
#include "server_virtual_machine.h"
#include "server_json_writer.h"
#include "server_self_writer.h"
#include <iostream>

static const char* class_name = "object";
static const char* name_slot = "_Name";
static const char* self_slot = "_Self";

Object::Object(){
	this -> mark = false;
}


void Object::set_mark(bool val){
	this -> mark = val;
}


bool Object::is_mark(){
	return this -> mark;
}

void Object::add_slot(Slot* sl){
	slots.insert({sl -> get_name(), sl});
}

Slot* Object::get_slot(string name){
	for (m_slots::iterator it=slots.begin(); it!=slots.end(); ++it){
		if (it->second -> name_is(name))
			return it->second;
	}
	return NULL;
}

Slot* Object::rm_slot(string name){
	for (m_slots::iterator it=slots.begin(); it!=slots.end(); ++it){
		if ((it->second) -> name_is(name)){
			Slot* sl = it->second;
			slots.erase(it);
			return sl;
		}
	}
	return NULL;
}

void Object::mark_slots(){
	for (m_slots::iterator it=slots.begin(); it!=slots.end(); ++it){
			(it->second) -> get_value() -> mark_slots();
			(it->second) -> set_mark(true);
	}
}

void Object::look_up(string name, p_slots& results){
	Slot* sl = get_slot(name);
	if (sl){
	    results.push_back(sl);
	    return;
	}
	
	for (m_slots::iterator it=slots.begin(); it!=slots.end(); ++it){
		if ((it->second) -> is_parent())
			(it->second) -> get_value() -> look_up(name, results);
	}
}

string Object::as_string(){
	return class_name;
}

int Object::as_int(){
	return 0;
}

string Object::get_json_slots(){
	JsonWriter json_writer;
	string json = json_writer.write(slots);
	return json;
}

void Object::get_self_slots(string& slots_self){
	SelfWriter self_writer;
	self_writer.write(slots, slots_self);
}

Slot* Object::clone(VM& vm){
	Slot* new_sl = vm.create_object(); 
	for (m_slots::iterator it=slots.begin(); it!=slots.end(); ++it){
		if (!is_base_slot(it->second)){
			Slot* sl = (it->second) -> get_value() -> clone(vm);
			
			if ((it->second) -> is_parent()){
				new_sl = vm.add_parent(new_sl, (it->first), sl);
			}else if ((it->second) -> is_code()){
				new_sl = vm.add_code(new_sl, (it->first), sl);
			}else{
				new_sl = vm.add_slot(new_sl, (it->first), sl);
			}
		}
	}
	return new_sl;
}

bool Object::is_base_slot(Slot* sl){
	string name = sl -> get_name();
	return ( (name == self_slot) || (name == name_slot) );
}

bool Object::is_num_slot(Slot* sl){
	string name = sl -> get_name();
	return ( (name == "+") || (name == "-") || (name == "*") || (name == "/") || 
		 (name == "==") || (name == "!=") );
}

Slot* Object::execute(VM& vm, p_objects& args){
	return clone(vm);
}

Slot* Object::print(VM& vm){
	std::cout << class_name << std::endl;
	return vm.create_string(class_name);
}

Slot* Object::as_slot(){
	for (m_slots::iterator it=slots.begin(); it!=slots.end(); ++it){
		if (!is_base_slot(it->second))
			return (it->second);
	}
	return NULL;
}

Object::~Object(){}