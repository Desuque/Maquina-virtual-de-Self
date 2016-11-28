#ifndef DIALOGONOMBRELOBBY_H
#define DIALOGONOMBRELOBBY_H

#include "common_proxyServer.h"
#include <gtkmm.h>

class DialogoNombreLobby : public Gtk::Dialog {
	protected:
		Glib::RefPtr<Gtk::Builder> m_builder;
		ProxyServer* proxy;
	public:	
		DialogoNombreLobby();
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