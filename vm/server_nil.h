#ifndef __server_nil_h__
#define __server_nil_h__

#include <vector>
#include "server_object.h"

typedef std::string string;

class Nil: public Object{
	private:
		string value;
	public:
		Nil(int id);
		string as_string();
		Slot* clone(VM& vm);
		Slot* print(VM& vm);
		void get_self_slots(string& slots_self);
};

#endif
