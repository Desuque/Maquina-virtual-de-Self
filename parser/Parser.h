#ifndef PARSER_H_
#define PARSER_H_

#include "Linker.h"
#include <string>
#include <vector>

class Parser {
private:
	Linker linker;
public:
	Parser();
	void ingresar_scripts(std::string codigo);
	virtual ~Parser();

private:
	bool parsear(std::string entrada);
	bool expression(std::string expression);
	bool unary_message(std::string unary_message);
	bool receiver(std::string receiver);
	bool name(std::string name);
	bool expressionCP(std::string receiber);
	bool expressionP(std::string receiber);
	bool constant(std::string constant);
	bool esString(std::string _string);
	bool esNumber(std::string number);
	bool object(std::string object);
	bool slot_list(std::string s_list);
	bool slot_name_extended(std::string s_name_extended);
	bool keyword_message(std::string key_message);
	bool lower_keyword(std::string lower_key);




	void modificarScript(std::string* script, std::string* otra_expression);




	//Clase Utilidades o reemplazar con un regex, despues fijate
	std::string cortarString(std::string cadena, int inicio, int fin);

};

#endif /* PARSER_H */
