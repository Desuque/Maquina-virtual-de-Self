#include "Parser.h"
#include "Linker.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

Parser::Parser() {
}

std::string Parser::cortarString(std::string cadena, int inicio, int fin) {
	std::string auxiliar;
	for(int i = inicio; i<fin; i++) {
		auxiliar += cadena.at(i);
	}
	//std::cout<<"Auxiliar: "<<auxiliar<<std::endl;
	return auxiliar;
}

bool Parser::expressionP(std::string receiber) {
	char inicio = receiber.at(0);
    char fin = receiber.at(receiber.size()-1);
    //std::cout<<"Inicio: "<<inicio<<" Fin: "<<fin<<std::endl;
    if ((inicio == '(') && (fin == ')')) {
    	std::string _expression = cortarString(receiber, 1, receiber.size()-1);
    	std::cout<<"ExpressionP: "<<_expression<<std::endl;
    	if (expression(_expression)) {
    		return true;
    	}
    }
    return false;
}

bool Parser::esString(std::string _string) {
	std::regex string_regex("'[^']*'");
	if(std::regex_match(_string, string_regex)) {
	    std::string _auxiliar = cortarString(_string, 1, _string.size()-1);
	    std::cout<<"String sin comillas: "<<_auxiliar<<std::endl;
	    return true;
	}
    return false;
}

bool Parser::esNumber(std::string number) {
	std::regex rr(R"(((\+|-)?[[:digit:]]+)(\.(([[:digit:]]+)?))?((e|E)((\+|-)?)[[:digit:]]+)?)");
	if(regex_match(number,rr)) {
		//std::cout<<"EsNumber: "<<number<<std::endl;
		return true;
	}
	return false;
}

bool Parser::slot_name_extended(std::string s_name_extended) {
	if (s_name_extended.at(0) == ':') {
		std::cout<<"Esto falta programarlo papa, las cosas solas no se hacen!"<<std::endl;
		return true;
	} else if (s_name_extended.at(s_name_extended.size()-1) == '*') {
		std::cout<<"Esto tambien loco, ponete las pilas!"<<std::endl;
		return true;
	} else {
		name(s_name_extended);
		return true;
	}
	return false;
}

bool Parser::slot_list(std::string s_list) {
	std::regex slot_list_regex("(.*) (<-|=) ([^.]*)");
	std::smatch match;
	std::string s_name_extended;
	//std::cout<<"Slot list a analizar: "<<s_list<<std::endl;
	if (std::regex_search(s_list, match, slot_list_regex) && match.size() > 1) {
		s_name_extended = match.str(1);
		if (slot_name_extended(s_name_extended)) {
			//std::cout<<"ENTRE AL SLOT LIST: "<<s_name_extended<<std::endl;
			std::string _expression = match.str(3);
			std::cout<<"Expresion:"<<_expression<<std::endl;

			if (expression(_expression)) {
				linker.create_slot_list();
				//std::cout<<"Segunda parteeeeeeeeeeeeeee: "<<_expression<<std::endl;
			}
		}
		return true;
	}

	return false;
}

bool Parser::object(std::string object) {
	//No se envian los parentesis, se los agrego para comprobar
	//que cumple con el estandar de objeto
	std::string std_object = '(' + object + ')';

	//REFACTOR, ACA FALTA QUE LA SEGUNDA PARTE DEL OBJETO SEA UN SCRIPT!!!
	std::regex object_regex("\\(\\| (.*) \\|\\)");
	std::smatch match;
	std::string s_list;
	if (std::regex_search(std_object, match, object_regex) && match.size() > 1) {
		s_list = match.str(1);
		slot_list(s_list);
		//std::cout<<"Slot_list: "<<s_list<<std::endl;
		return true;
	}
	return false;
}

bool Parser::constant(std::string constant) {
	//number | string | object | ‘nil’ | name
	//std::cout<<"Constante: "<<constant<<std::endl;
	if (esString(constant)) {
		std::cout<<"ESTRING!: "<<constant<<std::endl;
		linker.create_string(constant);
		return true;
	}
	if (esNumber(constant)) {
		//std::cout<<"Era un numero!"<<std::endl;
		linker.create_int(constant);
		return true;
	}
	//object := ‘(|’ slot_list ‘|’ script ‘)’
	if (object(constant)) {
		//std::cout<<"Era un objeto!"<<std::endl;
		return true;
	}
	if (name(constant)) {
		return true;
	}
	return false;
}

bool Parser::expressionCP(std::string receiber) {
	if (expressionP(receiber)) {
		return true;
	}
	if (constant(receiber)) {
		return true;
	}
	return false;
}

bool Parser::receiver(std::string _receiver) {
	if(expressionCP(_receiver)) {
		return true;
	}
	if(constant(_receiver)) {
		return true;
	}
	return false;
}

bool Parser::name(std::string name) {
	//Cualquier palabra alfanumerica que empiece con una letra en minusculas
    std::regex name_regex("[a-z][A-Z-a-z-0-9]*");
    if(std::regex_match(name, name_regex)) {
    	linker.set_name(name);
    	//REFACTOR ESTE CREATE, ESTA MAL
    	linker.create_name(name);
    	//std::cout<<"Name: "<<name<<std::endl;
    	return true;
    }
    return false;
}

bool Parser::unary_message(std::string unary_message) {
	std::smatch match;
	std::regex umess_regex("([^\\s])*\\s([a-z][A-Z-a-z-0-9]*).");

	if (std::regex_search(unary_message, match, umess_regex) && match.size() > 1) {
		std::string _receiver = match.str(1);
		if(receiver(_receiver)) {
			std::string _name = match.str(2);
			if(name(_name)) {
				//std::cout<<"Aca entre a crear un unary"<<std::endl;
				linker.create_unary_message();
				return true;
			}
		}
	}
	return false;
}

bool Parser::lower_keyword(std::string lower_key) {
	//Cualquier palabra alfanumerica que empiece con una letra en minusculas
	//o un guion bajo
    std::regex lower_key_regex("([a-z]|_)[A-Z-a-z-0-9]*");
    if(std::regex_match(lower_key, lower_key_regex)) {
    	//std::cout<<"LOWER KEY: "<<lower_key<<std::endl;
    	return true;
    }
    return false;
}

bool Parser::keyword_message(std::string key_message) {

	//TODO falta que se sigan agregando expresiones { cap_keyword ‘:’ expressionCP }
	std::regex key_message_regex("(.*) ([^:]*): ([^.]*).");
	std::smatch match;

	//std::cout<<"KEYWORD MESSAGE: "<<key_message<<std::endl;
	if (std::regex_search(key_message, match, key_message_regex) && match.size() > 1) {
		//std::cout<<"Entre, soy un keyword "<<std::endl;
		std::string _receiver = match.str(1);
		//std::cout<<"Entre, soy un keyword receiber: "<<_receiber<<std::endl;
		if(receiver(_receiver)) {
			linker.set_receiver(_receiver);
			//std::cout<<"Entre, soy un keyword receiber"<<std::endl;
			std::string _lower_keyword = match.str(2);
			if (lower_keyword(_lower_keyword)) {
				linker.set_lower_keyword(_lower_keyword);
				std::string _expressionCP = match.str(3);
				if (expressionCP(_expressionCP) || (_expressionCP == "@")) {
				//std::cout<<"Entre, soy un keyword, lower keyword. "<<std::endl;
				linker.create_keyword_message(_lower_keyword);
				return true;
				}
			}
		}
	}
	return false;
}

void Parser::modificarScript(std::string* script, std::string* otra_expression) {
	/**
	* Esto parece un poco extraño pero funciona asi:
	* Una vez que parseó una expresion entre parentesis, reemplazo todo el texto analizado
	* por un caracter magico. Esto se traduce en que, al analizar el texto restante se
	* pida por el ultimo tipo de objeto creado en la VM.
	* Si el objeto creado se corresponde con lo que se desea seguir creando, se sigue
	* con la ejecucion normal del programa.
	*/
	std::string expression_completa = '(' + *otra_expression + ')';
	std::string::size_type pos = script->find(expression_completa, 0);
	//std::cout << "Cadena original: "<<*script<<std::endl;

	if (pos < std::string::npos)
	script->replace(pos, expression_completa.length(), "@");
	//std::cout << "Cadena resultante: "<<*script<<std::endl;
}

bool Parser::parsear(std::string script) {
	bool parentesis_encontrado = false;
	std::string otra_expression;

	for(unsigned int i=0; i<script.size()-1; i++) {
		if (script.at(i) == ')') {
			parentesis_encontrado = false;
			if (parsear(otra_expression)) {
				modificarScript(&script, &otra_expression);
				break;
			}
		}
		if (parentesis_encontrado) {
			otra_expression += script.at(i);
		}
		if(script.at(i) == '(') {
			parentesis_encontrado = true;
		}
	}
	//std::cout<<"Script: "<<script<<std::endl;
	//std::cout<<"Otra expression: "<<otra_expression<<std::endl;
	return expression(script);
}

bool Parser::expression(std::string expression) {
	std::cout<<"Expression: "<<expression<<std::endl;
	if(keyword_message(expression)) {
		return true;
	}
	//if(binary_message(expression)) {
		//return true;
	//}
	if(unary_message(expression)) {
		return true;
	}
	if(expressionCP(expression)) {
		return true;
	}

	return false;
}

void Parser::ingresar_scripts(std::string codigo) {
/**
	std::stringstream my_stream(std::ios::in|std::ios::out);
	        std::string dat("Hey, I have a double : 74.79 .");

	        my_stream.str(dat);
	        my_stream.seekg(-7,std::ios::end);

	        double val;
	        my_stream>>val;

	        val= val*val;

	        my_stream.seekp(-7,std::ios::end);
	        my_stream<<val;

	        std::string new_val = my_stream.str();
	        std::cout<<new_val;

**/


	std::stringstream script(codigo);

	//seekg(0); seekg(0,ios::beg);         //sets the get pointer to the beginning.
	//seekg(5,ios::beg);      //sets the get pointer to 5 chars forward of the beginning.
	//tellp(); tellg()              //returns the current value of the put/get pointer
	//seekp(-10,ios::end);   //sets the put pointer to 10 chars before the end
	//seekp(1,ios::cur);      //proceeds to next char


	//"2 print."
	//"(9+3) print."


	script.seekg(0);
	std::string value;
	script>>value;
	std::cout<<"Valor: "<<value<<std::endl;
	std::cout<<"Pointer: "<<script.tellg();
	//guardar el valor con los piquitos en int y en string
	//a ver que guarda, con eso estaria el parser

	//expression(codigo);

/**
	std::vector<std::string> scripts;
	std::stringstream stringStream(codigo);
	std::string line;
	while(std::getline(stringStream, line))
	{
	    std::size_t prev = 0, pos;
	    //REvisar el \n, tiene que ser el punto final del script
	    while ((pos = line.find_first_of("\n", prev)) != std::string::npos)
	    {
	        if (pos > prev) {
	            scripts.push_back((line.substr(prev, pos-prev)).c_str());
	        }
	        prev = pos+1;
	    }
	    if (prev < line.length()) {
	    	scripts.push_back((line.substr(prev, std::string::npos)).c_str());
	    }
	}

	for(unsigned int i=0; i<scripts.size();i++) {
		parsear(scripts.at(i));
		/**
		 * Termino de ejecutar el script y recolecto

		linker.collect();
	}
**/
}

Parser::~Parser() {
}


/**
 * TODO
 * Sacar las expresiones regulares hasta los lugares donde no hay opcion de recursividad
 * Sacar los parentesis
 * REsolver de afuera para adentro
 * Sacar lo del @
 * stringstream, recordar la posicion del puntero, si falla, volver a la posicion anterior
 */
