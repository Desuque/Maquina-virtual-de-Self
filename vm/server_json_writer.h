#ifndef __server_json_writer_h__
#define __server_json_writer_h__

#include <string>
#include <map>
#include <vector>

#include "server_slot.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

typedef std::multimap<std::string,Slot*> m_slots;
typedef std::string string;
typedef std::vector<string> v_strings;
typedef std::vector<int> v_ids;
typedef std::vector<Slot*> v_slots;

using namespace rapidjson;

class JsonWriter{	
	public:
		string write_slot(int id_base, Slot* sl);
		string write(int id_base, m_slots& slots);
		string write_code(string id, string code);
        string write_files_name(v_strings names);
        string write_garbage_ids(v_ids& ids);
		string write_slots_value(int id_base, v_slots& slots);
		string write_position(int id, double posX, double posY);

	private:
		void write_slot_attr(Writer<StringBuffer>& writer, string& name, string& value, bool& type, bool& code, int& id, int& id_base);
		void get_slot_attr(Slot* sl, string& name, string& value, bool& type, bool& code, int& id);
};

#endif
