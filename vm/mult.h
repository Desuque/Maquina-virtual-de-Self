#ifndef __mult_h__
#define __mult_h__

#include <vector>
#include "object.h"

class Mult: public Object{
	public:
		Slot* execute(VM& vm, p_objects& args);
};

#endif
