#include "morph.h"
#include "slot.h"
#include "referencia.h"
#include <gtkmm.h>
#include <pangomm/layout.h>

Morph::Morph(double posX, double posY){
	this->posX = posX;
	this->posY = posY;
	this -> nombreObjeto = "que?";
	this->referencia=nullptr;
}

Morph::Morph(double posX, double posY, int width, int height,Gtk::TextView* m_TextView){
	this->posX = posX;
	this->posY = posY;
	this->width = width;
	this->height = height;
	this -> nombreObjeto = "";
	this->m_TextView=m_TextView;
	refTextViewConsola = Gtk::TextBuffer::create();
	refTextViewConsola->set_text("");
	this->referencia=nullptr;
}

Morph::Morph(std::string nombreObjeto, double posX, double posY, int width, int height){
	Pango::FontDescription font;
	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_LIGHT);
	// http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
	auto layout = create_pango_layout(this->nombreObjeto);
	layout->set_font_description(font);
	int text_width;
	int text_height;
	layout->get_pixel_size(text_width, text_height);
	this->height = text_height;
	this->posX = posX;
	this->posY = posY;
	this->width = width;
	this->nombreObjeto = nombreObjeto;
	refTextViewConsola = Gtk::TextBuffer::create();
	refTextViewConsola->set_text("");
	this->referencia=nullptr;
	//referencia = new Morph(posX-8,posY,8,8,m_TextView);
}

void Morph::agregarReferencia(Morph* referencia){
	this->referencia = referencia;
}


Morph::Morph(){
}

void Morph::agregarSlot(std::string nombreSlot){
	//Morph* morph = new Morph(nombreSlot, this->posX, (this->posY)+(this->height), this->width, this->height);
	Slot* slot = new Slot(nombreSlot, this->posX, (this->posY)+(this->height), this->width, this->height);
	//this->height+=morph->height;
	this->height+=slot->height;
	
	// ver que hacer con el alto del slot.
	slots.push_back(slot);
	//slots.push_back(morph);
}

void Morph::agregarSlot(InterfaceSlot* interface_slot){

	Slot* slot = new Slot(interface_slot->get_name(), this->posX, (this->posY)+(this->height), this->width, this->height);
	this->height+=slot->height;
	
	std::cout << this ->height << std::endl;
	std::cout << this -> width << std::endl;

	slot->value = interface_slot->get_value();
	slots.push_back(slot);
}

void Morph::draw(const Cairo::RefPtr<Cairo::Context>& cr){

	cr->set_line_width(2.5);
	cr->set_source_rgb(0.0, 0.0, 0.0);
	
	cr -> rectangle(this->posX, this -> posY,this->width, this->height);
	//cr ->fill();
	cr-> stroke();

	for (int i=0; i < slots.size(); ++i){
		//slots[i] -> draw_slot(cr);
		slots[i] -> draw(cr);
		//morph->width += text_width;
		//this->height += slots[i]->height;
	}

	Pango::FontDescription font;
	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_BOLD);
	auto layout = create_pango_layout(this->nombreObjeto);

	int text_width;
	int text_height;

	layout->get_pixel_size(text_width, text_height);
	
	cr->move_to((this->posX)+(text_width/2), (this->posY)+(text_height/2));

	layout->show_in_cairo_context(cr);
}

Morph* Morph::clikEnObtenerSlot(int posX,int posY){     
	for (int i=0; i < slots.size(); ++i){         // guardar cuando agregue poli         
		if ((posX >= (slots[i]->posX) + (slots[i]->width) -10) && (posY >= (slots[i]->posY) + 2)
			&& (posX <= (((slots[i]->posX) + (slots[i]->width))-2)) && (posY <=
			((slots[i]->posY)+slots[i]->height-2))) {   
				Morph* morph = new Morph(slots[i]->value,slots[i]->posX + slots[i]->width +20, slots[i]->posY+4,m_TextView);             
				//Morph* morph = new (slots[i]->value,slots[i]->posX + slots[i]->width +20, slots[i]->posY+4,m_TextView);             
				
				morph -> referencia = new Morph(morph->posX-8,morph->posY,8,8,m_TextView);                   
				slots[i] -> referencia = morph->referencia;
				morph -> referencia -> slotPadre = morph;
				slots[i] -> referencia -> slotPadre = morph;
				return morph;         
			}     
		}     
	return nullptr; 
}

Slot* Morph::obtenerSlot(int posX,int posY){     
	for (int i=0; i < slots.size(); ++i){         // guardar cuando agregue poli         
		if ((posX >= (slots[i]->posX) + (slots[i]->width) -10) && (posY >= (slots[i]->posY) + 2)
			&& (posX <= (((slots[i]->posX) + (slots[i]->width))-2)) && (posY <=
			((slots[i]->posY)+slots[i]->height-2))) {   
				return slots[i];     
			}     
		}     
	return nullptr; 
}

Morph* Morph::clickEnReferenciaAMorph(int posX,int posY){
		/*if ((posX >= this->posX) && (posY >= this->posY)
	    		&& (posX <= ((this->posX)+8)) && (posY <= ((this->posY)+8)))
		{
			std::cout << " referencia a morph" << std::endl;		
      		//Morph* morph = new Morph("",this->posX-8,this->posY,m_TextView);
      		referencia=nullptr;
      		return referencia;
      	}*/
	return nullptr;
}

void Morph::mostrarDescripcionMorph(){
	//Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer1;
	//m_refTextBuffer1 = Gtk::TextBuffer::create();
	//refTextViewConsola->set_text("");
	m_TextView -> set_buffer(refTextViewConsola);
}

void Morph::actualizar_posicion(double x, double y){
	
	// para no dibujar fuera de la ventana
	/*if (x + width > 400 || y + height > 800){
		if (x + width > 400 ){
			this->posX = 400;
		} 
		if (y + height > 800 ){
			this -> posY = 800;
		}
		return;
	}
	if (x < 0 || y < 0){
		if (x < 0){
			this->posX = 0;
		}
		if (y < 0){
			this -> posY = 0;
		}
		return;
	}*/
	for (int i=0; i < slots.size(); ++i){
		std::cout << "actualizaar pos slot\n";
		slots[i] -> posX = x;
		slots[i] -> posY = y + (slots[i]->posY - this->posY);
		/*if(slots[i] -> referencia){
			std::cout << "actualizaar referencia\n";
			slots[i] ->referencia->posX=x-8;
			slots[i] ->referencia->posY=y;
		}*/
	}
	if(referencia != nullptr){
		referencia->posX=x-8;
		referencia->posY=y;
	}
	this->posX = x;
	this->posY = y;		
}



Morph::~Morph(){}
