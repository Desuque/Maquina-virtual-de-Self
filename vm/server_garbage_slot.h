#ifndef __server_garbage_slot_h__
#define __server_garbage_slot_h__

#include <string>
#include <vector>
#include "server_slot.h"

typedef std::string string;
typedef std::vector<int> v_ids;

class GarbageSlot: public Slot{
        private:
                v_ids ids;
	public:
		GarbageSlot(int id, string name, v_ids& ids);
                string json();
};

#endif
