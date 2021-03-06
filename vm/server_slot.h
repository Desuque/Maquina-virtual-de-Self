#ifndef __server_slot_h__
#define __server_slot_h__

#include <string>
#include <vector>
#include <tuple>
#include "server_object.h"
#include "server_error_type.h"

class Object;
typedef std::string string;

class Slot: public Object{
  
	private:
		bool type;
		std::vector<string> args;
		std::tuple <bool,string,int> parent;
		bool code;
		string name;
		Object* value;
	
	public:
		Slot(int id, string slot_name);
		string get_name();
		string parent_name();
		Object* get_value();
		void set_name(string name);
		void set_parent(bool val, string name_parent, int id_parent);
		void set_code(bool val);
		void set_type(bool val);
		void set_value(Object* obj);
		void set_obj_value(int id);
                void set_nil_value(int id);
		void set_int_value(int id, int value);
		void set_float_value(int id, float value);
		void set_boolean_value(int id, bool value);
		void set_string_value(int id, string value);
                void set_error_value(int id, string value);
		void set_int_method_value(int id, string name);
		void add_slot(Slot* slot, p_slots& v_sl);
		bool name_is(string name);
                void get_arguments(std::vector<string>& args);
		bool is_parent();
		bool is_code();
		bool is_immutable();
		int get_parent_id();
                void add_argument(string arg);
		virtual string json(int id_base);
		Slot* execute(VM& vm, p_objects& args);
		~Slot();
        
        private:
                bool has_no_ref();
                void valid_mutable();
};

#endif
