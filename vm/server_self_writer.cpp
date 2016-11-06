#include "server_self_writer.h"

void SelfWriter::write(m_slots& slots, string& slots_self){
	slots_self += "(| ";
	for (m_slots::iterator it=slots.begin(); it!=slots.end(); ++it){
		Slot* sl = (it->second);
		if (!sl->is_base_slot(sl) && !sl->is_num_slot(sl)){
			if ( sl -> is_parent()){
				slots_self +=  sl -> get_name() + "* = ";
			}else{
				slots_self +=  sl -> get_name() + " = ";
			}
			if ( sl -> is_code()){
				slots_self += "(|| ";
				slots_self += sl -> get_value() -> as_string();
				slots_self += " ). ";
			}else if ( sl -> is_parent()){
				slots_self += sl -> parent_name() + ". ";
			}else{
				sl -> get_value() -> get_self_slots(slots_self);
			}
		}
	}
	slots_self += " |). ";
}
