#include "common_proxy.h"
#include "common_socket.h"

#ifndef PROXYSERVER_H_
#define PROXYSERVER_H_

class ProxyServer : public Proxy {
	private:
		Socket sktCliente;
	public:
		ProxyServer(){}
		ProxyServer(const char* hostname, const unsigned int puerto);
		void connect(const char* hostname, const unsigned int puerto);
		std::string recibir(size_t cantidad);
		uint32_t recibirId(size_t cantidad);
		uint32_t enviarString(std::string textoAEnviar);
		std::string recibirJson();
		void enviarJson(std::string jsonAEnviar);
		void enviar(uint32_t entero, size_t cantidad);
		uint32_t recibirCodigo(size_t cantidad);
		uint32_t recibirTamanioDeMensaje(size_t cantidad);
		// habria que sacarla
		std::string recibirSlotsDe(std::string lobbyId);
		void pedirSlotsDe(std::string idObjeto);
		uint32_t recibirCodigoMensaje(size_t cantidad);
		void enviarCodigoAEjecutar(std::string idObjeto, std::string textoAEnviar);
		std::string agregarSlotA(std::string idObjeto, std::string textoAEnviar);
		ProxyServer(Socket& socket);
		ProxyServer(ProxyServer&& other);
		ProxyServer& operator=(ProxyServer&& other);
		void cerrarConexion();
		~ProxyServer();
};

#endif
