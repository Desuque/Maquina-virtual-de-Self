#ifndef DIALOGOINICIAL_H
#define DIALOGOINICIAL_H

#include "common_proxyServer.h"
#include <gtkmm.h>

class DialogoInicial : public Gtk::Dialog {
	protected:
		Glib::RefPtr<Gtk::Builder> m_builder;
		ProxyServer* proxy;
	public:	
		DialogoInicial();
		DialogoInicial(BaseObjectType* cobject, 	
					const Glib::RefPtr<Gtk::Builder>& refGlade);
		DialogoInicial(BaseObjectType* cobject, 
					const Glib::RefPtr<Gtk::Builder>& refGlade,
					bool is_glad);
		void setProxy(ProxyServer* proxy);
		void nuevoLobbyClick();
		void cargarLobbyClick();
		bool cerrarDialogoClick(GdkEventAny*);
		virtual ~DialogoInicial();
};

#endif