
#ifndef LINKER_H_
#define LINKER_H_

#include "server_virtual_machine.h"
#include <vector>

class Linker {
private:
	VM* vm;
	std::string id;

public:
	Linker();
	void setVM(VM* vm);
	void setID(std::string id);

	Slot* create_number(std::string number);
	Slot* create_string(std::string string);
	Slot* create_unary_message(Slot* receiver, std::string name);
	Slot* create_binary_message(Slot* receiver, std::string op, Slot* expCP);
	Slot* create_keyword_message(Slot* receiver, std::string lower_or_cap, Slot* expCP);
	Slot* create_slot(Slot* slot, std::string slot_name_extended, std::string op, Slot* exp);
	Slot* create_object();

	Slot* get_object_by_name(std::string name);
	bool remove_slots(std::string context, std::string slot);
	virtual ~Linker();
	
private:
	bool collect(std::string obj, std::string msg);
	Slot* create_float(std::string number);
	Slot* create_int(std::string number);

};

#endif /* LINKER_H_ */
