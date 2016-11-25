#ifndef DIALOGOSELECCIONARLOBBY_H
#define DIALOGOSELECCIONARLOBBY_H

#include "listaDeLobbys.h"
#include "common_proxyServer.h"
#include <gtkmm.h>

class DialogoSeleccionLobby : public Gtk::Dialog {
	protected:
		Glib::RefPtr<Gtk::Builder> m_builder;
		//ListaDeLobbys* listaDeLobbys;
		//ProxyServer* proxy;
	public:	
		DialogoSeleccionLobby();
		DialogoSeleccionLobby(BaseObjectType* cobject, 	
					const Glib::RefPtr<Gtk::Builder>& refGlade);
		/*void setProxy(ProxyServer* proxy){
			this->proxy = proxy;
		}*/
		void setListaLobbys(std::vector<std::string> listaLobbys);
		virtual ~DialogoSeleccionLobby();
};

#endif