#ifndef SLOT_H
#define SLOT_H
#include "morph.h"
#include "string.h"

#define ALTURA_SLOT 18

class Slot : public Morph{
public:
	//std::string name;
	//Morph* morph;
	/*int posX;
	int posY;
	int width;
	int height;*/
	bool dibujadoComoMorph;
	Referencia* referencia;
	/* interface Slot */
	bool type;
	bool code;
	std::string name;
	std::string value;
	int id;
public:	
	Slot(){}
	
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
	}

	Slot(std::string name, double posX, double posY, int width, int height){
		Pango::FontDescription font;
		font.set_family("Monospace");
		font.set_weight(Pango::WEIGHT_LIGHT);
		// http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
		auto layout = create_pango_layout(this->name);
		layout->set_font_description(font);
		int text_width;
		int text_height;
		layout->get_pixel_size(text_width, text_height);
		this->height = text_height;
		this->posX = posX;
		this->posY = posY;
		this->width = width;
		this->name = name;
		this->dibujadoComoMorph=false;
		this->referencia = nullptr;
		/*refTextViewConsola = Gtk::TextBuffer::create();
		refTextViewConsola->set_text("");*/
		//referencia = new Morph(posX-8,posY,8,8,m_TextView);
	}

	int get_id(){
		return this->id;
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

	void draw(const Cairo::RefPtr<Cairo::Context>& cr){
		Pango::FontDescription font;

		font.set_family("Monospace");
		font.set_weight(Pango::WEIGHT_LIGHT);

		// http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
		auto layout = create_pango_layout(this->name);

		layout->set_font_description(font);

		/*int text_width,text_height;
		layout->get_pixel_size(text_width, text_height);*/
		
		//this->height = text_height;

		// rectangulo que representa al slot
		cr -> rectangle(this->posX , this->posY, this->width, this->height);
		cr->set_line_width(0.5);
		//cr-> stroke_preserve();
		// rectangulo para pedir el slot.
		cr -> rectangle(this->posX + this->width - 10, this->posY + 2, 8, this->height - 4);
		//cr-> stroke();
		
		/*if(this->referencia){
			cr->move_to(this->posX + this->width - 6, this->posY + (this->height/2));
			cr -> line_to(this->referencia->posX,this->referencia->posY+4);
		}*/

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