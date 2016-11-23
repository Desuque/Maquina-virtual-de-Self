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
                float as_float();
		void get_self_slots(string& slots_self);
                Slot* add(VM& vm, Object* op);
                Slot* sub(VM& vm, Object* op);
                Slot* mult(VM& vm, Object* op);
                Slot* div(VM& vm, Object* op);
                Slot* equal(VM& vm, Object* op);
                Slot* nequal(VM& vm, Object* op);
};

#endif
