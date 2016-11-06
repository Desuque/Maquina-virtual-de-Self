#ifndef __server_not_equal_h__
#define __server_not_equal_h__

#include <vector>
#include "server_object.h"

class NotEqual: public Object{
	public:
		Slot* execute(VM& vm, p_objects& args);
};

#endif
