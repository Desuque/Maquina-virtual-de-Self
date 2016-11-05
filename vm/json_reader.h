#ifndef __json_reader_h__
#define __json_reader_h__

#include "interface_slot.h"
#include <vector>

class JsonReader{
	public:
		void read(std::vector<InterfaceSlot*>& i_slots, string json_slots);
};

#endif
