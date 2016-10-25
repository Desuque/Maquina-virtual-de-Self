#ifndef __virtual_machine_h__
#define __virtual_machine_h__

#include <vector>
#include <string>
#include "object.h"

typedef std::vector<Slot*> p_slots;
typedef std::vector<Object*> p_objects;
typedef std::string string;

class VM{
	private:
		p_slots slots;
		
	public:
		VM();
		Slot* search_obj(string name);
		Slot* create_object();
		Slot* create_int(int value);
		Slot* create_string(string value);
		Slot* create_boolean(bool value);
		Slot* add_slot(Slot* sl_recv, string sl_recv_id, Slot* sl);
		Slot* add_parent(Slot* sl_recv, string sl_recv_id, Slot* sl);
		Slot* add_code(Slot* sl_recv, string sl_recv_id, Slot* sl);
		Slot* rm_slot(Slot* sl_recv, string slot);
		Slot* unary_message(Slot* sl_recv, string msg);
		Slot* binary_message(Slot* sl_recv, string msg, Slot* sl);
		Slot* keyword_message(Slot* sl_recv, string obj_id, Slot* sl);
		~VM();
		
	private:
		Slot* search_and_execute_msg(Slot* sl_recv, string msg, p_objects& args);
		Slot* execute_msg(Slot* msg, Slot* sl_invoker, p_objects& args);
		Slot* search_msg(Slot* sl_recv, string msg);
		void add_basic_slots(Slot* sl, string name);
		void add_default_name_slot(Slot* sl, string name);
		void add_default_self_slot(Slot* sl);
		void add_default_numeric_slots(Slot* sl_recv);
};

#endif
