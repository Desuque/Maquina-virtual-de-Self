#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include "server_parser.h"
#include "server_linker.h"
#include "server_null_linker.h"
#include "server_virtual_machine.h"

#define NOT_SET -1
#define ADD_SLOTS 3
#define REMOVE_SLOTS 4
#define COLLECT 14
#define ERROR 0

Parser::Parser() : vm(NULL) {
	msg = "";
	flag = NOT_SET;
}

void Parser::setVM(VM* vm) {
	this->vm = vm;
}

bool Parser::number(std::stringstream* codigo, int* posicion, Slot** slot) {
	codigo->clear();
	int posicionOriginal = *posicion;

	int valor;
	int decimales = 0;
	bool its_num = false;
	bool its_float = false;
	codigo->seekg(*posicion, std::ios::beg);
	*codigo>>valor;

	if(codigo->fail()) {
		codigo->clear();
		*posicion = posicionOriginal;
	} else {
		char c;
		int posAux = codigo->tellg();
		its_num = true;
		codigo->get(c);
		if(c != '.') {
			//Si no hay un punto, estoy seguro de que es un int
			codigo->unget();
			*posicion = codigo->tellg();
		} else {
			//Puede ser o no un float
			*codigo>>decimales;
			if(codigo->fail()) {
				//Despues del punto no hay numeros, no era un float
				codigo->clear();
				*posicion = posAux;
			} else {
				//Era un float
				its_float = true;
				*posicion = codigo->tellg();
			}
		}
	}

	std::stringstream ss;
	if(its_float) {
		ss << valor << "." << decimales;
	} else {
		ss << valor;
	}
	std::string str = ss.str();

	if(its_num) {
		*slot = linker->create_number(str);
		return true;
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::text(std::stringstream* codigo, int* posicion, Slot** slot) {
	int posicionOriginal = *posicion;
	//Elimino posibles espacios
	erase_white_spaces(codigo, posicion);
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);

	char c;
	bool found_text = false;
	std::string auxiliar;
	std::string c_str;
	codigo->get(c);
	auxiliar = c;
	if(auxiliar == "'") {
		while(codigo->get(c)) {
			auxiliar += c;
			*posicion = codigo->tellg();

			c_str = c;
			if(c_str == "'") {
				found_text = true;
				break;
			}
		}
	}

	if(found_text) {
		*slot = linker->create_string(auxiliar);
		std::cout<<"El string de texto se crea y existe!"<<std::endl;
		return true;
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::pipe_with_script(std::stringstream* codigo, int* posicion, Slot** slot) {
	codigo->clear();
	int posicionOriginal = *posicion;
	//Elimino posibles espacios
	erase_white_spaces(codigo, posicion);
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);

	char c;
	std::string msg_script;
	codigo->get(c);
	if(c == '|') {
		int posInicio = codigo->tellg();
		int posSalida;

		//Elimino posibles espacios
		erase_white_spaces(codigo, posicion);
		*posicion = codigo->tellg();
		codigo->seekg(*posicion, std::ios::beg);

		//Null parser para evitar que se ejecute codigo
		std::cout<<"Antes de crear el parser null"<<std::endl;
		Parser null_parser;
		nLinker n_linker;
		null_parser.set_linker(&n_linker);
		std::cout<<"Fin de creaciones"<<std::endl;
		if(null_parser.script(codigo, posicion)) {
			posSalida = *posicion;
			codigo->seekg(posInicio, std::ios::beg);
			for(unsigned int i = posInicio; i<posSalida; i++) {
				codigo->get(c);
				msg_script += c;
			}
			std::cout<<"Esta es la posicion de salida: "<<*posicion<<" y el tellg: "<<codigo->tellg()<<std::endl;
			std::cout<<"Este es el script ingresado: "<<msg_script<<std::endl;
			*posicion = codigo->tellg();
			//*slot = linker->set_object_script(*slot, script);
			return true;		
		} else {
			//Si no hay coincidencia, vuelvo el puntero a su posicion original
			*posicion = posicionOriginal;
			return false;
		}
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::pipe_without_script(std::stringstream* codigo, int* posicion) {
	codigo->clear();
	int posicionOriginal = *posicion;
	//Elimino posibles espacios
	erase_white_spaces(codigo, posicion);
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);

	char c;
	codigo->get(c);
	if(c == '|') {
		*posicion = codigo->tellg();
		//Elimino posibles espacios
		erase_white_spaces(codigo, posicion);
		codigo->seekg(*posicion, std::ios::beg);
		codigo->get(c);
		if(c == ')') {
			*posicion = codigo->tellg();
			return true;
		}
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::object_end(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;

	//Elimino posibles espacios
	erase_white_spaces(codigo, posicion);
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);

	char c;
	codigo->get(c);
	if(c == '|') {
		*posicion = codigo->tellg();

		//Elimino posibles espacios
		erase_white_spaces(codigo, posicion);

		codigo->seekg(*posicion, std::ios::beg);
		codigo->get(c);
		if(c == ')') {
			*posicion = codigo->tellg();
			return true;
		}
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;

}

bool Parser::object_intro(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;

	//Elimino posibles espacios
	erase_white_spaces(codigo, posicion);
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);

	char c;
	codigo->get(c);
	if(c == '(') {
		*posicion = codigo->tellg();

		//Elimino posibles espacios
		erase_white_spaces(codigo, posicion);

		codigo->seekg(*posicion, std::ios::beg);
		codigo->get(c);
		if(c == '|') {
			*posicion = codigo->tellg();
			return true;
		}
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

Slot* Parser::process_slot_list(Slot* object, std::string slot_name_extended, std::string op, Slot* exp) {
	return linker->create_slot(object, slot_name_extended, op, exp);
}

Slot* Parser::process_slot_list(Slot* object, std::string slot_name_extended) {
	return linker->create_slot(object, slot_name_extended);	
}

bool Parser::slot_list(std::stringstream* codigo, int* posicion, Slot** slot) {
	int posicionOriginal = *posicion;

	Slot* exp = NULL;

	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	std::string valor;

	if(slot_name_extended(codigo, posicion)) {
		std::string slot_name_extended = get_msg();

		//Elimino posibles espacios
		erase_white_spaces(codigo, posicion);
		//Leo todo el valor desde la posicion indicada
		codigo->seekg(*posicion, std::ios::beg);
		char c;
		codigo->get(c);

		//Si un slot no se lo inicializa por default Self crea 
		//un slot mutable inicializado con el objeto ​nil
		if(c == '.') {
			*slot = process_slot_list(*slot, slot_name_extended);
			*posicion = codigo->tellg();
			return true;
		}
		if(slot_operator(codigo, posicion)) {
			std::string op = get_op();
			if(expression(codigo, posicion, &exp)) {
				if(final(codigo, posicion)) {
					*slot = process_slot_list(*slot, slot_name_extended, op, exp);
					*posicion = codigo->tellg();
					return true;
				}
			}
		} //TODO AGREGAR * Y :
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::object(std::stringstream* codigo, int* posicion, Slot** slot) {
	codigo->clear();
	int posicionOriginal = *posicion;

	Slot* object = linker->create_object();
	Slot* script = NULL;

	std::cout<<"Vemos que trae el flag de msg: "<<get_msg()<<std::endl;

	//Compruebo que efectivamente comienza con "(|"
	if(object_intro(codigo, posicion)) {
		
		if(slot_list(codigo, posicion, &object) || empty(codigo, posicion)) {
			//Si ya se cargo al menos un slot_list, busco otros posibles slots_lists
			//En caso de no haber, se sigue parseando el objeto
			bool nextSlot = true;
			
			//ESTE WHILE NO ME DEJA SALIR SI NO HAY SLOTS!!!!!!! REVISAR!!!

			while(nextSlot) {
				nextSlot = slot_list(codigo, posicion, &object);
			}
			if (pipe_without_script(codigo, posicion)) {
				//Quiere decir que se encontro una barra y un parentesis
				*slot = object;
				return true;
			}
			if (pipe_with_script(codigo, posicion, &object)) {
				std::cout<<"Entro al pipe with script"<<std::endl;
				//Elimino posibles espacios
				erase_white_spaces(codigo, posicion);
				//Leo todo el valor desde la posicion indicada
				codigo->seekg(*posicion, std::ios::beg);

				char c;
				codigo->get(c);
				if(c == ')') {
					*posicion = codigo->tellg();
					*slot = object;
					return true;
				}
			}
		}
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

Slot* Parser::process_nil() {
	return vm->create_nil();
}

bool Parser::nil(std::stringstream* codigo, int* posicion, Slot** slot) {
	codigo->clear();
	int posicionOriginal = *posicion;

	//Elimino posibles espacios
	erase_white_spaces(codigo, posicion);
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);

	char c;
	std::string auxiliar;
	while(codigo->get(c)) {
		if ((c == 'n') || (c == 'i') || (c == 'l')) {
			auxiliar += c;
			*posicion = codigo->tellg();
		} else {
			break;
		}
	}

	if(auxiliar == "nil") {
		*slot = process_nil();
		return true;
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

void Parser::erase_white_spaces(std::stringstream* codigo, int* posicion) {
	codigo->seekg(*posicion, std::ios::beg);
	char c;
	while( codigo->get(c) && isspace(c) ) {
		*posicion = codigo->tellg();
	}
}

bool Parser::name(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;
	//Elimino posibles espacios
	erase_white_spaces(codigo, posicion);
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);

	char c;
	std::string auxiliar;
	while(codigo->get(c)) {
		if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) ||
				((c >= '0') && (c <= '9'))) {
			auxiliar += c;
			*posicion = codigo->tellg();
		} else {
			break;
		}
	}

	//Cualquier palabra alfanumerica que empiece con una letra en minusculas
	std::regex rr(R"([a-z][A-Z-a-z-0-9]*)");
	if(regex_match(auxiliar,rr)) {
		set_msg(auxiliar);
		return true;
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::slot_name_extended(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;
	//Elimino posibles espacios
	erase_white_spaces(codigo, posicion);
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);

	char c;
	codigo->get(c);
	
	//No se van a implementar argumentos por el momento
	if(c == ':') {
		*posicion = codigo->tellg();
		if(name(codigo, posicion)) {
			std::cout<<"Cumple con el argumento!"<<std::endl;
			return true;
		}
	}

	//Si no se cumple retorno a la posicion inicial
	*posicion = posicionOriginal;

	//Compruebo si se trata de un parent
	if(name(codigo, posicion)) {
		//Elimino posibles espacios
		erase_white_spaces(codigo, posicion);
		//Leo todo el valor desde la posicion indicada
		codigo->seekg(*posicion, std::ios::beg);

		char c;
		codigo->get(c);
		if(c == '*') {
			std::cout<<"Cumplo con el parent"<<std::endl;
			*posicion = codigo->tellg();
			return true;
		}
	}

	//Si no se cumple retorno a la posicion inicial
	*posicion = posicionOriginal;

	//Compruebo si se trata de un name normal
	if(name(codigo, posicion)) {
		std::cout<<"Era un name normal"<<std::endl;
		return true;
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

/**
 * Este metodo no modifica la posicion del puntero en el codigo ya que
 * solo se quiere comprobar que se trata de un objeto sin slots
 */
bool Parser::empty(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;
	//Elimino posibles espacios
	erase_white_spaces(codigo, posicion);
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);

	char c;
	codigo->get(c);

	if(c == '|') {
		*posicion = posicionOriginal;
		return true;
	} else {
		*posicion = posicionOriginal;
		return false;
	}
}

bool Parser::slot_operator(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;
	//Elimino posibles espacios
	erase_white_spaces(codigo, posicion);
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);

	char c;
	std::string valor;
	while(codigo->get(c)) {
		if ((c == '=')|| (c == '<') || (c == '-')) {
			valor += c;
			*posicion = codigo->tellg();
		} else {
			break;
		}
	}

	bool itsOperator = false;

	if(valor == "=") {
		itsOperator = true;
	}
	if(valor == "<-") {
		itsOperator = true;
	}

	if(itsOperator) {
		set_op(valor);
		return true;
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::expressionP(std::stringstream* codigo, int* posicion, Slot** slot) {
	int posicionOriginal = *posicion;
	//Elimino posibles espacios
	erase_white_spaces(codigo, posicion);
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);

	char c;
	codigo->get(c);

	if(c == '(') {
		*posicion = codigo->tellg();
		codigo->seekg(*posicion, std::ios::beg);
		if(expression(codigo, posicion, slot)) {
			//Elimino posibles espacios
			erase_white_spaces(codigo, posicion);
			codigo->seekg(*posicion, std::ios::beg);
			codigo->get(c);
			if(c == ')') {
				*posicion = codigo->tellg();
				return true;
			}
		}
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::constant(std::stringstream* codigo, int* posicion, Slot** slot) {
	codigo->clear();
	int posicionOriginal = *posicion;
	
	if(number(codigo, posicion, slot)) {
		return true;
	}

	if(text(codigo, posicion, slot)) {
		return true;
	}

	if(object(codigo, posicion, slot)) {
		return true;
	}
	if(nil(codigo, posicion, slot)) {
		return true;
	}
	if(name(codigo, posicion)) {
		return true;
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::receiver(std::stringstream* codigo, int* posicion, Slot** slot) {
	int posicionOriginal = *posicion;

	if(expressionCP(codigo, posicion, slot)) {
		return true;
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::lower_keyword(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;
	//Elimino posibles espacios
	erase_white_spaces(codigo, posicion);
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);

	char c;
	std::string auxiliar;
	while(codigo->get(c)) {
		if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) ||
				((c >= '0') && (c <= '9')) || (c == '_')) {
			auxiliar += c;
			*posicion = codigo->tellg();
		} else {
			break;
		}
	}

	//Cualquier palabra alfanumerica que empiece con una letra en minusculas
	std::regex rr(R"((\_|[a-z])[A-Z-a-z-0-9]*)");
	if(regex_match(auxiliar,rr)) {
		set_msg(auxiliar);
		return true;
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::cap_keyword(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;
	//Elimino posibles espacios
	erase_white_spaces(codigo, posicion);
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);

	char c;
	std::string auxiliar;
	while(codigo->get(c)) {
		if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) ||
				((c >= '0') && (c <= '9'))) {
			auxiliar += c;
			*posicion = codigo->tellg();
		} else {
			break;
		}
	}

	//Cualquier palabra alfanumerica que empiece con una letra en minusculas
	std::regex rr(R"(([A-Z])[A-Z-a-z-0-9]*)");
	if(regex_match(auxiliar,rr)) {
		set_msg(auxiliar);
		return true;
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::operador(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;
	//Elimino posibles espacios
	erase_white_spaces(codigo, posicion);
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);

	char c;
	std::string valor;
	while(codigo->get(c)) {
		if ((c == '+')|| (c == '-') || (c == '*') ||
				(c == '/') || (c == '!') || (c == '=')) {
			valor += c;
			*posicion = codigo->tellg();
		} else {
			break;
		}
	}

	bool itsOperator = false;

	if(valor == "+") {
		itsOperator = true;
	}
	if(valor == "-") {
		itsOperator = true;
	}
	if(valor == "*") {
		itsOperator = true;
	}
	if(valor == "/") {
		itsOperator = true;
	}
	if(valor == "!=") {
		itsOperator = true;
	}
	if(valor == "==") {
		itsOperator = true;
	}

	if(itsOperator) {
		set_op(valor);
		return true;
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::remove_slots(std::stringstream* codigo, int* posicion, Slot** slot) {
	int posicionOriginal = *posicion;
	bool remove = true;
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);

	if(object_intro(codigo, posicion)) {
		while(remove) {
			if(name(codigo, posicion)) {
				if(final(codigo, posicion)) {
					std::string msg_slot_to_erase = get_msg();
					if(msg_slot_to_erase.size() != 0) {
						*slot = linker->remove_slots(*slot, msg_slot_to_erase);	
					} else {
						remove = false;
					}
				} else {
					break;
				}
			} else {
				break;
			}
		}
		if(object_end(codigo, posicion)) {
			return true;
		}
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

Slot* Parser::process_keyword_message(Slot* receiver, std::string lower_or_cap, Slot* expCP) {
	//if((receiver != NULL) && (expCP != NULL)) {
		return linker->create_keyword_message(receiver, lower_or_cap, expCP);
	//}
	//return NULL;
}

bool Parser::keyword_message(std::stringstream* codigo, int* posicion, Slot** slot) {
	codigo->clear();
	int posicionOriginal = *posicion;

	Slot* slot_receiver = NULL;
	Slot* slot_expCP = NULL;

	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	std::string valor;

	if(receiver(codigo, posicion, &slot_receiver)) {
		//El receiver siempre es el contexto en un keyword_message, por ende
		//obtengo el slot correspondiente usando el name cargado
		std::string msg_receiver = get_msg();
		if(msg_receiver.size() != 0) {
			slot_receiver = linker->get_context(msg_receiver);
		}

		std::string msg = get_msg();
		if(lower_keyword(codigo, posicion)) {
			std::string lower_key = get_msg();
			
			//Elimino posibles espacios
			erase_white_spaces(codigo, posicion);
			//Leo todo el valor desde la posicion indicada
			codigo->seekg(*posicion, std::ios::beg);
			char c;
			codigo->get(c);

			if(c == ':') {
				*posicion = codigo->tellg();
				codigo->seekg(*posicion, std::ios::beg);
				*codigo>>valor;

				if(lower_key == "_RemoveSlots") {
					std::cout<<"Entro aca!"<<std::endl;
					if(remove_slots(codigo, posicion, &slot_expCP)) {
						*slot = process_keyword_message(slot_receiver, lower_key, slot_expCP);
						setFlag(lower_key);
						return true;
					} else {
						//Si no hay coincidencia, vuelvo el puntero a su posicion original
						*posicion = posicionOriginal;
						return false;
					}

				//Tiene en cuenta _AddSlots: como cualquier metodo de usuario
				} else {
				//if(lower_key == "_AddSlots") {
					if(expressionCP(codigo, posicion, &slot_expCP)) {
						*slot = process_keyword_message(slot_receiver, lower_key, slot_expCP);
						setFlag(lower_key);
				
						/**
						 * Puede haber o no cap_keyword
												 
						bool isCap_keyword = true;
						while(isCap_keyword) {
							if(cap_keyword(codigo, posicion)) {
								std::string cap_key = get_msg();
								//Elimino posibles espacios en blanco
								erase_white_spaces(codigo, posicion);

								char c;
								codigo->seekg(*posicion, std::ios::beg);
								codigo->get(c);
								if(c == ':') {
									*posicion = codigo->tellg();
									if(expressionCP(codigo, posicion, &slot_expCP)) {
										*slot = process_keyword_message(slot_receiver, lower_key, slot_expCP);
										isCap_keyword = true;
									}
								}
							} else {
								isCap_keyword = false;
							}
						}

						
						 * Fin carga de cap_keyword
						 */
						return true;
						
					}
				}
			}
		}
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	*slot = NULL;
	clean_flag();
	return false;
}

Slot* Parser::process_binary_message(Slot* receiver, std::string op, Slot* expCP) {
	//if((receiver != NULL) && (expCP != NULL)) {
		return linker->create_binary_message(receiver, op, expCP);
	//}
	//return NULL;
}

bool Parser::binary_message(std::stringstream* codigo, int* posicion, Slot** slot) {
	int posicionOriginal = *posicion;

	Slot* slot_receiver = NULL;
	Slot* slot_expCP = NULL;

	if(receiver(codigo, posicion, &slot_receiver)) {
		//Si el receiver era un name, queda guardado en la variable msg_name
		//Busco el slot correspondiente a ese name
		std::string msg_receiver = get_msg();
		if(msg_receiver.size() != 0) {
			slot_receiver = linker->get_object_by_name(msg_receiver);
		}
		if(operador(codigo, posicion)) {
			std::string op = get_op();
			if(expressionCP(codigo, posicion, &slot_expCP)) {
				//Si la expressionCP era un name, queda guardado en la variable msg_name
				//Busco el slot correspondiente a ese name
				std::string msg_expCP = get_msg();
				if(msg_expCP.size() != 0) {
					slot_expCP = linker->get_object_by_name(msg_expCP);
				}
				*slot = linker->create_binary_message(slot_receiver, op, slot_expCP);
				return true;
			}
		}
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

Slot* Parser::process_unary_message(Slot* receiver, std::string name) {
	//if(receiver != NULL) {
		return linker->create_unary_message(receiver, name);
	//}
	//return NULL;
}

bool Parser::unary_message(std::stringstream* codigo, int* posicion, Slot** slot) {
	int posicionOriginal = *posicion;
	bool valid_search;
	
	Slot* slot_receiver = NULL;

	if(receiver(codigo, posicion, &slot_receiver)) {
		//Si el receiver era un name, queda guardado en la variable msg_name
		//Busco el slot correspondiente a ese name
		std::string msg_receiver = get_msg();
		if(msg_receiver.size() != 0) {
			slot_receiver =  linker->get_object_by_name(msg_receiver);
		}

		if (name(codigo, posicion)) {
			codigo->seekg(*posicion, std::ios::beg);
			std::string msg_name = get_msg();
			
			if((msg_receiver == "lobby") && (msg_name == "collect")) {
				int posAux = codigo->tellg();
				//Elimino posibles espacios
				erase_white_spaces(codigo, &posAux);
				//Leo todo el valor desde la posicion indicada
				codigo->seekg(posAux, std::ios::beg);
				char c;
				codigo->get(c);
				if (c == '.') {
					*slot = linker->collect();
					setFlag(msg_name);
				}
			} else {
				//Actualizo la referencia al slot final creado
				*slot = process_unary_message(slot_receiver, msg_name);
			}
			return true;
		}
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::only_name(std::stringstream* codigo, int* posicion, Slot** slot) {
	int posicionOriginal = *posicion;
	std::cout<<"Entro"<<std::endl;
	if (name(codigo, posicion)) {
		std::cout<<"Aca tambien"<<std::endl;
		std::string msg_name = get_msg();
		if(msg_name.size() != 0) {
			std::cout<<"Entro al linker trucho"<<std::endl;
			std::cout<<"Memoria del linker trucho: "<<linker<<std::endl;
			*slot = linker->only_name(msg_name, *slot);	
			std::cout<<"Salgo del linker trucho"<<std::endl;
			return true;
		}
	}
	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::expressionCP(std::stringstream* codigo, int* posicion, Slot** slot) {
	int posicionOriginal = *posicion;

	if (expressionP(codigo, posicion, slot)) {
		return true;
	}
	if (constant(codigo, posicion, slot)) {
		return true;
	}
	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::expression(std::stringstream* codigo, int* posicion, Slot** slot) {
	int posicionOriginal = *posicion;
	
	if(keyword_message(codigo, posicion, slot)) {
		return true;
	}
	if(binary_message(codigo, posicion, slot)) {
		return true;
	}
	if(unary_message(codigo, posicion, slot)) {
		return true;
	}
	if(expressionCP(codigo, posicion, slot)) {
		return true;
	}
	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::final(std::stringstream *codigo, int* posicion) {
	int posicionOriginal = *posicion;
	//Elimino posibles espacios
	erase_white_spaces(codigo, posicion);
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	char c;
	codigo->get(c);
	if(c == '.') {
		*posicion = codigo->tellg();
		return true;
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::script(std::stringstream *codigo, int* posicion) {
	//Por cada script, se devuelve un Slot
	Slot* slot = NULL;
	int posicionOriginal = *posicion;
	std::cout<<"Paso por aca"<<std::endl;
	if(only_name(codigo, posicion, &slot)) {
		std::cout<<"ENtro"<<std::endl;
		if(final(codigo, posicion)) {
			std::cout<<"Paso el final"<<std::endl;
			slots_to_process.push_back(slot);
			if (!codigo->eof()) {
				script(codigo, posicion);
			}
			return true;
		} else {
			*posicion = posicionOriginal;
		}
	} else {
		*posicion = posicionOriginal;
	}
	std::cout<<"Y por Paso por aca"<<std::endl;

	if(expression(codigo, posicion, &slot)) {
		if(final(codigo, posicion)) {
			codigo->seekg(*posicion, std::ios::beg);
			std::string valor;
			*codigo>>valor;
			//Guardo el slot a retornar a la VM en la lista de slots procesados
			slots_to_process.push_back(slot);
			if (!codigo->eof()) {
				script(codigo, posicion);
			}
			return true;
		}
	}
	return false;
}

Slot* Parser::process_error(std::string msg_error) {
	return linker->create_error(msg_error);
}

Slot* Parser::parsear(std::string codigo) {
	std::stringstream scripts(codigo);
	int posicion = scripts.tellg();
	scripts.seekp(posicion, std::ios::beg);

	//Primero se parsea todo el algoritmo utilizando un null parser para evitar modificar la VM
	Parser null_parser;
	nLinker n_linker;
	null_parser.set_linker(&n_linker);
	int posAux = posicion;
	
	if(null_parser.script(&scripts, &posAux)) {
		//Si todas las instrucciones son validas, procedemos a parsear creando los
		//objetos en la VM
		script(&scripts, &posicion);
		std::cout<<"Se completa el script papa!"<<std::endl;
		Slot* ret = slots_to_process.at(slots_to_process.size()-1);
		std::cout<<"Algo pasa aca?"<<std::endl;
		return ret; //HARDCODEADO, DEVUELVE SIEMPRE EL PRIMERO!!!		
	} else {
		std::cout<<"No es un script dog, lo siento"<<std::endl;
		Slot* error = process_error("Sintax error.");
		setFlag("Error");
		return NULL;
	}
}
/**
bool Parser::null_parser(std::string codigo, std::string id) {
	std::cout<<"Entre"<<std::endl;
	Parser null_parser;
	null_parser.parsear(codigo);
	return true;
}
**/

void Parser::set_linker(Linker* linker) {
	this->linker = linker;
}

Slot* Parser::parsear(std::string codigo, std::string id) {
	//if(null_parser(codigo, id)) {

	//Seteo el linker a usar
	Linker linker;
	set_linker(&linker);

	//Seteo el ID del contexto donde se va a trabajar
	linker.setID(id);
	linker.setVM(vm);
	//Parseo como un script comun
	return parsear(codigo);
	//}
}

void Parser::set_msg(std::string msg) {
	this->msg = msg;
}

std::string Parser::get_msg() {
	std::string aux = msg;
	this->msg = "";
	return aux;
}

void Parser::set_op(std::string op) {
	this->op = op;
}

std::string Parser::get_op() {
	return op;
}

void Parser::setFlag(std::string valor) {
	if(valor == "_AddSlots") {
		flag = ADD_SLOTS;
	} else if (valor == "_RemoveSlots") {
		flag = REMOVE_SLOTS;
	} else if (valor == "Error") {
		flag = ERROR;
	} else if (valor == "collect") {
		flag = COLLECT;
	} else {
		flag = NOT_SET;
	}
}

void Parser::clean_flag() {
	flag = NOT_SET;
}

int Parser::getFlag() {
	return flag;
}

Parser::~Parser() {
}
