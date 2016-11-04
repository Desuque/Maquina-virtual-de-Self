#ifndef __error_type_h__
#define __error_type_h__

#include <iostream>
#include <exception>

class ErrorType: public std::exception{
	private:
		std::string name;
	public:
		ErrorType(std::string name):name(name){}
		virtual const char* what() const throw(){
			std::string msg;
			msg = "Error: " + this -> name + " is immutable.";
			return msg.c_str();
		}
};

#endif
