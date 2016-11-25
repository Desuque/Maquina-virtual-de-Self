
#ifndef NULL_LINKER_H_
#define NULL_LINKER_H_

#include "server_virtual_machine.h"
#include "server_linker.h"

class nLinker : public Linker {
public:
	nLinker();
	Slot* create_number(std::string number);
	Slot* create_string(std::string string);
	Slot* create_unary_message(Slot* receiver, std::string name);
	Slot* create_binary_message(Slot* receiver, std::string op, Slot* expCP);
	Slot* create_keyword_message(Slot* receiver, std::string lower_or_cap, Slot* expCP);
	Slot* create_slot(Slot* slot, std::string slot_name_extended, std::string op, Slot* exp);
	Slot* create_slot(Slot* object, std::string slot_name_extended);
	Slot* create_nil();
	Slot* create_object();
	Slot* collect();
	Slot* get_object_by_name(std::string name);
	Slot* get_context(std::string name);
	Slot* remove_slots(Slot* slot, std::string slot_to_erase);
	Slot* only_name(std::string name, Slot* slot);
	Slot* create_error(std::string msg_error);
	Slot* set_object_script(Slot* slot, std::string script, std::string msg_slot_name_extended);
	Slot* clone_obj_by_name(std::string receiver);
	virtual ~nLinker();
private:
	Slot* create_float(std::string number);
	Slot* create_int(std::string number);
};

#endif /* NULL_LINKER_H_ */
