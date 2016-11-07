#ifndef SLOT_H
#define SLOT_H
#include "morph.h"
#include "string.h"

class Slot : public Morph{
public:
	//std::string name;
	//Morph* morph;
	Morph* referencia = nullptr;
	int posX;
	int posY;
	int width;
	int height;

	/* interface Slot */
	bool type;
	bool code;
	std::string name;
	std::string value;
public:	
	Slot(){}
	
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
		/*refTextViewConsola = Gtk::TextBuffer::create();
		refTextViewConsola->set_text("");*/
		//referencia = new Morph(posX-8,posY,8,8,m_TextView);
	}

	void draw(const Cairo::RefPtr<Cairo::Context>& cr){
		Pango::FontDescription font;

		font.set_family("Monospace");
		font.set_weight(Pango::WEIGHT_LIGHT);

		// http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
		auto layout = create_pango_layout(this->name);

		layout->set_font_description(font);

		int text_width,text_height;
		layout->get_pixel_size(text_width, text_height);
		
		//this->height = text_height;

		cr -> rectangle(this->posX , this->posY, this->width, this->height);
		cr->set_line_width(0.5);
		//cr-> stroke_preserve();
		cr -> rectangle(this->posX + this->width - 10, this->posY + 2, 8, this->height - 4);
		//cr-> stroke();
		
		if(this->referencia){
			cr->move_to(this->posX + this->width - 6, this->posY + (this->height/2));
			cr -> line_to(this->referencia->posX,this->referencia->posY+4);
		}

		cr->stroke();

		cr->move_to(this->posX, this->posY);

		layout->show_in_cairo_context(cr);
	}
	
	virtual ~Slot(){}
};

#endif