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

string JsonWriter::write_position(int id, double posX, double posY){
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("id");
	writer.Int(id);
	writer.String("posX");
	writer.Double(posX);
	writer.String("posY");
	writer.Double(posY);
	writer.EndObject();
	return s.GetString();
}



string JsonWriter::write_id_morph_id_slot(int id_morph, int id_slot){
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("id_morph");
	writer.Int(id_morph);
	writer.String("id_slot");
	writer.Int(id_slot);
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

