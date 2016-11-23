#include "server_self_writer.h"


static const string start = "lobby _AddSlots: ";
static const string inic = " (| ";
static const string equal = " = ";
static const string end = " |) ."; 
                
void SelfWriter::write(m_slots& slots, string& slots_self){
	bool start = true;    
	for (m_slots::iterator it=slots.begin(); it!=slots.end(); ++it){
		Slot* sl = (it->second);
		if (start && valid_slot(sl->get_name())){
                        slots_self += " (|";
                        start = false;
                }
                    
                if (!sl->is_base_slot(sl) && !sl->is_num_slot(sl)){
			if ( sl -> is_parent()){
				slots_self += " " + sl -> get_name() + "* =";
			}else{
                                if (sl -> get_name() != "" )
                                    slots_self += " " + sl -> get_name() + " =";
			}
			if ( sl -> is_code()){
				slots_self += " (|| ";
				slots_self += sl -> get_value() -> as_string();
				slots_self += " ).";
			}else if ( sl -> is_parent()){
				slots_self += " " + sl -> parent_name() + ".";
			}else{
				sl -> get_value() -> get_self_slots(slots_self);
			}
		}
		
		m_slots::iterator final_iter = slots.end();
                --final_iter;
                if (it == final_iter && valid_slot(sl->get_name()))
		        slots_self += " |).";
	}
}

bool SelfWriter::valid_slot(string name){
        return (name != "" && name != "_Name" && name !="_Self");
}

string SelfWriter::write_full(string slot_name, m_slots& slots){
        string slots_self = start + inic + slot_name + equal;
        get_full(slots, slots_self);
        slots_self += end;
        return slots_self;
}

void SelfWriter::get_full(m_slots& slots, string& slots_self){
        bool start = true;    
	for (m_slots::iterator it=slots.begin(); it!=slots.end(); ++it){
		Slot* sl = (it->second);
		if (start && valid_slot(sl->get_name())){
                        slots_self += " (|";
                        start = false;
                }
                    
                if (!sl->is_base_slot(sl) && !sl->is_num_slot(sl)){
			if (sl -> get_name() != "" )
                                slots_self += " " + sl -> get_name() + " =";
			if ( sl -> is_code()){
				slots_self += " (|| ";
				slots_self += sl -> get_value() -> as_string();
				slots_self += " ).";
			}else{
				sl -> get_value() -> get_self_slots(slots_self);
			}
		}
		
		m_slots::iterator final_iter = slots.end();
                --final_iter;
                if (it == final_iter && valid_slot(sl->get_name()))
		        slots_self += " |).";
	}
}