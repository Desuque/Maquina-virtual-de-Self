#ifndef __server_check_point_h__
#define __server_check_point_h__

#include <string>
#include "server_slot.h"

typedef std::string string;

class CheckPoint: public Slot{
	public:
		CheckPoint(string name);
		bool is_check();
};

#endif
