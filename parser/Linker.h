
#ifndef LINKER_H_
#define LINKER_H_

#include "virtual_machine.h"
#include <vector>

class Linker {
private:
	VM vm;
	std::vector<Slot*> slots;
	std::vector<std::string> objetos; //Todavia no son objetos, son las palabras clave necesarias para crearlos
	Slot* last_created;

public:
	Linker();
	void create_name(std::string name);
	void create_int(std::string number);
	void create_string(std::string string);
	void set_receiver(std::string receiber);
	void set_lower_keyword(std::string keyword);
	void create_keyword_message(std::string lower_keyword);
	void set_name(std::string name);
	void create_unary_message();


	void create_slot_list();
	void unary_message(std::string message);

	void collect();
	virtual ~Linker();

private:
	void vaciar();
};

#endif /* LINKER_H_ */
