#ifndef DIALOGONOMBRELOBBY_H
#define DIALOGONOMBRELOBBY_H

#include "proxyServer.h"
#include <gtkmm.h>

class DialogoNombreLobby : public Gtk::Dialog {
	protected:
		Glib::RefPtr<Gtk::Builder> m_builder;
		ProxyServer* proxy;
	public:	
		DialogoNombreLobby();
		// este constructor me obliga Glade a definirlo asi, si es que quiero tener mi clase.
		// https://developer.gnome.org/gtkmm-tutorial/stable/sec-builder-using-derived-widgets.html.en
		DialogoNombreLobby(BaseObjectType* cobject, 	
					const Glib::RefPtr<Gtk::Builder>& refGlade);
		void setProxy(ProxyServer* proxy){
			this->proxy = proxy;
		}
		void botonOkNombre();
		void botonSalir();
		virtual ~DialogoNombreLobby();
};

#endif