#ifndef Cliente_H
#define Cliente_H

#include "common_proxyServer.h"
#include "common_thread.h"
#include "myarea.h"
#include "client_json_reader.h"
#include "client_interface_slot.h"

#define TAM_COD 1
#define ACTUALIZAR_VISTA 8
#define GET_IT 9
#define DO_IT 10

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