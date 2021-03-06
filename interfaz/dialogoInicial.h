#ifndef DIALOGOINICIAL_H
#define DIALOGOINICIAL_H

#include "proxyServer.h"
#include <gtkmm.h>

class DialogoInicial : public Gtk::Dialog {
	protected:
		Glib::RefPtr<Gtk::Builder> m_builder;
		ProxyServer* proxy;
		sigc::connection sigcBotonOk;
	public:	
		DialogoInicial();
		// este constructor me obliga Glade a definirlo asi, si es que quiero tener mi clase.
		// https://developer.gnome.org/gtkmm-tutorial/stable/sec-builder-using-derived-widgets.html.en
		DialogoInicial(BaseObjectType* cobject, 	
					const Glib::RefPtr<Gtk::Builder>& refGlade);
		void setProxy(ProxyServer* proxy);
		void nuevoLobbyClick();
		void cargarLobbyClick();
		bool cerrarDialogoClick(GdkEventAny*);
		void salirClick();
		void botonOkLobby();
		virtual ~DialogoInicial();
};

#endif