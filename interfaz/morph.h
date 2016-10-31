#ifndef MORPH_H
#define MORPH_H

#include <iostream>
#include <gtkmm.h>

class Morph : public Gtk::DrawingArea{
	public:
		std::string nombreObjeto;
		Gtk::TextView* m_TextView;
		//Glib::RefPtr<Gtk::Builder> m_builder;
		Glib::RefPtr<Gtk::TextBuffer> refTextViewConsola;
		Morph* referencia = nullptr;
		std::vector<Morph*> slots;
		std::vector<Morph*> morphsReferenciados;
		int posX;
		int posY;
		int width;
		int height;
	public:
		Morph(double posX, double posY, int width, int height,Gtk::TextView* m_TextView);

		Morph(std::string nombreObjeto, double posX, double posY) : 
				nombreObjeto(nombreObjeto), posX(posX), posY(posY){
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
					refTextViewConsola = Gtk::TextBuffer::create();
					refTextViewConsola->set_text("");
		}

		Morph(std::string nombreObjeto, double posX, double posY, 	Gtk::TextView* m_TextView) : 
				nombreObjeto(nombreObjeto), posX(posX), posY(posY), m_TextView(m_TextView){
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
			refTextViewConsola = Gtk::TextBuffer::create();
			refTextViewConsola->set_text("");
  		}
  		Morph* get_it(){
			std::cout << "Action 1 selected" << std::endl;
			std::string objeto;
			std::string mensaje;
			if (!m_TextView) std::cout << "error" << std::endl;
			refTextViewConsola = m_TextView -> get_buffer();
			std::string str = m_TextView->get_buffer()->get_text();
			std::istringstream iss(str);
			iss >> std::noskipws;
			iss >> objeto  >> mensaje;  
			std::cout << objeto << std::endl;
			if (str == "") objeto = "que?!";
			return new Morph(objeto,200,200,m_TextView);
  		}

  		void do_it(){
			std::cout << "Action 1 selected" << std::endl;
			std::string objeto;
			std::string mensaje;
			if (!m_TextView) std::cout << "error" << std::endl;
			refTextViewConsola = m_TextView -> get_buffer();
			std::string str = m_TextView->get_buffer()->get_text();
			std::istringstream iss(str);
			iss >> std::noskipws;
			iss >> objeto  >> mensaje;  
			std::cout << objeto << std::endl;
  		}
  		void agregarSlot(std::string nombreSlot);
  		Morph* clikEnObtenerSlot(int posX,int posY);
  		Morph* clickEnReferenciaAMorph(int posX,int posY);
		Morph(double posX, double posY);
		Morph();
		void mostrarDescripcionMorph();
		void actualizar_posicion(double x, double y);
	    bool operator==(const Morph& rhs) const { 
	    	if ((rhs.posX >= this->posX) && (rhs.posY >= this->posY)
	    		&& (rhs.posX <= ((this->posX) + this->width)) && (rhs.posY <= ((this->posY)+this->height))){
	    		return true;
	    	}
	    	return false;
	    }

		void draw(const Cairo::RefPtr<Cairo::Context>& cr);
		void draw_slot(const Cairo::RefPtr<Cairo::Context>& cr);
		~Morph();
};

#endif