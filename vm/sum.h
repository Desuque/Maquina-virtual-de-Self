#ifndef __sum_h__
#define __sum_h__

#include <vector>
#include "object.h"

class Sum: public Object{
	public:
		Slot* execute(VM& vm, p_objects& args);
};

#endif
