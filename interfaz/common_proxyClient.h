
#include "common_socket.h"
#include "common_proxy.h"

#ifndef PROXYCLIENT_H_
#define PROXYCLIENT_H_

class ProxyClient : public Proxy {
	private:
		Socket server;
	public:
		ProxyClient(){}
		explicit ProxyClient(const unsigned int puerto);
		explicit ProxyClient(Socket& socket);
		void bindAndListen(const unsigned int puerto);
        void aceptarCliente(ProxyClient* proxy);
        void set_socket(Socket& socket);
		void cerrarConexion();
		
		void enviarJson(std::string json);
		std::string recibirJson();
		uint32_t recibirCodigoMensaje();
		void enviarCodigoMensaje(uint32_t codigoMensaje);
		uint32_t recibirTamMensaje();

		ProxyClient(ProxyClient&& other);
		ProxyClient& operator=(ProxyClient&& other);
		~ProxyClient();
};

#endif
