#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include "Parser.h"
#include "Linker.h"
#include "server_virtual_machine.h"

#define NOT_SET -1

Parser::Parser() : vm(NULL) {
	msg = "";
	flag = NOT_SET;
}

void Parser::setVM(VM* vm) {
	this->vm = vm;
	linker.setVM(vm);
}

bool Parser::number(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;
	std::string valor;
	codigo->seekg(*posicion, std::ios::beg);
	*codigo>>valor;

	std::regex rr(R"(((\+|-)?[[:digit:]]+)(\.(([[:digit:]]+)?))?)");
	if(regex_match(valor,rr)) {
		*posicion = codigo->tellg();
		linker.create_int(valor);
		return true;
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::text(std::stringstream* codigo, int* posicion) {
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
			//*posicion = posicionOriginal+(i+1);
			auxiliar += c;
			i++;
		}
	}
	if(valido == false) {
		if(auxiliar.at(auxiliar.size()-1) == inicial.at(0)) {
			//std::cout<<"Texto: "<<auxiliar<<std::endl;
			//std::cout<<"Posicion: "<<*posicion<<std::endl;
			*posicion = posFinal;
			linker.create_string(auxiliar);
			return true;
		}
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::object(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;

	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	std::string valor;
	*codigo>>valor;

	if(valor == "(|") {
		*posicion = codigo->tellg();
		if(slot_list(codigo, posicion)) {
			bool nextSlot = true;
			while(nextSlot) {
				nextSlot = slot_list(codigo, posicion);
				//TODO CARGAR LOS SLOTS LISTS DESDE EL LINKER!
			}
			*codigo>>valor;
			//El script puede ser parte o no del objeto
			if((valor.at(0) == '|') && (valor.at(1) == ')')) {
				*posicion = codigo->tellg();
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
			}
		}
	}

	//Si no hay coincidencia, vuelvo el puntero a su posicion original
	*posicion = posicionOriginal;
	return false;
}

bool Parser::nil(std::stringstream* codigo, int* posicion) {
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

bool Parser::name(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;

	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	std::string valor;
	*codigo>>valor;

	//Vuelvo el puntero para atras y comparo valor a valor, si se cumple
	//la condicion, avanzo el puntero en el stream
	codigo->seekg(posicionOriginal, std::ios::beg);

	std::string auxiliar;
	bool valido = true;
	unsigned int i = 0;
	int posiciones = 1;
	while((valido) && (valor.size() > i)) {
	    char c = valor.at(i);
	    if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) ||
	    		((c >= '0') && (c <= '9'))) {
	    	posiciones++;
	    	auxiliar += c;
	    } else {
	    	valido = false;
	    }
	    i++;
	}

	//Cualquier palabra alfanumerica que empiece con una letra en minusculas
	std::regex name_regex("[a-z][A-Z-a-z-0-9]*");
	if(std::regex_match(auxiliar, name_regex)) {
		*posicion = *posicion + posiciones;
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
	std::string valor;
	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);

	if(slot_name_extended(codigo, posicion)) {
		std::string slot = get_msg();
		codigo->seekg(*posicion, std::ios::beg);
		*codigo>>valor;

		if((valor == "=") || (valor == "<-")) {
			*posicion = codigo->tellg();
			if(expression(codigo, posicion)) {
				if(final(codigo, posicion)) {
					linker.create_slot(slot);
					*posicion = codigo->tellg();
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

	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	std::string valor;
	*codigo>>valor;

	if(valor.at(0) == '(') {
		*posicion = posicionOriginal + 1;
		if(expression(codigo, posicion)) {
			codigo->seekg(*posicion, std::ios::beg);
			*codigo>>valor;
			if(valor.at(0) == ')') {
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

	std::regex rr(R"((\_|[a-z])[A-Z-a-z-0-9]*)");
	if(regex_match(auxiliar,rr)) {
		//*posicion = codigo->tellg();
		*posicion = *posicion - (lengValor - lengAux);
		//*posicion = codigo->tellg();
		//*posicion = *posicion - 1;
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

void Parser::setFlag(std::string valor) {
	if(valor == "_AddSlots:") {
		flag = 3;
	} else {
		flag = -1;
	}
}

int Parser::getFlag() {
	return flag;
}

bool Parser::keyword_message(std::stringstream* codigo, int* posicion) {
	int posicionOriginal = *posicion;

	//Leo todo el valor desde la posicion indicada
	codigo->seekg(*posicion, std::ios::beg);
	std::string valor;
	*codigo>>valor;

	if(receiver(codigo, posicion)) {
		std::string msg = get_msg();
		if(lower_keyword(codigo, posicion)) {
			codigo->seekg(*posicion, std::ios::beg);
			*codigo>>valor;
			std::string lower_key = valor;
			setFlag(valor);
			if((valor.at(valor.size()-1) == ':')) {
				*posicion = codigo->tellg();
				codigo->seekg(*posicion, std::ios::beg);
				*codigo>>valor;
				if(expressionCP(codigo, posicion)) {
					linker.create_keyword_message(msg, lower_key);
					bool isCap_keyword = true;
					while(isCap_keyword) {

						if(cap_keyword(codigo, posicion)) {
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
					}
					return true;
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
	if(receiver(codigo, posicion)) {
		std::string msg_name = get_msg();
		if (name(codigo, posicion)) {
			std::string msg = get_msg();
			if (msg_name.size() == 0) {
				linker.create_unary_message(msg);
			} else {
				linker.create_unary_message(msg_name, msg);
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
	codigo->seekg(*posicion, std::ios::beg);
	std::string valor;
	*codigo>>valor;

	if(valor.at(0) == '.') {
		//*posicion = (int)codigo->tellg() - 1;
		*posicion = posicionOriginal + 1;
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
		std::cout<<"Era un script hecho y derecho!"<<std::endl;
	}
	std::cout<<"ESto devuelvo: "<<linker.get_last_slot()<<std::endl;
	return linker.get_last_slot();
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

Parser::~Parser() {
}
