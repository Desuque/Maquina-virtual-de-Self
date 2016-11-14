#include "common_proxyServer.h"
#include "common_proxy.h"
#include "common_socket.h"
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>

#define PEDIR_SLOTS 2
#define AGREGAR_SLOT 3

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

uint32_t ProxyServer::recibirCodigoMensaje(size_t cantidad){
	return Proxy::recibirCantidad(sktCliente,cantidad);
}

uint32_t ProxyServer::enviarCodigoAEjecutar(std::string idObjeto, std::string textoAEnviar){
	// modificar cuando sea generico
	
    this->enviar(5,sizeof(char));

	char buff[5];
	bzero(buff,5);
	uint32_t tamMensaje = idObjeto.length();
	tamMensaje = htonl(tamMensaje);
	memcpy(buff,&tamMensaje ,sizeof(uint32_t));
	
	sktCliente.send(buff,sizeof(uint32_t));

	sktCliente.send(idObjeto.c_str(), idObjeto.length());

	//repito codigo luego
	tamMensaje = textoAEnviar.length();
	bzero(buff,5);
	tamMensaje = htonl(tamMensaje);
	memcpy(buff,&tamMensaje ,sizeof(uint32_t));
	
	sktCliente.send(buff,sizeof(uint32_t));

	sktCliente.send(textoAEnviar.c_str(), textoAEnviar.length());

	//std::cout << recibir(1) << std::endl;
	return recibirCodigoMensaje(1);
	/*tamMensaje = 0;
	bzero(buff,5);
	sktCliente.receive(buff, sizeof(tamMensaje));
	memcpy(&tamMensaje, buff,sizeof(tamMensaje));
	tamMensaje = ntohl(tamMensaje);

	//char infoSlots[tamMensaje+1];
	//bzero(infoSlots,tamMensaje+1);
	char infoSlots[1024];
	bzero(infoSlots,1024);

	sktCliente.receive(infoSlots,tamMensaje);

	return std::string(infoSlots);*/

	//return agregarSlotA(idObjeto, textoAEnviar);
}

std::string ProxyServer::agregarSlotA(std::string idObjeto, std::string textoAEnviar){
	char buff[5];
	bzero(buff,5);

	/*uint32_t idMensaje = PEDIR_SLOTS;
	memcpy(buff,&idMensaje ,sizeof(idMensaje));
	sktCliente.send(buff,sizeof(idMensaje));*/

    this->enviar(AGREGAR_SLOT,sizeof(char));

	uint32_t tamMensaje = idObjeto.length();
	bzero(buff,5);
	tamMensaje = htonl(tamMensaje);
	memcpy(buff,&tamMensaje ,sizeof(uint32_t));
	
	sktCliente.send(buff,sizeof(uint32_t));

	sktCliente.send(idObjeto.c_str(), idObjeto.length());

	//repito codigo luego
	tamMensaje = textoAEnviar.length();
	bzero(buff,5);
	tamMensaje = htonl(tamMensaje);
	memcpy(buff,&tamMensaje ,sizeof(uint32_t));
	
	sktCliente.send(buff,sizeof(uint32_t));

	sktCliente.send(textoAEnviar.c_str(), textoAEnviar.length());

	tamMensaje = 0;
	bzero(buff,5);
	sktCliente.receive(buff, sizeof(tamMensaje));
	memcpy(&tamMensaje, buff,sizeof(tamMensaje));
	tamMensaje = ntohl(tamMensaje);

	/*char infoSlots[tamMensaje+1];
	bzero(infoSlots,tamMensaje+1);*/
	char infoSlots[1024];
	bzero(infoSlots,1024);

	sktCliente.receive(infoSlots,tamMensaje);

	return std::string(infoSlots);
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

	/*char infoSlots[tamMensaje+1];
	bzero(infoSlots,tamMensaje+1);*/
	char infoSlots[1024];
	bzero(infoSlots,1024);

	sktCliente.receive(infoSlots,tamMensaje);

	return std::string(infoSlots);
}


void ProxyServer::enviar(uint32_t entero, size_t cantidad){
	Proxy::enviarCantidad(sktCliente, entero, cantidad);
}

std::string ProxyServer::recibir(size_t cantidad){
	return Proxy::recibir(sktCliente,cantidad);
}

uint32_t ProxyServer::recibirCodigoRespuesta(size_t cantidad){
	return Proxy::recibirCantidad(sktCliente,cantidad);
}

ProxyServer::~ProxyServer(){
	sktCliente.shutdown();
}
