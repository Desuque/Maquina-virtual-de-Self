
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
		void enviarSlots(std::string slots);
		//void enviar(std::string mensaje, size_t cantidad);
		void enviar(uint32_t entero, size_t cantidad);
		std::string recibir(size_t cantidad);
		uint32_t recibirCodigoMensaje(size_t cantidad);
		uint32_t recibirTamMensaje(size_t cantidad);
		ProxyClient(ProxyClient&& other);
		ProxyClient& operator=(ProxyClient&& other);
		~ProxyClient();
};

#endif
