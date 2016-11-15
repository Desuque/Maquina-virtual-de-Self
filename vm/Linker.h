
#ifndef LINKER_H_
#define LINKER_H_

#include "server_virtual_machine.h"
#include <vector>

class Linker {
private:
	VM* vm;
	std::vector<Slot*> slots;

public:
	Linker();
	void setVM(VM* vm);
	Slot* get_last_slot();
	void create_unary_message(std::string msg);
	void create_unary_message(std::string name, std::string msg);
	void create_binary_message(std::string op);
	void create_keyword_message(std::string obj, std::string lower_key);
	void create_string(std::string string);
	void create_int(std::string number);

	void create_slot(std::string slot);
	Slot* get_object_by_name(std::string name);
	bool remove_slots(std::string context, std::string slot);


private:
	int get_last_created_pos();
	Slot* get_slot(int pos);






public:
	void unary_message(std::string message);
	virtual ~Linker();
};

#endif /* LINKER_H_ */
