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
typedef std::vector<int> v_ids;
typedef std::vector<string> strings;

class VM{
	private:
		map_slots slots;
		int id_slots;

	public:
		VM();
		int get_id_slots();
		Slot* immutable_object(Slot* sl);
		string save();
		string get_slots(int id, Slot* sl);
		string get_slot(int id_base, Slot* sl);
                Slot* collect();
		Slot* search_obj(string name);
		Slot* search_obj_id(int id);
		Slot* create_object();
		Slot* create_nil();
                Slot* create_error(string msg);
                Slot* create_int(int value);
		Slot* create_float(float value);
		Slot* create_string(string value);
		Slot* create_boolean(bool value);
		Slot* add_slot(Slot* sl_recv, string sl_recv_id, Slot* sl);
		Slot* add_parent(Slot* sl_recv, string sl_recv_id, Slot* sl);
		Slot* add_argument(Slot* sl_recv, string sl_recv_id, Slot* sl);
		Slot* add_code(Slot* sl_recv, string sl_recv_id, Slot* sl);
		Slot* rm_slot(Slot* sl_recv, string slot);
		Slot* unary_message(Slot* sl_recv, string msg);
		Slot* binary_message(Slot* sl_recv, string msg, Slot* sl);
		Slot* keyword_message(Slot* sl_recv, string obj_id, Slot* sl);
                Slot* search_obj_by_name(string name, int context);
                Slot* clone_obj_by_name(string name, int context);
                string get_slot_to_share(int id);
		string get_json_id(int id);
		~VM();
		
	private:
                Slot* key_add_slots(Slot* sl_recv, Slot* sl);
                Slot* key_rm_slots(Slot* sl_recv, Slot* sl);
                Slot* key_other_msg(Slot* sl_recv, Slot* sl, string msg);
                void keyword_args(Slot* sl_msg, Object* obj);
                Slot* add_parent_argument(Slot* sl_recv, string sl_recv_id, Slot* sl);
		Slot* search_and_execute_msg(Slot* sl_recv, string msg, p_objects& args, bool keyword);
		Slot* execute_msg(Slot* msg, Slot* sl_invoker, p_objects& args);
                Slot* execute_user_msg(Slot* sl_recv, Slot* sl_msg, Object* obj, bool keyword);
		Slot* search_msg(Slot* sl_recv, string msg);
                string get_json_container(Slot* sl, int id_base);
                string get_json_obj(Slot* sl, int id_base);
		void unmark_slots();
		void garbage_collector(v_ids& ids);
		void add_basic_slots(Slot* sl, string name);
		void add_default_name_slot(Slot* sl, string name);
		void add_default_self_slot(Slot* sl);
		void add_default_numeric_slots(Slot* sl_recv);
                Slot* create_garbage_slot(v_ids& ids);
};

#endif
