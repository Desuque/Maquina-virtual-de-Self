#ifndef __sub_h__
#define __sub_h__

#include <vector>
#include "object.h"

class Sub: public Object{
	public:
		Slot* execute(VM& vm, p_objects& args);
};

#endif
