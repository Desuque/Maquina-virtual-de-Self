
#ifndef PARSER_H_
#define PARSER_H_

class Parser {
public:
	Parser();
	void parsear(std::string codigo);
	virtual ~Parser();
private:
	bool script(std::stringstream* codigo, int* posicion);
	bool expression(std::stringstream* codigo, int* posicion);
	bool keyword_message(std::stringstream* codigo, int* posicion);
	bool binary_message(std::stringstream* codigo, int* posicion);
	bool unary_message(std::stringstream* codigo, int* posicion);
	bool expressionCP(std::stringstream* codigo, int* posicion);
	bool receiver(std::stringstream* codigo, int* posicion);
	bool constant(std::stringstream* codigo, int* posicion);
	bool expressionP(std::stringstream* codigo, int* posicion);
	bool number(std::stringstream* codigo, int* posicion);
	bool text(std::stringstream* codigo, int* posicion);
	bool object(std::stringstream* codigo, int* posicion);
	bool slot_list(std::stringstream* codigo, int* posicion);
	bool slot_name_extended(std::stringstream* codigo, int* posicion);
	bool nil(std::stringstream* codigo, int* posicion);
	bool name(std::stringstream* codigo, int* posicion);
	bool operador(std::stringstream* codigo, int* posicion);
	bool lower_keyword(std::stringstream* codigo, int* posicion);



	bool final(std::stringstream *codigo, int* posicion);


};

#endif /* PARSER_H_ */
