#include "server_json_writer.h"
#include <sstream>

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

string float_to_string(float aFloat){
	std::ostringstream ss;
	ss << aFloat;
	return ss.str();
}

string JsonWriter::write_position(string id, float posX, float posY){
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("id");
	writer.String(id.c_str());
	writer.String("posX");
	writer.String(float_to_string(posX).c_str());
	writer.String("posY");
	writer.String(float_to_string(posY).c_str());
	writer.EndObject();
	return s.GetString();
}


string JsonWriter::write_garbage_ids(v_ids& ids){
        StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("ids");
	writer.StartArray();
	int size = ids.size();
        for (int i = 0; i < size; i++){
                writer.StartObject();
                writer.Key("id");
                writer.Int(ids[i]);
                writer.EndObject();
        }
	writer.EndArray();
	writer.EndObject();
	return s.GetString();
}

