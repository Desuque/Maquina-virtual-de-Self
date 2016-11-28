#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <gtkmm.h>
#include "common_proxyServer.h"

class VentanaPrincipal : public Gtk::Window {

	protected:
		Glib::RefPtr<Gtk::Builder> m_builder;
		ProxyServer* proxy;
	public:
		// este constructor me obliga Glade a definirlo asi, si es que quiero tener mi clase.
		// https://developer.gnome.org/gtkmm-tutorial/stable/sec-builder-using-derived-widgets.html.en
		VentanaPrincipal(BaseObjectType* cobject, 	
						const Glib::RefPtr<Gtk::Builder>& refGlade);
		VentanaPrincipal();
		void iniciarDialogos();
		bool onWindowDelete(GdkEventAny*);
		void on_button_close();
		void setProxy(ProxyServer* proxy);
		virtual ~VentanaPrincipal();
};

#endif