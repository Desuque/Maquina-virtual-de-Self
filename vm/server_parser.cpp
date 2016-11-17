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

	std::cout<<"Entro al numero!: "<<valor<<std::endl;

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
	codigo->clear();
	int posicionOriginal = *posicion;

	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	std::string valor;
	*codigo>>valor;

	int posFinal = codigo->tellg();
	//Vuelvo el puntero para atras y comparo valor a valor, si se cumple
	//la condicion, avanzo el puntero en el stream
	codigo->seekg(posicionOriginal, std::ios::beg);

	std::string inicial = "'";
	std::string auxiliar;
	bool valido = true;
	unsigned int i = 0;
	if(valor.at(i) == inicial.at(0)) {
		auxiliar += valor.at(i);
		i++;
    	*posicion = posicionOriginal+1;
		while(valido && i<valor.size()) {
			char c = valor.at(i);
			if (c == inicial.at(0)) {
				valido = false;
			}
			auxiliar += c;
			i++;
		}
	}
	if(valido == false) {
		if(auxiliar.at(auxiliar.size()-1) == inicial.at(0)) {
			*posicion = posFinal;
			linker.create_string(auxiliar);
			return true;
		}
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
	std::cout<<"VALOR DEL C con script: "<<c<<std::endl;
	if(c == '|') {
		std::cout<<"Encontre un pipe adentro de script!"<<std::endl;
		//Elimino posibles espacios
		erase_white_spaces(codigo, posicion);
		while(codigo->get(c)) {
			if(c == ')') {
				break;
			}
			script+= c;
		}
		std::cout<<"SCRIPT QUE CARGASTE PAPA: "<<script<<std::endl;
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
	std::cout<<"VALOR DEL C: "<<c<<std::endl;
	if(c == '|') {
		std::cout<<"Encontre un pipe!"<<std::endl;
		//Elimino posibles espacios
		erase_white_spaces(codigo, posicion);
		codigo->get(c);
		if(c == ')') {
			std::cout<<"Encontre el parentesis que me faltana!"<<std::endl;
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

	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	std::string valor;
	*codigo>>valor;

	if(valor == "(|") {
		std::cout<<"Valor: "<<valor<<std::endl;
		*posicion = codigo->tellg();
		if(slot_list(codigo, posicion)) {
			std::cout<<"Pasa el slot list"<<std::endl;
			//TODO MAS DE UN SLOT_LIST
			//bool nextSlot = true;
			//while(nextSlot) {
				//nextSlot = slot_list(codigo, posicion);
				//TODO CARGAR LOS SLOTS LISTS DESDE EL LINKER!
			//}

			if (pipe_without_script(codigo, posicion)) {
				//Quiere decir que se encontro una barra y un parentesis
				return true;
			}
			if (pipe_with_script(codigo, posicion)) {
				//Se encontró que no hay un parentesis luego de la barra
				//Se guarda el script sin comprobar su sintaxis
				return true;
			}
/**
			int posAux = codigo->tellg();
			*codigo>>valor;
			std::cout<<"Valor: "<<valor<<std::endl;
			//El script puede ser parte o no del objeto
			if((valor.at(0) == '|') && (valor.at(1) == ')')) {
				*posicion = posAux + 3;
				return true;
			} else if((valor.at(0) == '|') && (valor.at(1) != ')')) {
				*posicion = *posicion + 1;
				if(script(codigo, posicion)) {
					*codigo>>valor;
					if(valor == ")") {
						//Actualizo la posicion en el codigo original
						*posicion = codigo->tellg();
						return true;
					}
				}
			}**/
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
	int posicionOriginal = *posicion;

	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	char c;
	bool exit = false;
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

bool Parser::slot_list(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;

	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	std::string valor;

	if(slot_name_extended(codigo, posicion)) {
		std::string slot = get_msg();
		codigo->seekg(*posicion, std::ios::beg);
		*codigo>>valor;
		std::cout<<"Valor del slot: "<<valor<<std::endl;
		if((valor == "=") || (valor == "<-")) {
			*posicion = codigo->tellg();
			if(expression(codigo, posicion)) {
				if(final(codigo, posicion)) {
					linker.create_slot(slot);
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

	std::cout<<"Entro en expresP: "<<std::endl;

	char c;
	codigo->get(c);
	std::cout<<"Entro en expresP: "<<c<<std::endl;

	if(c == '(') {
		*posicion = codigo->tellg();
		codigo->seekg(*posicion, std::ios::beg);
		if(expression(codigo, posicion)) {
			//Elimino posibles espacios
			erase_white_spaces(codigo, posicion);
			codigo->seekg(*posicion, std::ios::beg);
			codigo->get(c);
			std::cout<<"Ultimo caracter: "<<c<<std::endl;
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

	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	std::string valor;
	*codigo>>valor;

	unsigned int i=0;
	std::string auxiliar;

	while((valor.at(i) != ':') && (i<valor.size()-1)) {
		auxiliar+=valor.at(i);
		i++;
	}
	int lengValor = valor.length();
	int lengAux = auxiliar.length();

	std::regex rr(R"(([A-Z])[A-Z-a-z-0-9]*)");
	if(regex_match(auxiliar,rr)) {
		*posicion = codigo->tellg();
		*posicion = *posicion - (lengValor - lengAux);
		//*posicion = codigo->tellg();
		//*posicion = *posicion - 1;
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
		//std::cout<<"Paso el receiver"<<std::endl;
		if(lower_keyword(codigo, posicion)) {
			//std::cout<<"Paso el lower"<<std::endl;

			std::string lower_key = get_msg();
			setFlag(lower_key);
			//std::cout<<"Flag: "<<lower_key<<std::endl;
			codigo->seekg(*posicion, std::ios::beg);
			*codigo>>valor;
			//std::cout<<"Lowert: "<<valor<<std::endl;
			if(valor == ":") {
				*posicion = codigo->tellg();
				codigo->seekg(*posicion, std::ios::beg);
				*codigo>>valor;

				if(lower_key == "_AddSlots") {
					if(expressionCP(codigo, posicion)) {
						linker.create_keyword_message(msg, lower_key);
						/**
						bool isCap_keyword = true;
						while(isCap_keyword) {
							std::cout<<"Por ahora no rompe"<<std::endl;
							if(cap_keyword(codigo, posicion)) {
								std::cout<<"Pasa==="<<std::endl;
								codigo->seekg(*posicion, std::ios::beg);
								*codigo>>valor;
								std::string lower_key = valor;
								if((valor.at(valor.size()-1) == ':')) {
									*posicion = codigo->tellg();
									if(expressionCP(codigo, posicion)) {
										isCap_keyword = true;
									}
								}
							} else {
								isCap_keyword = false;
							}
						}**/
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

	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	std::string valor;
	*codigo>>valor;

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
		*posicion = codigo->tellg();
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
		std::cout<<"Entro al receiver del binary"<<std::endl;
		if(operador(codigo, posicion)) {
			std::cout<<"Entro al operador del binary"<<std::endl;

			if(expressionCP(codigo, posicion)) {
				std::cout<<"Entro al cp"<<std::endl;
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
	std::cout<<"Posicion: "<<*posicion<<std::endl;
	char c;
	codigo->get(c);
	std::cout<<"VALOR DE C EN FINAL: "<<c<<std::endl;
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
