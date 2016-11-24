#ifndef __client_json_reader_h__
#define __client_json_reader_h__

#include "client_interface_slot.h"
#include <vector>

typedef  std::vector<string> v_strings;
typedef  std::vector<int> v_ints;

class JsonReader{
	public:
		void read(std::vector<InterfaceSlot*>& i_slots, string json_slots);
		void read_code(string json, string& id, string& code);
        void read_names(string json, v_strings& names);
        void read_garbage_ids(string json, v_ints& ids);
        void read_position(string json, int& id, double& posX, double& posY); 
        void read_id_morph_id_slot(string json, int& id_morph, int& id_slot);
        void read_id_morph(string json, int& id_morph);
		void read_share_data(string data, int& id, string& lobby_des);
};

#endif
