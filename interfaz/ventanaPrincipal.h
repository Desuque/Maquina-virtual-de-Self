#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <gtkmm.h>
#include "common_proxyServer.h"
class VentanaPrincipal : public Gtk::Window {

	protected:
		Glib::RefPtr<Gtk::Builder> m_builder;
		ProxyServer* proxy;
	public:
		VentanaPrincipal(BaseObjectType* cobject, 	
						const Glib::RefPtr<Gtk::Builder>& refGlade);
		VentanaPrincipal(BaseObjectType* cobject, 
						const Glib::RefPtr<Gtk::Builder>& refGlade,
						bool is_glad);
		VentanaPrincipal();
		void iniciarDialogos();
		bool onWindowDelete(GdkEventAny*);
		void on_button_close();
		void setProxy(ProxyServer* proxy);
		virtual ~VentanaPrincipal();
};

#endif