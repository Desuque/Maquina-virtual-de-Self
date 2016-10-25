#ifndef __div_h__
#define __div_h__

#include <vector>
#include "object.h"

class Div: public Object{
	public:
		Slot* execute(VM& vm, p_objects& args);
};

#endif
