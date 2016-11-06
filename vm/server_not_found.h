#ifndef __server_not_found_h__
#define __server_not_found_h__

#include <iostream>
#include <exception>

class NotFound: public std::exception{
	private:
		std::string name;
	public:
		NotFound(std::string name):name(name){}
		virtual const char* what() const throw(){
			std::string msg;
			msg = "Error: " + this -> name + " not found.";
			return msg.c_str();
		}
};

#endif
