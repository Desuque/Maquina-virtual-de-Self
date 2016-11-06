#ifndef __self_writer_h__
#define __self_writer_h__

#include <string>

#include "slot.h"
#include <map>

typedef std::string string;
typedef std::multimap<std::string,Slot*> m_slots;


class SelfWriter{	
	public:
		void write(m_slots& slots, string& slots_self);
};

#endif
