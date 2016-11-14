#ifndef MORPH_H
#define MORPH_H
class Slot;
class Referencia;
#include <iostream>
#include <gtkmm.h>
#include "client_interface_slot.h"
#include "common_proxyServer.h"
//#include "slot.h"
//#include "referencia.h"

class Morph : public Gtk::DrawingArea{
	public:
		std::string nombreObjeto;
		Gtk::TextView* m_TextView;
		Gtk::TextView* textViewCodigoAsociado;
		//Glib::RefPtr<Gtk::Builder> m_builder;
		Glib::RefPtr<Gtk::TextBuffer> refTextViewConsola;
		Glib::RefPtr<Gtk::TextBuffer> refTextViewCodigoAsociado;
		std::vector<Referencia*> referencias;
		//std::vector<Morph*> slots;
		std::vector<Slot*> slots;
		int posX;
		int posY;
		int width;
		int height;
		int id;
	public:
		Morph(double posX, double posY, int width, int height,Gtk::TextView* m_TextView);
		Morph(std::string nombreObjeto, double posX, double posY, int width, int height);
		Morph(std::string nombreObjeto, double posX, double posY);
		Morph(std::string nombreObjeto, int id, double posX, double posY, 
			Gtk::TextView* m_TextView, Gtk::TextView* codAsociado);
		
		void borrarReferencia(Referencia* referencia){
			for (int i=0; i < referencias.size(); ++i){
				if (referencias[i] == referencia){
					referencias.erase(referencias.begin()+i);
				}
			}
		}
		std::string get_id_to_string();
		

  		std::string get_it();
  		std::string do_it();
  		void actualizarAlturaMorph(size_t alturaDeSlot);
  		void agregarSlot(std::string nombreSlot);
  		// decidir como hacer esto.
  		void agregarSlot(InterfaceSlot* nombreSlot);

  		Slot* obtenerSlot(int posX,int posY);

  		Morph* clikEnObtenerSlot(int posX,int posY);
  		Morph* clickEnReferenciaAMorph(int posX,int posY);
		Morph(double posX, double posY);
		Morph();
		void mostrarDescripcionMorph();
		void actualizar_posicion(double x, double y);

		void draw(const Cairo::RefPtr<Cairo::Context>& cr);
		void draw_slot(const Cairo::RefPtr<Cairo::Context>& cr);

	    bool operator==(const Morph& rhs) const {
	    	std::cout << "se comparan dos morphs\n";
	    	if ((rhs.posX >= this->posX) && (rhs.posY >= this->posY)
	    		&& (rhs.posX <= ((this->posX) + this->width)) 
	    		&& (rhs.posY <= ((this->posY)+this->height))){
	    		return true;
	    	}
	    	return false;
	    }
	    bool operator!=(const Morph& rhs) const { 
	    	return !(*this==rhs);
	    }

		virtual ~Morph();
};

#endif