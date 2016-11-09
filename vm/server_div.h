#ifndef __server_div_h__
#define __server_div_h__

#include <vector>
#include "server_object.h"

class Div: public Object{
	public:
		Div(int id);
		Slot* execute(VM& vm, p_objects& args);
};

#endif
