#ifndef __server_object_h__
#define __server_object_h__

#include <vector>
#include <string>
#include <map>

class Slot;
class VM;
class Object;

typedef std::string string;
typedef std::vector<Slot*> p_slots;
typedef std::multimap<std::string,Slot*> m_slots;
typedef std::vector<Object*> p_objects;

class Object{
	protected:
		m_slots slots;
		bool mark;
		int id;
		
	public:
		Object(int id);
		void add_slot(Slot* sl);
		Slot* rm_slot(string slot);
		Slot* get_slot(string name);
		p_slots get_slots();
		void look_up(string name, p_slots& results);
		void mark_slots();
		void set_mark(bool val);
		string get_json_slots();
		int get_id();
		bool is_mark();
		bool is_base_slot(Slot* sl);
		bool is_num_slot(Slot* sl);
		Slot* as_slot();
		virtual int as_int();
		virtual string as_string();
		virtual Slot* print(VM& vm);
		virtual Slot* clone(VM& vm);
		virtual Slot* execute(VM& vm, p_objects& args);
		virtual void get_self_slots(string& slots_self);
		virtual ~Object();
};

#endif
