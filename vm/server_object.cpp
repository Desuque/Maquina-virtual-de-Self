#include "server_object.h"
#include "server_slot.h"
#include "server_virtual_machine.h"
#include "server_json_writer.h"
#include "server_self_writer.h"
#include "server_error_type.h"
#include <iostream>

static const int default_slots_size = 2;
static const char* class_name = "object";
static const char* name_slot = "_Name";
static const char* self_slot = "_Self";

Object::Object(int id){
	this -> mark = false;
	this -> id = id;
}

int Object::get_id(){
	return this -> id;
}

void Object::set_mark(bool val){
	this -> mark = val;
}


bool Object::is_mark(){
	return this -> mark;
}

void Object::add_slot(Slot* sl){
        if ( sl ){
            Slot* sl_rep = get_slot(sl -> get_name());
            if (!sl_rep || (sl_rep -> get_name() == "")){
                    slots.insert({sl -> get_name(), sl});
            }else{
                if (!sl_rep -> is_immutable()){
                        rm_slot(sl -> get_name());
                        slots.insert({sl -> get_name(), sl});
                }else{
                        //throw ErrorType(sl -> get_name());
                        throw "Objeto NO MUTABLE";
                }
            }
        }
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
	    //return;
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

float Object::as_float(){
        return 0;
}

Slot* Object::add(VM& vm, Object* op){
        return vm.create_int(0);
}

Slot* Object::div(VM& vm, Object* op){
        return vm.create_int(0);
}

Slot* Object::sub(VM& vm, Object* op){
        return vm.create_int(0);
}

Slot* Object::mult(VM& vm, Object* op){
        return vm.create_int(0);
}

Slot* Object::equal(VM& vm, Object* op){
	return vm.create_boolean(0);
}

Slot* Object::nequal(VM& vm, Object* op){
	return vm.create_boolean(0);
}

string Object::get_json_slots(int id_base){
	JsonWriter json_writer;
	string json = json_writer.write(id_base, slots);
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
	return vm.create_string(class_name);
}

p_slots Object::get_slots(){
        p_slots v_slots;
        for (m_slots::iterator it=slots.begin(); it!=slots.end(); ++it){
		if (!is_base_slot(it->second)){
			v_slots.push_back((it->second)->get_value()->as_slot());
                }
	}
	return std::move(v_slots);
}

Slot* Object::as_slot(){
	for (m_slots::iterator it=slots.begin(); it!=slots.end(); ++it){
		if (!is_base_slot(it->second))
			return (it->second);
	}
	return NULL;
}

bool Object::is_container(){
        if (slots.size() == default_slots_size)
            return false;
        
        for (m_slots::iterator it=slots.begin(); it!=slots.end(); ++it){
		if ((it->second)->get_name() != "" && !is_base_slot(it->second))
			return false;
	}
	
	return true;
}

Object::~Object(){}
