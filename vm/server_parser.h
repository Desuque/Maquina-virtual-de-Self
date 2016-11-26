
#ifndef PARSER_H_
#define PARSER_H_

#include "server_linker.h"
#include "server_null_linker.h"
#include "server_virtual_machine.h"
#include <vector>

class Parser {
private:
	VM* vm;
	Linker* linker;
	std::string msg;
	std::string op;
	std::string script_name;
	bool code_flag;
	int flag;
	std::vector<Slot*> slots_to_process;

public:
	Parser();
	void setVM(VM* vm);
	int getFlag();
	std::vector<Slot*> parsear(std::string codigo, std::string id, std::vector<int>& flags);
	virtual ~Parser();
private:
	void set_linker(Linker* linker);
	std::vector<Slot*> parsear(std::string codigo, std::vector<int>& flags);
	bool script(std::stringstream* codigo, int* posicion, std::vector<int>& flags);
	bool expression(std::stringstream* codigo, int* posicion, Slot** slot);
	Slot* process_unary_message(Slot* receiver, std::string name);
	Slot* process_binary_message(Slot* receiver, std::string op, Slot* expCP);
	Slot* process_keyword_message(Slot* receiver, std::string lower_or_cap, Slot* expCP);
	Slot* process_slot_list(Slot* object, std::string slot_name_extended, std::string op, Slot* exp);
	Slot* process_slot_list(Slot* object, std::string slot_name_extended);
	Slot* process_nil();
	Slot* process_error(std::string msg_error);
	Slot* process_clone_object(std::string receiver);

	bool unary_message(std::stringstream* codigo, int* posicion, Slot** slot);
	bool binary_message(std::stringstream* codigo, int* posicion, Slot** slot);
	bool keyword_message(std::stringstream* codigo, int* posicion, Slot** slot);
	bool expressionCP(std::stringstream* codigo, int* posicion, Slot** slot);
	bool receiver(std::stringstream* codigo, int* posicion, Slot** slot);
	bool constant(std::stringstream* codigo, int* posicion, Slot** slot);
	bool only_name(std::stringstream* codigo, int* posicion, Slot** slot);
	bool expressionP(std::stringstream* codigo, int* posicion, Slot** slot);
	bool number(std::stringstream* codigo, int* posicion, Slot** slot);
	bool text(std::stringstream* codigo, int* posicion, Slot** slot);
	bool object_intro(std::stringstream* codigo, int* posicion);
	bool object_end(std::stringstream* codigo, int* posicion);
	bool object(std::stringstream* codigo, int* posicion, Slot** slot);
	bool slot_operator(std::stringstream* codigo, int* posicion);
	bool slot_list(std::stringstream* codigo, int* posicion, Slot** slot);
	bool slot_name_extended(std::stringstream* codigo, int* posicion);
	bool nil(std::stringstream* codigo, int* posicion, Slot** slot);
	bool name(std::stringstream* codigo, int* posicion);
	bool operador(std::stringstream* codigo, int* posicion);
	bool lower_keyword(std::stringstream* codigo, int* posicion);
	bool cap_keyword(std::stringstream* codigo, int* posicion);
	bool final(std::stringstream *codigo, int* posicion);
	bool empty(std::stringstream* codigo, int* posicion);
	bool pipe_without_script(std::stringstream* codigo, int* posicion);
	bool pipe_with_script(std::stringstream* codigo, int* posicion, Slot** slot, std::string msg_slot_name_extended);
	bool remove_slots(std::stringstream* codigo, int* posicion, Slot** slot);
	void setFlag(std::string valor);
	void set_msg(std::string msg);
	std::string get_msg();
	void set_op(std::string op);
	std::string get_op();
	void set_script_name(std::string slot_name_ext);
	std::string get_script_name();
	void set_code_flag(bool code_flag);
	bool get_code_flag();

	void erase_white_spaces(std::stringstream* codigo, int* posicion);
	
	void clean_flag();
};

#endif /* PARSER_H_ */
