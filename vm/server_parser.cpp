#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include "server_parser.h"
#include "server_linker.h"
#include "server_virtual_machine.h"

#define NOT_SET -1
#define ADD_SLOTS 3
#define REMOVE_SLOTS 4
#define ERROR 0

Parser::Parser() : vm(NULL) {
	msg = "";
	flag = NOT_SET;
}

void Parser::setVM(VM* vm) {
	this->vm = vm;
	linker.setVM(vm);
}

bool Parser::number(std::stringstream* codigo, int* posicion) {
	codigo->clear();
	int posicionOriginal = *posicion;

	float valor;
	codigo->seekg(*posicion, std::ios::beg);
	*codigo>>valor;

	if(codigo->fail()){
		codigo->clear();
		*posicion = posicionOriginal;
		return false;
	} else {
		*posicion = codigo->tellg();
		std::stringstream ss;
		ss << valor;
		std::string str = ss.str();
		linker.create_number(str);
		return true;
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::text(std::stringstream* codigo, int* posicion) {
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
		linker.create_string(auxiliar);
		return true;
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::pipe_with_script(std::stringstream* codigo, int* posicion) {
	codigo->clear();
	int posicionOriginal = *posicion;
	//Elimino posibles espacios
	erase_white_spaces(codigo, posicion);
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);

	char c;
	std::string script;
	codigo->get(c);
	if(c == '|') {
		//Elimino posibles espacios
		erase_white_spaces(codigo, posicion);
		*posicion = codigo->tellg();
		codigo->seekg(*posicion, std::ios::beg);
		while(codigo->get(c)) {
			if(c == ')') {
				break;
			}
			script+= c;
		}
		*posicion = codigo->tellg();
		return true;
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

bool Parser::object(std::stringstream* codigo, int* posicion) {
	codigo->clear();
	int posicionOriginal = *posicion;

	//Compruebo que efectivamente comienza con "(|"
	if(object_intro(codigo, posicion)) {
		//TODO PENSAR EL EMPTY

		//Puede ser que slot_list sea empty (| | ... )
		//*posicion = posicionOriginal;
		//if (empty(codigo, posicion)){
			//std::cout<<"Era un objeto vacio!"<<std::endl;
			//return true;
		//}

		if(slot_list(codigo, posicion) || (empty(codigo, posicion))) {
			//Si ya se cargo al menos un slot_list, busco otros posibles slots_lists
			//En caso de no haber, se sigue parseando el objeto

			bool nextSlot = true;
			while(nextSlot) {
				nextSlot = slot_list(codigo, posicion);
			}
			if (pipe_without_script(codigo, posicion)) {
				//Quiere decir que se encontro una barra y un parentesis
				return true;
			}
			if (pipe_with_script(codigo, posicion)) {
				//Se encontró que no hay un parentesis luego de la barra
				//Se guarda el script sin comprobar su sintaxis
				return true;
			}
		}
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::nil(std::stringstream* codigo, int* posicion) {
	codigo->clear();
	int posicionOriginal = *posicion;

	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	std::string valor;
	*codigo>>valor;

	if(valor == "nil") {
		*posicion = codigo->tellg();
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

	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	std::string valor;
	*codigo>>valor;

	if(valor.at(0) == ':') {
		*posicion = posicionOriginal + 2;
		codigo->seekg(*posicion, std::ios::beg);
		if(name(codigo, posicion)) {
			return true;
		}
	}

	if(valor.at(valor.size()-1) == '*') {
		//Guardo todo en un sstream auxiliar para comprobar que lo que sigue
		//es un name, si cumple, actualizo la posicion real
		std::stringstream auxiliar;
		for(unsigned int i=0; i<valor.size()-1; i++) {
			auxiliar << valor.at(i);
		}
		int inicio = 0;
		if(name(&auxiliar, &inicio)) {
			*posicion = codigo->tellg();
			return true;
		}
	}
	if(name(codigo, posicion)) {
		//*posicion = codigo->tellg();
		return true;
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;

}

/**
 * Este metodo no modifica la posicion del puntero en el codigo ya que
 * solo se quiere comprobar que el mismo representa un vacio
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

bool Parser::slot_list(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;

	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	std::string valor;

	if(slot_name_extended(codigo, posicion)) {
		std::string slot_name = get_msg();

		if(slot_operator(codigo, posicion)) {
			std::string op = get_op();
			if(expression(codigo, posicion)) {
				if(final(codigo, posicion)) {
					linker.create_slot(slot_name);
					return true;
				}
			}
		}
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::expressionP(std::stringstream* codigo, int* posicion) {
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
		if(expression(codigo, posicion)) {
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

bool Parser::constant(std::stringstream* codigo, int* posicion) {
	codigo->clear();
	int posicionOriginal = *posicion;

	if(number(codigo, posicion)) {
		return true;
	}

	if(text(codigo, posicion)) {
		return true;
	}

	if(object(codigo, posicion)) {
		return true;
	}
	if(nil(codigo, posicion)) {
		return true;
	}
	if(name(codigo, posicion)) {
		return true;
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::receiver(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;

	if(expressionCP(codigo, posicion)) {
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

bool Parser::removeSlots(std::stringstream* codigo, int* posicion, std::string context) {
	int posicionOriginal = *posicion;

	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	std::string valor;
	*codigo>>valor;

	bool erase;
	if(valor == "(|") {
		*posicion = codigo->tellg();
		if(name(codigo, posicion)) {
			std::string slot = get_msg();
			erase = linker.remove_slots(context, slot);
			if(erase == false) {
				setFlag("Error");
			}
		}
		*codigo>>valor;

		if((valor.at(0) == '|') && (valor.at(1) == ')')) {
			*posicion = codigo->tellg();
			return true;
		}
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::keyword_message(std::stringstream* codigo, int* posicion) {
	codigo->clear();
	int posicionOriginal = *posicion;

	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	std::string valor;

	if(receiver(codigo, posicion)) {
		std::string msg = get_msg();
		if(lower_keyword(codigo, posicion)) {
			std::string lower_key = get_msg();
			setFlag(lower_key);
			codigo->seekg(*posicion, std::ios::beg);
			*codigo>>valor;
			if(valor == ":") {
				*posicion = codigo->tellg();
				codigo->seekg(*posicion, std::ios::beg);
				*codigo>>valor;

				if(lower_key == "_AddSlots") {
					if(expressionCP(codigo, posicion)) {
						linker.create_keyword_message(msg, lower_key);

						/**
						 * Puede haber o no cap_keyword
						 */
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
									if(expressionCP(codigo, posicion)) {
										isCap_keyword = true;
									}
								}
							} else {
								isCap_keyword = false;
							}
						}

						/**
						 * Fin carga de cap_keyword
						 */

						return true;
					}
				}

				if(lower_key == "_RemoveSlots") {
					std::string contexto = msg;
					if(removeSlots(codigo, posicion, contexto)) {
						return true;
					}
				}
			}
		}
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

bool Parser::binary_message(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;

	if(receiver(codigo, posicion)) {
		if(operador(codigo, posicion)) {
			if(expressionCP(codigo, posicion)) {
				std::string op = get_op();
				linker.create_binary_message(op);
				return true;
			}
		}
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::unary_message(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;
	bool valid_search;
	if(receiver(codigo, posicion)) {
		std::string msg_name = get_msg();
		if (name(codigo, posicion)) {
			codigo->seekg(*posicion, std::ios::beg);
			std::string valor;
			*codigo>>valor;
			std::string msg = get_msg();
			if (msg_name.size() == 0) {
				linker.create_unary_message(msg);
			} else {
				valid_search = linker.create_unary_message(msg_name, msg);
				if(valid_search == false) {
					setFlag("Error");
				}
			}
			return true;
		}
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::expressionCP(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;

	if (expressionP(codigo, posicion)) {
		return true;
	}
	if (constant(codigo, posicion)) {
		return true;
	}
	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::expression(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;

	if(keyword_message(codigo, posicion)) {
		return true;
	}
	if(binary_message(codigo, posicion)) {
		return true;
	}
	if(unary_message(codigo, posicion)) {
		return true;
	}
	if(expressionCP(codigo, posicion)) {
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
	if(expression(codigo, posicion)) {
		if(final(codigo, posicion)) {
			codigo->seekg(*posicion, std::ios::beg);
			std::string valor;
			*codigo>>valor;
			if (!codigo->eof()) {
				script(codigo, posicion);
			}
			return true;
		}
	}
	return false;
}

Slot* Parser::parsear(std::string codigo) {
	std::stringstream scripts(codigo);
	int posicion = scripts.tellg();

	scripts.seekp(posicion, std::ios::beg);
	if(script(&scripts, &posicion)) {
		std::cout<<"Se completa el script papa!"<<std::endl;
		return linker.get_last_slot();
	} else {
		std::cout<<"No es un script dog, lo siento"<<std::endl;
		setFlag("Error");
		return NULL;
	}
}

Slot* Parser::parsear(std::string codigo, std::string id) {
	//Seteo el ID del contexto donde se va a trabajar
	linker.setID(id);
	//Parseo como un script comun
	return parsear(codigo);
}

void Parser::set_msg(std::string msg) {
	this->msg = msg;
}

std::string Parser::get_msg() {
	std::string aux = msg;
	msg = "";
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
	} else {
		flag = NOT_SET;
	}
}

int Parser::getFlag() {
	return flag;
}

Parser::~Parser() {
}
