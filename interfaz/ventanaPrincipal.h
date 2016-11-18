#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <gtkmm.h>

class VentanaPrincipal : public Gtk::Window {

	protected:
		Glib::RefPtr<Gtk::Builder> m_builder;

	public:
		VentanaPrincipal(BaseObjectType* cobject, 	
						const Glib::RefPtr<Gtk::Builder>& refGlade);
		VentanaPrincipal(BaseObjectType* cobject, 
						const Glib::RefPtr<Gtk::Builder>& refGlade,
						bool is_glad);
		VentanaPrincipal();
		bool onWindowDelete(GdkEventAny*);
		void on_button_close();
		virtual ~VentanaPrincipal();
};

#endif