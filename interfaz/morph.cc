#include "morph.h"
#include <gtkmm.h>
#include <pangomm/layout.h>

Morph::Morph(double posX, double posY){
	this->posX = posX;
	this->posY = posY;
	this -> nombreObjeto = "que?";
}

Morph::Morph(double posX, double posY, int width, int height,Gtk::TextView* m_TextView){
	this->posX = posX;
	this->posY = posY;
	this->width = width;
	this->height = height;
	this -> nombreObjeto = "";
	refTextViewConsola = Gtk::TextBuffer::create();
	refTextViewConsola->set_text("");
}

Morph::Morph(){
}

void Morph::agregarSlot(std::string nombreSlot){
	Morph* morph = new Morph(nombreSlot, this->posX,(this->posY)+(this->height));
	morph -> width = this -> width;
	// ver que hacer con el alto del slot.
	slots.push_back(morph);
}

void Morph::draw(const Cairo::RefPtr<Cairo::Context>& cr){

	Pango::FontDescription font;

	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_BOLD);

	// http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
	auto layout = create_pango_layout(this->nombreObjeto);

	layout->set_font_description(font);

	int text_width;
	int text_height;

	layout->get_pixel_size(text_width, text_height);

	this->width = 2*text_width;
	this->height = 2*text_height;

	cr->set_line_width(2.5);
	cr->set_source_rgb(0.0, 0.0, 0.0);
	
	cr -> rectangle(this->posX, this -> posY,this->width, this->height);
	//cr ->fill();
	cr-> stroke();

	for (int i=0; i < slots.size(); ++i){
		slots[i] -> draw_slot(cr);
		//morph->width += text_width;
		this->height += slots[i]->height;
	}
	
	cr->move_to((this->posX)+(text_width/2), (this->posY)+(text_height/2));

	layout->show_in_cairo_context(cr);
}

Morph* Morph::clikEnObtenerSlot(int posX,int posY){
	for (int i=0; i < slots.size(); ++i){
		// guardar cuando agregue poli
		if ((posX >= (slots[i]->posX) + (slots[i]->width) -10) && (posY >= (slots[i]->posY) + 2)
	    	&& (posX <= (((slots[i]->posX) + (slots[i]->width))-2)) && (posY <= ((slots[i]->posY)+slots[i]->height-2)))
		{		
		//this->posX + this->width - 10, this->posY + 2, 8, text_height - 4
		//if(*(slots[i]) == Morph(posX,posY)){
     		//return slots[i]; 		
     		// DECIDIR COMO HACER CON EL TEXT VIEW
      		//slots[i]->agregarReferenciaA()
      		Morph* morph = new Morph(slots[i]->nombreObjeto,slots[i]->posX + slots[i]->width + 20,slots[i]->posY+4,m_TextView);
      		//morph->posX-8,morph->posY , 8, 8
      		slots[i] -> morphsReferenciados.push_back(morph);		
      		return morph;
      	}
	}
	return nullptr;
}

Morph* Morph::clickEnReferenciaAMorph(int posX,int posY){
		if ((posX >= this->posX-8) && (posY >= this->posY)
	    		&& (posX <= ((this->posX))) && (posY <= ((this->posY)+8)))
		{
			std::cout << " referencia a morph" << std::endl;		
      		Morph* morph = new Morph("",this->posX-8,this->posY,m_TextView);
      		return morph;
      	}
	return nullptr;
}

// iria en la clase Slot.
void Morph::draw_slot(const Cairo::RefPtr<Cairo::Context>& cr){
	Pango::FontDescription font;

	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_LIGHT);

	// http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
	auto layout = create_pango_layout(this->nombreObjeto);

	layout->set_font_description(font);

	int text_width,text_height;
	layout->get_pixel_size(text_width, text_height);
	
	this->height = text_height;

	cr -> rectangle(this->posX , this->posY, this->width, this->height);
	cr->set_line_width(0.5);
	//cr-> stroke_preserve();
	cr -> rectangle(this->posX + this->width - 10, this->posY + 2, 8, this->height - 4);
	cr-> stroke();
		
	for (int i=0; i < morphsReferenciados.size(); ++i){
		Morph* morph = morphsReferenciados[i];
		cr->move_to(this->posX + this->width - 6, this->posY + (this->height/2));
		//cr -> line_to(morph->posX,morph->posY+4);
		//cr->rectangle(morph->posX,morph->posY , 10, 10);
		cr -> line_to(morph->posX-8,morph->posY+4);
		cr->rectangle(morph->posX-8,morph->posY , 8, 8);
	}
	
	cr-> stroke();

	cr->move_to(this->posX, this->posY);

	layout->show_in_cairo_context(cr);
}

void Morph::mostrarDescripcionMorph(){
	//Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer1;
	//m_refTextBuffer1 = Gtk::TextBuffer::create();
	//refTextViewConsola->set_text("");
	m_TextView -> set_buffer(refTextViewConsola);
}

void Morph::actualizar_posicion(double x, double y){
	
	// para no dibujar fuera de la ventana
	/*if (x > 400 + width || y + height > 800){
		if (x > 400 + width){
			this->posX = 400 + width;
		}
		if (x > 400 + width){
			this -> posY = 800;
		}
		return;
	}
	if (x < 0 || y < 0){
		return;
	}*/
	for (int i=0; i < slots.size(); ++i){
		slots[i] -> posX = x;
		slots[i] -> posY = y + (slots[i]->posY - this->posY) ;
	}
	this->posX = x;
	this->posY = y;
		
}

Morph::~Morph(){}
