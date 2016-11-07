
#include <iostream>
#include "Parser.h"

using namespace std;

int main() {
	Parser parser;

	parser.parsear("223423 print.");
	std::cout<<std::endl<<std::endl;
	parser.parsear("'hola' print.");
	std::cout<<std::endl<<std::endl;

	parser.parsear("(| algo = 3 . | print. ) print.");
	std::cout<<std::endl<<std::endl;
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

	parser.parsear("'hola' print. 'chau' print.");


	std::cout<<std::endl<<std::endl;
	parser.parsear("(3 + 4 ) print.");
	std::cout<<std::endl<<std::endl;
	parser.parsear("(2 print ) print.");

	std::cout<<std::endl<<std::endl;
	parser.parsear("lobby _AddSlots: (| y <- 8 . | print. ) .");

	return 0;
}
