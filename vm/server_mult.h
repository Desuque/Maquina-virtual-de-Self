#ifndef __server_mult_h__
#define __server_mult_h__

#include <vector>
#include "server_object.h"

class Mult: public Object{
	public:
		Slot* execute(VM& vm, p_objects& args);
};

#endif
