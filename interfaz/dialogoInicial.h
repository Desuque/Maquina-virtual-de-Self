#ifndef DIALOGOINICIAL_H
#define DIALOGOINICIAL_H

#include "common_proxyServer.h"
#include <gtkmm.h>

class DialogoInicial : public Gtk::Dialog {
	protected:
		Glib::RefPtr<Gtk::Builder> m_builder;
		ProxyServer* proxy;
		sigc::connection sigcBotonOk;
	public:	
		DialogoInicial();
		// Este Constructor me obliga Glade a definirlo asi.
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