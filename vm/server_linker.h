
#ifndef LINKER_H_
#define LINKER_H_

#include "server_virtual_machine.h"

class Linker {
private:
	VM* vm;
	std::string id;

public:
	Linker();
	void setVM(VM* vm);
	void setID(std::string id);

	virtual Slot* create_number(std::string number);
	virtual Slot* create_string(std::string string);
	virtual Slot* create_unary_message(Slot* receiver, std::string name);
	virtual Slot* create_binary_message(Slot* receiver, std::string op, Slot* expCP);
	virtual Slot* create_keyword_message(Slot* receiver, std::string lower_or_cap, Slot* expCP, bool addSlots);
	virtual Slot* create_keyword_message(std::string lower_or_cap, Slot* expCP);
	virtual Slot* create_slot(Slot* object, std::string slot_name_extended, std::string op, Slot* exp, bool code_flag);
	virtual Slot* create_slot(Slot* object, std::string slot_name_extended);
	virtual Slot* create_parent_slot(Slot* object, std::string parent, std::string son);
	virtual Slot* create_nil();
	virtual Slot* create_object();
	virtual Slot* collect();
	virtual Slot* get_object_by_name(std::string name);
	virtual Slot* get_context(std::string name);
	virtual Slot* remove_slots(Slot* slot, std::string slot_to_erase);
	virtual Slot* only_name(std::string name, Slot* slot);
	virtual Slot* create_error(std::string msg_error);
	virtual Slot* set_object_script(Slot* slot, std::string script, std::string msg_slot_name_extended, std::vector<std::string> args);
	virtual Slot* create_user_method(Slot *method, Slot* expCP);
	virtual Slot* clone_obj_by_name(std::string receiver);

	virtual ~Linker();
private:
	virtual Slot* create_float(std::string number);
	virtual Slot* create_int(std::string number);
};

#endif /* LINKER_H_ */
