#ifndef __server_self_writer_h__
#define __server_self_writer_h__

#include <string>

#include "server_slot.h"
#include <map>

typedef std::string string;
typedef std::multimap<std::string,Slot*> m_slots;


class SelfWriter{	
	public:
		void write(m_slots& slots, string& slots_self);
                string write_full(string slot_name, m_slots& slots);
	
        private:
		bool valid_slot(string name);
                void get_full(m_slots& slots, string& slots_self);
};

#endif
