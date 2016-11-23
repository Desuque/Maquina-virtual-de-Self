#ifndef Cliente_H
#define Cliente_H

#include "common_proxyServer.h"
#include "common_thread.h"
#include "myarea.h"
#include "client_json_reader.h"
#include "client_interface_slot.h"



class Cliente : public Thread {
	private:
		ProxyServer& proxy;
		MyArea* myArea;

	public:	
		Cliente(ProxyServer& proxy, MyArea* myArea);

		virtual void run();

		~Cliente();
};

#endif