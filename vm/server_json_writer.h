#ifndef __server_json_writer_h__
#define __server_json_writer_h__

#include <string>
#include <map>

#include "server_slot.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

typedef std::multimap<std::string,Slot*> m_slots;
typedef std::string string;

using namespace rapidjson;

class JsonWriter{	
	public:
		string write_slot(Slot* sl);
		string write(m_slots& slots);
		string write_code(string id, string code);
	
	private:
		void write_slot_attr(Writer<StringBuffer>& writer, string& name, string& value, bool& type, bool& code, int& id);
		void get_slot_attr(Slot* sl, string& name, string& value, bool& type, bool& code, int& id);
};

#endif
