#ifndef __int_h__
#define __int_h__

#include <vector>
#include "object.h"

class Int: public Object{
	private:
		int value;
	public:
		Int(int value);
		Slot* clone(VM& vm);
		Slot* print(VM& vm);
		string as_string();
		int as_int();
		void get_self_slots(string& slots_self);
};

#endif
