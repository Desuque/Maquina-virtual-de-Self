#include "slot.h"
#include "referencia.h"
void Slot::actualizarSlot(InterfaceSlot* interface_slot){
	this->type = interface_slot->get_type();
	this->code = interface_slot->has_code();
	this->name = interface_slot->get_name();
	this->value = interface_slot->get_value();
	this->id = interface_slot->get_id();
	this->id_base = interface_slot->get_id();
}