#ifndef SLOT_H
#define SLOT_H
#include "morph.h"
#include "string.h"

#define ALTURA_SLOT 18

class Slot : public Morph{
private:
	bool dibujadoComoMorph;
	/* interface Slot */
	bool type;
	bool code;
	std::string name;
	std::string value;
	int id;
	int id_base;

public:	
	Referencia* referencia;

public:	
	//Slot(){}
	
	Slot(InterfaceSlot* interfaceSlot, double posX, double posY, int width, int height){
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

	std::string get_name(){
		return this->name;
	}

	int getWidth(){
		return this->width;
	}

	int getHeight(){
		return this->height;
	}

	bool tieneEsteId(int id){
		return (this->id == id);
	}

	int get_id(){
		return this->id;
	}

	bool has_code(){
		return this->code;
	}

	std::string get_value(){
		return this->value;
	}

	bool estaDibujadoComoMorph(){
		return this->dibujadoComoMorph;
	}

	void setEstaDibujadoComoMorph(bool estaDibujado){
		this->dibujadoComoMorph = estaDibujado;
	}

	std::string get_id_to_string(){
		return std::to_string(this->id);
	}

	bool elValorEsPrimitivo(){
		return (this->value != "object");
	}

	void draw(const Cairo::RefPtr<Cairo::Context>& cr){
		Pango::FontDescription font;

		font.set_family("Monospace");
		font.set_weight(Pango::WEIGHT_LIGHT);

		// http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
		auto layout = create_pango_layout(this->name);

		layout->set_font_description(font);

		// rectangulo que representa al slot
		cr -> rectangle(this->posX , this->posY, this->width, this->height);

		// para obtener el valor el valor del slot como morph
		cr->set_line_width(0.5);
		cr -> rectangle(this->posX + this->width - 10, this->posY + 2, 8, this->height - 4);
		
		cr->stroke();

		cr->move_to(this->posX, this->posY);

		layout->show_in_cairo_context(cr);
	}

	void setReferencia(Referencia* unaReferencia){
		this->referencia = unaReferencia;
	}

	bool clikEnObtenerSlot(int posX,int posY){
		return	(posX >= (this->posX) + (this->width) - 10) 
				&& (posY >= (this->posY) + 2)
				&& (posX <= (((this->posX) + (this->width)) - 2)) 
				&& (posY <= ((this->posY) + this->height - 2));
	}
	
	size_t get_height(){
		return this->height;
	}
	
	virtual ~Slot(){}
};

#endif