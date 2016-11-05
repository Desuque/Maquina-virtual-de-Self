#ifndef __interface_slot_h__
#define __interface_slot_h__

#include <string>

typedef std::string string;

class InterfaceSlot{
  
	private:
		bool type;
		bool code;
		string name;
		string value;
	
	public:
		InterfaceSlot();
		InterfaceSlot(string name, string value, bool code, bool type);
		void print_attr();
};

#endif
