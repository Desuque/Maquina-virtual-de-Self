
#include <iostream>
#include "Parser.h"

using namespace std;

int main() {
	Parser parser;

	//Test unary message
	parser.parsear("223423 print.");
	std::cout<<std::endl<<std::endl;
	parser.parsear("'hola' print.");
	std::cout<<std::endl<<std::endl;
	parser.parsear("'hola' print. 'chau' print.");
	std::cout<<std::endl<<std::endl;

	//Test binary message
	parser.parsear("2 + 2 .");
	std::cout<<std::endl<<std::endl;

	//Test binary message con print
	parser.parsear("(2 + 2 ) print.");
	std::cout<<std::endl<<std::endl;
	parser.parsear("(8 * 8 ) print.");
	std::cout<<std::endl<<std::endl;
	parser.parsear("(6 / 3 ) print.");
	std::cout<<std::endl<<std::endl;
	parser.parsear("(3 + 4 ) print.");
	std::cout<<std::endl<<std::endl;
	parser.parsear("(5 == 7 ) print.");


	//parser.parsear("(| algo = 3 . | print. ) print.");
	//std::cout<<std::endl<<std::endl;
	/**
	parser.parsear("(| :algo = 3 . | print. ) print.");
	std::cout<<std::endl<<std::endl;
	parser.parsear("(| algo* <- 3 . | print. ) print.");
	std::cout<<std::endl<<std::endl;
	parser.parsear("(| algo = nil . | print. ) print.");
	std::cout<<std::endl<<std::endl;
	parser.parsear("4 * 8 .");
	std::cout<<std::endl<<std::endl;
	parser.parsear("5 == 5 .");
	std::cout<<std::endl<<std::endl;
	parser.parsear("123 != 45 .");
	std::cout<<std::endl<<std::endl;


	std::cout<<std::endl<<std::endl;
	parser.parsear("(2 print ) print.");


	std::cout<<std::endl<<std::endl;
	parser.parsear("lobby _AddSlots: (| y <- 8 . | print. ) print.");
	**/
	return 0;
}
