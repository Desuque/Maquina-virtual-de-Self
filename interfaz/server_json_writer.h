#ifndef __server_json_writer_h__
#define __server_json_writer_h__

#include <string>
#include <map>
#include <vector>

#include "../vm/rapidjson/writer.h"
#include "../vm/rapidjson/stringbuffer.h"

typedef std::string string;
typedef std::vector<string> v_strings;
typedef std::vector<int> v_ids;

using namespace rapidjson;

class JsonWriter{	
	public:
		string write_code(string id, string code);
        string write_files_name(v_strings names);
	    string write_garbage_ids(v_ids& ids);
		string write_position(string id, float posX, float posY);
	private:
		void write_slot_attr(Writer<StringBuffer>& writer, string& name, string& value, bool& type, bool& code, int& id, int& id_base);
};

#endif
