#include "common_socket.h"

#ifndef PROXYSERVER_H_
#define PROXYSERVER_H_

class ProxyServer {
	private:
		Socket sktCliente;
	public:
		ProxyServer(){}
		ProxyServer(const char* hostname, const unsigned int puerto);
		void connect(const char* hostname, const unsigned int puerto);
		
		void enviarJson(std::string jsonAEnviar);
		std::string recibirJson();
		uint32_t recibirCodigoMensaje();
		void enviarCodigoMensaje(uint32_t codigoMensaje);
		uint32_t recibirTamanioDeMensaje();
		void enviarCodigoAEjecutar(std::string idObjeto, std::string textoAEnviar, int accion);
		std::string recibirSlotsDe(std::string lobbyId);
		void pedirMorphDeEsteSlot(int id_morph, int id_slot);
		void pedirSlotsDe(std::string idObjeto);
		
		ProxyServer(Socket& socket);
		ProxyServer(ProxyServer&& other);
		ProxyServer& operator=(ProxyServer&& other);
		void cerrarConexion();
		~ProxyServer();
};

#endif
