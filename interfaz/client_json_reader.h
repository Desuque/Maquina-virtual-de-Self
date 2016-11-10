#ifndef __client_json_reader_h__
#define __client_json_reader_h__

#include "client_interface_slot.h"
#include <vector>

class JsonReader{
	public:
		void read(std::vector<InterfaceSlot*>& i_slots, string json_slots);
		void read_code(string json, string& id, string& code);
};

#endif
