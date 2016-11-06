#ifndef __server_sum_h__
#define __server_sum_h__

#include <vector>
#include "server_object.h"

class Sum: public Object{
	public:
		Slot* execute(VM& vm, p_objects& args);
};

#endif
