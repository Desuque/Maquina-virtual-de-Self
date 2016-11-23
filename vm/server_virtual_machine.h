#ifndef __server_virtual_machine_h__
#define __server_virtual_machine_h__

#include <vector>
#include <string>
#include <stack>
#include <map>
#include "server_object.h"

typedef std::map<int, Slot*> map_slots;
typedef std::vector<Slot*> p_slots;
typedef std::vector<Object*> p_objects;
typedef std::string string;
typedef std::stack<Slot*> stack_slots;
typedef std::vector<int> v_ids;

class VM{
	private:
		map_slots slots;
		stack_slots tmp_slots;
		int id_slots;

	public:
		VM();
		void push_slot(Slot* sl);
		void revert();
		void checkpoint();
		int get_id_slots();
		Slot* immutable_object(Slot* sl);
		string save();
		string get_slots(int id, Slot* sl);
		string get_slot(int id_base, Slot* sl);
		Slot* pop_slot();
                Slot* collect();
		Slot* search_obj(string name);
		Slot* search_obj_id(int id);
		Slot* create_object();
		Slot* create_int(int value);
		Slot* create_float(float value);
		Slot* create_string(string value);
		Slot* create_boolean(bool value);
		Slot* add_slot(Slot* sl_recv, string sl_recv_id, Slot* sl);
		Slot* add_parent(Slot* sl_recv, string sl_recv_id, Slot* sl);
		Slot* add_code(Slot* sl_recv, string sl_recv_id, Slot* sl);
		Slot* rm_slot(Slot* sl_recv, string slot);
		Slot* unary_message(Slot* sl_recv, string msg);
		Slot* binary_message(Slot* sl_recv, string msg, Slot* sl);
		Slot* keyword_message(Slot* sl_recv, string obj_id, Slot* sl);
                Slot* search_obj_by_name(string name, int context);
                string get_slot_to_share(int id);
		~VM();
		
	private:
		Slot* search_and_execute_msg(Slot* sl_recv, string msg, p_objects& args);
		Slot* execute_msg(Slot* msg, Slot* sl_invoker, p_objects& args);
		Slot* search_msg(Slot* sl_recv, string msg);
		void unmark_slots();
		void garbage_collector(v_ids& ids);
		void add_basic_slots(Slot* sl, string name);
		void add_default_name_slot(Slot* sl, string name);
		void add_default_self_slot(Slot* sl);
		void add_default_numeric_slots(Slot* sl_recv);
                Slot* create_garbage_slot(v_ids& ids);
};

#endif
