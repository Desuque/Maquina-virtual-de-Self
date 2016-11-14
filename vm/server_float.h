#ifndef __server_float_h__
#define __server_float_h__

#include <vector>
#include "server_object.h"

class Float: public Object{
	private:
		float value;
	public:
		Float(int id, float value);
		Slot* clone(VM& vm);
		Slot* print(VM& vm);
		string as_string();
		int as_int();
		void get_self_slots(string& slots_self);
};

#endif