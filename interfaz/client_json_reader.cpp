#include "client_json_reader.h"
#include "../vm/rapidjson/document.h"
#include "client_interface_slot.h"
#include <iostream>

using namespace rapidjson;

void JsonReader::read(std::vector<InterfaceSlot*>& i_slots, string json_slots){
	Document document;
	document.Parse(json_slots.c_str());
	const Value& slots = document["slots"];
	assert(slots.IsArray());
	for (Value::ConstValueIterator itr = slots.Begin(); itr != slots.End(); ++itr) {
		string name = (*itr)["name"].GetString();
		string value = (*itr)["value"].GetString();
		bool type = (*itr)["type"].GetBool();
		bool code = (*itr)["code"].GetBool();
		int id = (*itr)["id"].GetInt();
                int id_base = (*itr)["id_base"].GetInt();
                /*std::cout << name << ": " << value 
			<< " C: " << code << " T: " << type << std::endl;*/
		i_slots.push_back(new InterfaceSlot(name, value, code, type, id, id_base));
	}
}

void JsonReader::read_code(string json, string& id, string& code){
	Document document;
	document.Parse(json.c_str());
	id = document["id"].GetString();
	code = document["code"].GetString();
}

void JsonReader::read_names(string json, v_strings& names){
        Document document;
	document.Parse(json.c_str());
	const Value& lobbies = document["lobbies"];
	assert(lobbies.IsArray());
	for (Value::ConstValueIterator itr = lobbies.Begin(); itr != lobbies.End(); ++itr) {
		string name = (*itr)["name"].GetString();
		names.push_back(name);
	}
}

void JsonReader::read_garbage_ids(string json, v_ints& gar_ids){
        Document document;
	document.Parse(json.c_str());
	const Value& ids = document["ids"];
	assert(ids.IsArray());
	for (Value::ConstValueIterator itr = ids.Begin(); itr != ids.End(); ++itr) {
		int id = (*itr)["id"].GetInt();
		gar_ids.push_back(id);
	}
}


