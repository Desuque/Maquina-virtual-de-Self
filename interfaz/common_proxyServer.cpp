#include "common_proxyServer.h"
#include "common_proxy.h"
#include "common_socket.h"
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>

#define PEDIR_SLOTS 2

ProxyServer::ProxyServer(const char* hostname, const unsigned int puerto){
	sktCliente.connect(hostname,puerto);
}

void ProxyServer::connect(const char* hostname, const unsigned int puerto){
	sktCliente.connect(hostname,puerto);	
}

ProxyServer::ProxyServer(Socket& socket):sktCliente(std::move(socket)){}

ProxyServer::ProxyServer(ProxyServer&& other):sktCliente(std::move(other.sktCliente)){}

ProxyServer& ProxyServer::operator=(ProxyServer&& other) {
	this->sktCliente = std::move(other.sktCliente);
	return *this;
}

uint32_t ProxyServer::recibirId(size_t cantidad){
	uint32_t lobbyId = 0;
	char buff[5];
	bzero(buff,5);
	sktCliente.receive(buff, sizeof(lobbyId));
	memcpy(&lobbyId, buff,sizeof(lobbyId));
	lobbyId = ntohl(lobbyId);
	return lobbyId;
}

std::string ProxyServer::recibirSlotsDe(std::string objeto){
	char buff[5];
	bzero(buff,5);

	/*uint32_t idMensaje = PEDIR_SLOTS;
	memcpy(buff,&idMensaje ,sizeof(idMensaje));
	sktCliente.send(buff,sizeof(idMensaje));*/

    this->enviar(PEDIR_SLOTS,sizeof(char));

	uint32_t tamMensaje = objeto.length();
	bzero(buff,5);
	tamMensaje = htonl(tamMensaje);
	memcpy(buff,&tamMensaje ,sizeof(uint32_t));
	
	sktCliente.send(buff,sizeof(uint32_t));

	sktCliente.send(objeto.c_str(), objeto.length());

	tamMensaje = 0;
	bzero(buff,5);
	sktCliente.receive(buff, sizeof(tamMensaje));
	memcpy(&tamMensaje, buff,sizeof(tamMensaje));
	tamMensaje = ntohl(tamMensaje);

	char infoSlots[tamMensaje+1];
	bzero(infoSlots,tamMensaje+1);

	sktCliente.receive(infoSlots,tamMensaje);

	return std::string(infoSlots);
}


void ProxyServer::enviar(uint32_t entero, size_t cantidad){
	Proxy::enviarCantidad(sktCliente, entero, cantidad);
}

std::string ProxyServer::recibir(size_t cantidad){
	return Proxy::recibir(sktCliente,cantidad);
}

ProxyServer::~ProxyServer(){
	sktCliente.shutdown();
}