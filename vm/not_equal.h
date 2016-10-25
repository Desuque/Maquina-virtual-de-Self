#ifndef __not_equal_h__
#define __not_equal_h__

#include <vector>
#include "object.h"

class NotEqual: public Object{
	public:
		Slot* execute(VM& vm, p_objects& args);
};

#endif
