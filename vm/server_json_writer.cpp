#include "server_json_writer.h"

string JsonWriter::write(m_slots& slots){
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("slots");
	writer.StartArray();
	
	for (m_slots::iterator it=slots.begin(); it!=slots.end(); ++it){
		Slot* sl = it->second;
		if (!sl->is_base_slot(sl)){
			string name, value;
			bool type, code;
			get_slot_attr(sl, name, value, type, code);
			write_slot_attr(writer,name, value, type, code);
		}
	}
	
	writer.EndArray();
	writer.EndObject();
	return s.GetString();
}

void JsonWriter::get_slot_attr(Slot* sl, string& name, string& value, bool& type, bool& code){
	name = sl -> get_name();
	value = sl -> get_value() -> as_string();
	type = sl -> is_immutable();
	code = sl -> is_code();
}

void JsonWriter::write_slot_attr(Writer<StringBuffer>& writer, string& name, string& value, bool& type, bool& code){
	writer.StartObject();
	writer.Key("name");
	writer.String(name.c_str());
	writer.Key("value");
	writer.String(value.c_str());
	writer.Key("type");
	writer.Bool(type);
	writer.Key("code");
	writer.Bool(code);
	writer.EndObject();
}
