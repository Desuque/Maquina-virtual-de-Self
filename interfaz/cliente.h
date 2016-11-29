#ifndef Cliente_H
#define Cliente_H

#include "proxyServer.h"
#include "common_thread.h"
#include "areaDeMorphs.h"
#include "client_json_reader.h"
#include "client_interface_slot.h"



class Cliente : public Thread {
	private:
		ProxyServer& proxy;
		AreaDeMorphs* areaDeMorphs;

	public:	
		Cliente(ProxyServer& proxy, AreaDeMorphs* areaDeMorphs);

		virtual void run();

		~Cliente();
};

#endif