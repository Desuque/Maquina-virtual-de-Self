#ifndef __check_point_h__
#define __check_point_h__

#include <string>
#include "slot.h"

typedef std::string string;

class CheckPoint: public Slot{
	public:
		CheckPoint(string name);
		bool is_check();
};

#endif
