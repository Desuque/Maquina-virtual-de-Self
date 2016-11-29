#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <gtkmm.h>
#include "proxyServer.h"

class VentanaPrincipal : public Gtk::Window {

	protected:
		Glib::RefPtr<Gtk::Builder> m_builder;
		Gtk::Label* labelNombreLobby;
		ProxyServer* proxy;
	public:
		// este constructor me obliga Glade a definirlo asi, si es que quiero tener mi clase.
		// https://developer.gnome.org/gtkmm-tutorial/stable/sec-builder-using-derived-widgets.html.en
		VentanaPrincipal(BaseObjectType* cobject, 	
						const Glib::RefPtr<Gtk::Builder>& refGlade);
		VentanaPrincipal();
		void iniciarDialogos();
		// para detectar el click de la ventana principal
		// y asi poder liberar recursos.
		bool onWindowDelete(GdkEventAny*);
		void on_button_close();
		void setProxy(ProxyServer* proxy);
		void setLabel(std::string nombreLobby);
		virtual ~VentanaPrincipal();
};

#endif