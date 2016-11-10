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
			int id;
			get_slot_attr(sl, name, value, type, code, id);
			write_slot_attr(writer,name, value, type, code, id);
		}
	}
	
	writer.EndArray();
	writer.EndObject();
	return s.GetString();
}

void JsonWriter::get_slot_attr(Slot* sl, string& name, string& value, bool& type, bool& code, int& id){
	name = sl -> get_name();
	value = sl -> get_value() -> as_string();
	type = sl -> is_immutable();
	code = sl -> is_code();
	id = sl -> get_id();
}

void JsonWriter::write_slot_attr(Writer<StringBuffer>& writer, string& name, string& value, bool& type, bool& code, int& id){
	writer.StartObject();
	writer.Key("id");
	writer.Int(id);
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

string JsonWriter::write_code(string id, string code){
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("id");
	writer.String(id.c_str());
	writer.Key("code");
	writer.String(code.c_str());
	writer.EndObject();
	return s.GetString();
}
