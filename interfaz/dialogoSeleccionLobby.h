#ifndef DIALOGOSELECCIONARLOBBY_H
#define DIALOGOSELECCIONARLOBBY_H

#include "common_proxyServer.h"
#include <gtkmm.h>

class DialogoSeleccionLobby : public Gtk::Dialog {
	protected:
		Glib::RefPtr<Gtk::Builder> m_builder;
		ProxyServer* proxy;
	public:	
		DialogoSeleccionLobby();
		DialogoSeleccionLobby(BaseObjectType* cobject, 	
					const Glib::RefPtr<Gtk::Builder>& refGlade);
		DialogoSeleccionLobby(BaseObjectType* cobject, 
					const Glib::RefPtr<Gtk::Builder>& refGlade,
					bool is_glad);
		void setProxy(ProxyServer* proxy){
			this->proxy = proxy;
		}
		void botonOkLobby();
		virtual ~DialogoSeleccionLobby();
};

#endif