#include "server_virtual_machine.h"
#include "server_slot.h"
#include "server_not_found.h"
#include "server_check_point.h"
#include <iostream>

static const char* global_obj = "lobby";
static const char* name_slot = "_Name";
static const char* self_slot = "_Self";
static const char* obj_name = "object";
static const char* num_slots[] = {"+", "*", "-", "/", "==", "!="};
static const int idx_global = 0;

VM::VM(){
	this -> id_slots = 0;
	Slot* lobby = new Slot(this -> id_slots, global_obj);
	this -> id_slots++;
	lobby -> set_obj_value(get_id_slots());
	add_basic_slots(lobby, global_obj);
	lobby -> set_mark(true);
}

Slot* VM::immutable_object(Slot* sl){
	sl -> set_type(true);
	return sl;
}

void VM::unmark_slots(){
	for (map_slots::iterator it = slots.begin(); it != slots.end();++it)  
		(it->second) -> get_value() -> set_mark(false);
}

void VM::garbage_collector(){
	map_slots::iterator itr = slots.begin();
	while (itr != slots.end()) {
		if (!(itr->second) -> is_mark()){
			delete itr->second;
			itr = slots.erase(itr);
		}else{
			++itr;
		}
	}
}

void VM::collect(){
	Slot* lobby = search_obj("lobby");	
	unmark_slots();
	lobby -> get_value() -> mark_slots();
	garbage_collector();
}

string VM::get_slots(Slot* sl){
	return sl -> get_value() -> get_json_slots();
}

void VM::push_slot(Slot* sl){
	tmp_slots.push(sl);
}

Slot* VM::pop_slot(){
	Slot* top = tmp_slots.top();
	tmp_slots.pop();
	return top;
}

void VM::checkpoint(){
	Slot* check = new CheckPoint(get_id_slots(), "check");
	tmp_slots.push(check);
}

void VM::revert(){
	bool get_it = false;
	while (!get_it){
		Slot* sl = tmp_slots.top();
		tmp_slots.pop();
		if (sl -> is_check()){
			get_it = true;
			delete sl;
		}
	}
}

Slot* VM::search_obj(string name){
	Slot* lobby = this -> slots[idx_global];
	if (name == global_obj)
		return lobby;
	
	p_slots results;
	lobby -> get_value() -> look_up(name, results);
	if (results.size() == 0)
		return NULL;
	return results[0];
}

Slot* VM::add_slot(Slot* sl_recv, string sl_recv_id, Slot* sl){
	sl -> set_name(sl_recv_id);
	sl_recv -> add_slot(sl);
	return sl_recv;
}

Slot* VM::add_parent(Slot* sl_recv, string sl_recv_id, Slot* sl){
	/*Slot* sl_clone = sl  -> get_value() -> clone(*this);
	sl_clone -> set_name(sl_recv_id);
	sl_clone -> set_parent(true, sl -> get_name());
	sl_recv -> add_slot(sl_clone);
	return sl_recv;*/
	Slot* sl_p = new Slot(get_id_slots(), sl_recv_id);
	sl_p -> set_value(sl->get_value());
	add_basic_slots(sl_p, sl_recv_id);
	sl_p -> set_parent(true, sl_p -> get_name(), sl -> get_id());
	sl_recv -> add_slot(sl_p);
	this -> slots.insert(std::pair<int,Slot*>(sl_p -> get_id(),sl_p));
	return sl_recv;
}

Slot* VM::rm_slot(Slot* sl_recv, string slot){
	return sl_recv -> get_value() -> rm_slot(slot);
}

Slot* VM::add_code(Slot* sl_recv, string sl_recv_id, Slot* sl){
	sl -> set_code(true);
	return add_slot(sl_recv, sl_recv_id, sl);
}

void VM::add_default_name_slot(Slot* sl_recv, string name = obj_name){
	Slot* sl = new Slot(get_id_slots(), name_slot);
	sl -> set_string_value(get_id_slots(), name);
	sl_recv -> add_slot(sl);
	this -> slots.insert(std::pair<int,Slot*>(sl -> get_id(),sl));
}

Slot* VM::create_object(){
	Slot* sl = new Slot(get_id_slots(), obj_name);
	sl -> set_obj_value(get_id_slots());
	add_basic_slots(sl, obj_name);
	return sl;
}

Slot* VM::create_int(int value){
	Slot* sl = new Slot(get_id_slots(), std::to_string(value));
	sl -> set_int_value(get_id_slots(), value);
	add_basic_slots(sl, std::to_string(value));
	add_default_numeric_slots(sl);
	return sl;
}

Slot* VM::create_string(string value){
	Slot* sl = new Slot(get_id_slots(), value);
	sl -> set_string_value(get_id_slots(), value);
	add_basic_slots(sl, value);
	return sl;
}

Slot* VM::create_boolean(bool value){
	Slot* sl = new Slot(get_id_slots(), std::to_string(value));
	sl -> set_boolean_value(get_id_slots(), value);
	add_basic_slots(sl, std::to_string(value));
	return sl;
}

void VM::add_basic_slots(Slot* sl, string name){
	this -> slots.insert(std::pair<int,Slot*>(sl -> get_id(),sl));
	add_default_name_slot(sl, name);
	add_default_self_slot(sl);
}

Slot* VM::search_msg(Slot* sl_recv, string msg){
	p_slots results;
	sl_recv -> get_value() -> look_up(msg, results);
	if (results.size() == 1)
		return results[0];
	return NULL;
}

Slot* VM::search_and_execute_msg(Slot* sl_recv, string msg, p_objects& args){
	Slot* sl_msg = search_msg(sl_recv, msg);
	if (sl_msg){
		if (sl_msg -> is_code())
			std::cout << "Ejecutar " << sl_msg -> get_value() -> as_string() << std::endl;
		
		return execute_msg(sl_msg, sl_recv, args);
	}
	return NULL;
}

Slot* VM::unary_message(Slot* sl_recv, string msg){
	p_objects args;
	args.push_back(sl_recv -> get_value());
	Slot* sl_ret = search_and_execute_msg(sl_recv, msg, args);
	if (!sl_ret){
		if (msg == "print"){
			sl_recv -> get_value() -> print(*this);
			return  sl_recv;
		}
		throw NotFound(msg);
	}
	return sl_ret;
}

Slot* VM::binary_message(Slot* sl_recv, string msg, Slot* sl){
	p_objects args;
	args.push_back(sl_recv -> get_value());
	args.push_back(sl -> get_value());
	Slot* sl_ret = search_and_execute_msg(sl_recv, msg, args);
	if (!sl_ret)
		throw NotFound(msg);
	return sl_ret;
}

Slot* VM::execute_msg(Slot* msg, Slot* sl_invoker,p_objects& args){
	msg -> get_value() -> rm_slot(self_slot);
	Slot* recv = sl_invoker -> clone(*this);
	msg -> get_value() -> add_slot(recv);
	return msg -> get_value() -> execute(*this, args);
}

Slot* VM::keyword_message(Slot* sl_recv, string obj_id, Slot* sl){
	Slot* sl_lobby = sl -> get_value() -> as_slot();
	sl_recv -> add_slot(sl_lobby);
	return sl_recv;
}

void VM::add_default_numeric_slots(Slot* sl_recv){
	for(const string &name : num_slots){
		Slot* sl = new Slot(get_id_slots(), name);
		sl -> set_int_method_value(get_id_slots(), name);
		add_basic_slots(sl, name);
		sl_recv -> add_slot(sl);
	}
}

void VM::add_default_self_slot(Slot* sl_recv){
	Slot* sl = new Slot(get_id_slots(), self_slot);
	sl -> set_obj_value(get_id_slots());
	sl -> set_parent(true, self_slot, sl -> get_id());
	sl_recv -> add_slot(sl);
	this -> slots.insert(std::pair<int,Slot*>(sl -> get_id(),sl));
}

string VM::save(){
	Slot* lobby = search_obj(global_obj);
	string vm_slots = "lobby _AddSlots: "; 
	lobby -> get_value() -> get_self_slots(vm_slots);
	return vm_slots;
}

int VM::get_id_slots(){
	this ->id_slots++;
	return id_slots;
}

Slot* VM::search_obj_id(int id){
	return slots[id];
}

VM::~VM(){
	for (map_slots::iterator it = slots.begin(); it != slots.end();++it)  
		delete (it->second);
}
