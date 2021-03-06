#ifndef MORPH_H
#define MORPH_H
class Slot;
class Referencia;
#include <iostream>
#include <gtkmm.h>
#include "client_interface_slot.h"
#include "proxyServer.h"

class Morph : public Gtk::DrawingArea{	
	private:
		std::string nombreObjeto;
		std::string nombreParaMostrar;
		Gtk::TextView* m_TextView;
		Gtk::TextView* textViewCodigoAsociado;
		Glib::RefPtr<Gtk::TextBuffer> refTextViewConsola;
		Glib::RefPtr<Gtk::TextBuffer> refTextViewCodigoAsociado;
		std::vector<Referencia*> referencias;
		std::vector<Slot*> slots;
	
	protected:
		double posX;
		double posY;
		int width;
		int height;
		int id;
	
	public:
		Morph();
		Morph(double posX, double posY);
		Morph(Slot* unSlot);
		Morph(Slot* unSlot, Gtk::TextView* m_TextView, Gtk::TextView* codAsociado);
		Morph(std::string nombreObjeto, int id, double posX, double posY, 
			Gtk::TextView* m_TextView, Gtk::TextView* codAsociado);
		Morph(InterfaceSlot* unSlot,double posX, double posY, 
				Gtk::TextView* m_TextView, Gtk::TextView* codigoAsociado);
		
		void borrarEstaReferencia(Referencia* referencia);
		std::string get_id_to_string();
		int get_id();
  		double getPosX();
  		double getPosY();
  		int getWidth();
		int getHeight();
		void resaltarMorph(const Cairo::RefPtr<Cairo::Context>& cr);
  		std::string get_it();
  		std::string do_it();
  		void actualizarAlturaMorph(size_t alturaDeSlot);
  		void agregarSlot(InterfaceSlot* nombreSlot);
		std::vector<Slot*>& getSlots();
		std::vector<Referencia*> getReferencias();
  		Slot* obtenerSlot(int posX,int posY);
  		Slot* obtenerSlotConId(int id_slot);    
  		std::vector<Slot*> obtenerSlotsConId(int id_slot);    
		Slot* obtenerSlotConEsteNombre(std::string nombre);
 		
 		std::string getNombreParaMostrar();
 		std::string getNombre();

		void mostrarDescripcionMorph();
		void actualizar_posicion(double x, double y);

		void draw(const Cairo::RefPtr<Cairo::Context>& cr);

	    bool operator==(const Morph& rhs) const {
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
		void actualizarValores(Slot* unSlot);
	    bool tieneElMismoIdQueEsteSlot(Slot* unSlot);
	    bool tieneElMismoIdQueEsteSlot(InterfaceSlot* unSlot);
	    void agregarReferencia(Referencia* unaReferencia);

		virtual ~Morph();
};

#endif