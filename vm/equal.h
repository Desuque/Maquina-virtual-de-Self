#ifndef __equal_h__
#define __equal_h__

#include <vector>
#include "object.h"

class Equal: public Object{
	public:
		Slot* execute(VM& vm, p_objects& args);
};

#endif
