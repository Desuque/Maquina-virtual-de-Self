#ifndef Cliente_H
#define Cliente_H

#include "common_proxyServer.h"
#include "common_thread.h"

#define TAM_COD 1
#define ACTUALIZAR_VISTA 8

class Cliente : public Thread {
	private:
		ProxyServer& proxy;
	public:	
		Cliente(ProxyServer& proxy);

		virtual void run() {
			uint32_t codigo = 0;
			while (true){
				try {
					codigo = proxy.recibirCodigo(TAM_COD);
				} catch (const std::exception &e){
					// error de conexion.
				}

				switch (codigo){
					


					case ACTUALIZAR_VISTA : {
						//....
						break;
					}
					default:
						break;
				}
			}		
	    }

		~Cliente();
};

#endif