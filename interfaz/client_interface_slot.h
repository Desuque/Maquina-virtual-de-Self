#ifndef __client_interface_slot_h__
#define __client_interface_slot_h__

#include <string>

typedef std::string string;

class InterfaceSlot{
  
	private:
		bool type;
		bool code;
		string name;
		string value;
		int id;
        int id_base;
	
	public:
		InterfaceSlot();
		InterfaceSlot(string name, string value, bool code, bool type, int id, int id_base);
		void print_attr();
		string get_name();
		string get_value();
		bool elValorEsPrimitivo(){
			return (this->value != "object");
		}
		int get_id();
		int get_id_base();
		bool has_code();
		bool get_type();
};

#endif
