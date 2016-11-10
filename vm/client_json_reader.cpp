#include "client_json_reader.h"
#include "rapidjson/document.h"
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
		/*std::cout << name << ": " << value 
			<< " C: " << code << " T: " << type << std::endl;*/
		i_slots.push_back(new InterfaceSlot(name, value, code, type, id));
	}
}

void JsonReader::read_code(string json, string& id, string& code){
	Document document;
	document.Parse(json.c_str());
	id = document["id"].GetString();
	code = document["code"].GetString();
}
