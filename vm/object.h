#ifndef __object_h__
#define __object_h__

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
		
	public:
		void add_slot(Slot* sl);
		Slot* rm_slot(string slot);
		Slot* get_slot(string name);
		void look_up(string name, p_slots& results);
		virtual int as_int();
		virtual string as_string();
		virtual Slot* print(VM& vm);
		virtual Slot* clone(VM& vm);
		virtual Slot* execute(VM& vm, p_objects& args);
		virtual ~Object();
	
	private:
		bool is_base_slot(Slot* sl);
};

#endif
