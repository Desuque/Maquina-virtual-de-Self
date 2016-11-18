#ifndef DIALOGO_H
#define DIALOGO_H

#include "common_proxyServer.h"
#include <gtkmm.h>

class Dialogo : public Gtk::Dialog {
	protected:
		ProxyServer* proxy;

	public:	
		Dialogo();
		void setProxy(ProxyServer* proxy){
			this->proxy = proxy;
		}
		virtual ~Dialogo();
};

#endif