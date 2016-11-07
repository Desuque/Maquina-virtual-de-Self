#ifndef MORPH_H
#define MORPH_H
class Slot;
class Referencia;
#include <iostream>
#include <gtkmm.h>
#include "../vm/client_interface_slot.h"
//#include "slot.h"
//#include "referencia.h"

class Morph : public Gtk::DrawingArea{
	public:
		std::string nombreObjeto;
		Gtk::TextView* m_TextView;
		//Glib::RefPtr<Gtk::Builder> m_builder;
		Glib::RefPtr<Gtk::TextBuffer> refTextViewConsola;
		Morph* referencia;
		Morph* slotPadre;
		//std::vector<Morph*> slots;
		std::vector<Slot*> slots;
		std::vector<Morph*> morphsReferenciados;
		int posX;
		int posY;
		int width;
		int height;
	public:
		Morph(double posX, double posY, int width, int height,Gtk::TextView* m_TextView);
		
		Morph(std::string nombreObjeto, double posX, double posY, int width, int height);
		
		Morph(std::string nombreObjeto, double posX, double posY) : 
			nombreObjeto(nombreObjeto), posX(posX), posY(posY), referencia(nullptr){
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
				nombreObjeto(nombreObjeto), posX(posX), posY(posY), m_TextView(m_TextView), referencia(nullptr) {
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
			//referencia = new Morph(posX-8,posY,8,8,m_TextView);
  		}
  		Morph* get_it(){
			std::string objeto;
			std::string mensaje;
			if (!m_TextView) std::cout << "error" << std::endl;
			refTextViewConsola = m_TextView -> get_buffer();
			std::string str = m_TextView->get_buffer()->get_text();
			std::istringstream iss(str);
			iss >> std::noskipws;
			iss >> objeto  >> mensaje;  
			std::cout << objeto << std::endl;
			//Morph* morph = nullptr;
			if (str != ""){
				//morph = new Morph("que?",200,200,m_TextView);
				this->nombreObjeto=objeto;
				
			}/* else {
				morph = new Morph(objeto,200,200,m_TextView);
            }*/

            // ENVIAR A SERVIDOR
            std::cout << this->nombreObjeto << " " << str
            		  << std::endl;
			
			return this;
  		}

  		void do_it(){
			std::string objeto;
			std::string mensaje;
			if (!m_TextView) std::cout << "error" << std::endl;
			refTextViewConsola = m_TextView -> get_buffer();
			std::string str = m_TextView->get_buffer()->get_text();
			std::istringstream iss(str);
			iss >> std::noskipws;
			iss >> objeto  >> mensaje;  
			std::cout << objeto << std::endl;
			// ENVIAR A SERVIDOR
            std::cout << this->nombreObjeto << " " << str
            		  << std::endl;
  		}
  		void agregarSlot(std::string nombreSlot);
  		// decidir como hacer esto.
  		void agregarSlot(InterfaceSlot* nombreSlot);
  		bool esUnaReferencia(){
  			// SOLUCIONAR
  			return (height==8 && width==8);
  		}

  		Slot* obtenerSlot(int posX,int posY);

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
	    bool operator!=(const Morph& rhs) const { 
	    	return !(*this==rhs);
	    }

		void draw(const Cairo::RefPtr<Cairo::Context>& cr);
		void draw_slot(const Cairo::RefPtr<Cairo::Context>& cr);

		void agregarReferencia(Morph* referencia);
		virtual ~Morph();
};

#endif