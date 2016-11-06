#ifndef __server_string_h__
#define __server_string_h__

#include <vector>
#include "server_object.h"

typedef std::string string;

class String: public Object{
	private:
		string value;
	public:
		String(string value);
		string as_string();
		Slot* clone(VM& vm);
		Slot* print(VM& vm);
		void get_self_slots(string& slots_self);
};

#endif
