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

Slot::Slot(InterfaceSlot* interfaceSlot, double posX, double posY, int width, int height){
	this->dibujadoComoMorph = false;
	this->referencia = nullptr;
	this->width = width;
	this->height = ALTURA_SLOT;
	this->posX = posX;
	this->posY = posY;
	this->name = interfaceSlot->get_name();
	this->code = interfaceSlot->has_code();
	this->value = interfaceSlot->get_value();
	this->type = interfaceSlot->get_type();
	this->id = interfaceSlot->get_id();
	this->id_base = interfaceSlot->get_id_base();
}

std::string Slot::get_name(){
	return this->name;
}

int Slot::getWidth(){
	return this->width;
}

int Slot::getHeight(){
	return this->height;
}

void Slot::borrarReferenciaAlMorphApuntado(){
	// al morph le saco esta referencia.
	this->referencia->borrarReferenciaAlMorphApuntado();
}

bool Slot::tieneEsteId(int id){
	return (this->id == id);
}

bool Slot::tieneEstaReferencia(Referencia* unaReferencia){
	if(this->referencia){
		return (this->referencia == unaReferencia);
	}
	return false;
}

bool Slot::tieneEsteNombre(std::string nombre){
	return (this->name == nombre);
}

int Slot::get_id(){
	return this->id;
}

bool Slot::has_code(){
	return this->code;
}

std::string Slot::get_value(){
	return this->value;
}

bool Slot::estaDibujadoComoMorph(){
	return this->dibujadoComoMorph;
}

void Slot::setEstaDibujadoComoMorph(bool estaDibujado){
	this->dibujadoComoMorph = estaDibujado;
}

std::string Slot::get_id_to_string(){
	return std::to_string(this->id);
}

bool Slot::elValorEsPrimitivo(){
	return (this->value != "object");
}

void Slot::draw(const Cairo::RefPtr<Cairo::Context>& cr){
	Pango::FontDescription font;

	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_LIGHT);

	// http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
	auto layout = create_pango_layout(this->name);

	layout->set_font_description(font);

	// rectangulo que representa al slot
	cr -> rectangle(this->posX , this->posY, this->width, this->height);

	cr->set_line_width(0.5);
	// rectangulo para obtener el valor el valor del slot como morph
	cr -> rectangle(this->posX + this->width - 10, this->posY + 2, 8, this->height - 4);
	
	cr->stroke();

	cr->move_to(this->posX, this->posY);

	layout->show_in_cairo_context(cr);
}

void Slot::setReferencia(Referencia* unaReferencia){
	this->referencia = unaReferencia;
}

//retorna verdadero si hicieron click en obtener slot como morph.
bool Slot::clikEnObtenerSlot(int posX,int posY){
	return	(posX >= (this->posX) + (this->width) - 10) 
			&& (posY >= (this->posY) + 2)
			&& (posX <= (((this->posX) + (this->width)) - 2)) 
			&& (posY <= ((this->posY) + this->height - 2));
}