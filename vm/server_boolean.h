#ifndef __server_boolean_h__
#define __server_boolean_h__

#include <vector>
#include "server_object.h"

class Boolean: public Object{
	private:
		bool value;
	public:
		Boolean(int id, bool value);
		Slot* clone(VM& vm);
		Slot* print(VM& vm);
		string as_string();
		void get_self_slots(string& slots_self);
};

#endif
