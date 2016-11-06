#ifndef __slot_h__
#define __slot_h__

#include <string>
#include <vector>
#include <utility>
#include "object.h"
#include "error_type.h"

class Object;
typedef std::string string;

class Slot: public Object{
  
	private:
		bool type;
		//bool parent;
		std::pair <bool,string> parent;
		bool code;
		string name;
		Object* value;
	
	public:
		Slot(string slot_name);
		string get_name();
		string parent_name();
		Object* get_value();
		void set_name(string name);
		void set_parent(bool val, string name_parent);
		void set_code(bool val);
		void set_type(bool val);
		void set_obj_value();
		void set_int_value(int value);
		void set_boolean_value(bool value);
		void set_string_value(string value);
		void set_int_method_value(string name);
		void add_slot(Slot* slot);
		bool name_is(string name);
		bool is_parent();
		bool is_code();
		bool is_immutable();
		Slot* execute(VM& vm, p_objects& args);
		virtual bool is_check();
		~Slot();
};

#endif
