#ifndef __server_sub_h__
#define __server_sub_h__

#include <vector>
#include "server_object.h"

class Sub: public Object{
	public:
		Slot* execute(VM& vm, p_objects& args);
};

#endif
