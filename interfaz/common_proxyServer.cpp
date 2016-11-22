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

uint32_t ProxyServer::enviarString(std::string textoAEnviar){
	char buff[5];
	bzero(buff,5);
	uint32_t tamMensaje = textoAEnviar.length();
	tamMensaje = htonl(tamMensaje);
	memcpy(buff,&tamMensaje ,sizeof(uint32_t));
	
	sktCliente.send(buff,sizeof(uint32_t));

	sktCliente.send(textoAEnviar.c_str(), textoAEnviar.length());

	//std::cout << recibir(1) << std::endl;
	return recibirCodigoMensaje(1);
}
void ProxyServer::enviarJson(std::string textoAEnviar){
	char buff[5];
	bzero(buff,5);
	uint32_t tamMensaje = textoAEnviar.length();
	tamMensaje = htonl(tamMensaje);
	memcpy(buff,&tamMensaje ,sizeof(uint32_t));
	
	sktCliente.send(buff,sizeof(uint32_t));

	sktCliente.send(textoAEnviar.c_str(), textoAEnviar.length());
}


void ProxyServer::enviarCodigoAEjecutar(std::string idObjeto, std::string textoAEnviar){
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

	//return recibirCodigoMensaje(1);
	
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
	//char infoSlots[1024];
	char* infoSlots = new char[tamMensaje+1];
	bzero(infoSlots,tamMensaje+1);

	sktCliente.receive(infoSlots,tamMensaje);

	std::string str_infoSlots = std::string(infoSlots);

	delete[] infoSlots;
	return str_infoSlots;
}

std::string ProxyServer::recibirJson(){
	char buff[5];
	bzero(buff,5);
	uint32_t tamMensaje = 0;
	
	sktCliente.receive(buff, sizeof(tamMensaje));
	memcpy(&tamMensaje, buff,sizeof(tamMensaje));
	tamMensaje = ntohl(tamMensaje);

	/*char infoSlots[tamMensaje+1];
	bzero(infoSlots,tamMensaje+1);*/
	char infoSlots[1024];
	bzero(infoSlots,1024);

	sktCliente.receive(infoSlots, tamMensaje);

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
	tamMensaje = htonl(tamMensaje);
	memcpy(buff,&tamMensaje ,sizeof(uint32_t));
	
	sktCliente.send(buff,sizeof(uint32_t));

	sktCliente.send(objeto.c_str(), objeto.length());

	recibirCodigoMensaje(1);

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

void ProxyServer::pedirSlotsDe(std::string idObjeto){
	char buff[5];
	bzero(buff,5);

    this->enviar(PEDIR_SLOTS,sizeof(char));

	uint32_t tamMensaje = idObjeto.length();
	tamMensaje = htonl(tamMensaje);
	memcpy(buff,&tamMensaje ,sizeof(uint32_t));
	
	sktCliente.send(buff,sizeof(uint32_t));

	sktCliente.send(idObjeto.c_str(), idObjeto.length());

	/*tamMensaje = 0;
	bzero(buff,5);
	sktCliente.receive(buff, sizeof(tamMensaje));
	memcpy(&tamMensaje, buff,sizeof(tamMensaje));
	tamMensaje = ntohl(tamMensaje);

	 hacer new
	char infoSlots[tamMensaje+1];
	bzero(infoSlots,tamMensaje+1);
	
	sktCliente.receive(infoSlots,tamMensaje);

	return std::string(infoSlots);*/

}


void ProxyServer::enviar(uint32_t entero, size_t cantidad){
	Proxy::enviarCantidad(sktCliente, entero, cantidad);
}

std::string ProxyServer::recibir(size_t cantidad){
	return Proxy::recibir(sktCliente,cantidad);
}

uint32_t ProxyServer::recibirCodigo(size_t cantidad){
	return Proxy::recibirCantidad(sktCliente,cantidad);
}

uint32_t ProxyServer::recibirTamanioDeMensaje(size_t cantidad){
	return Proxy::recibirCantidad(sktCliente,cantidad);
}

void ProxyServer::cerrarConexion(){
	sktCliente.shutdown();
}

ProxyServer::~ProxyServer(){
	sktCliente.shutdown();
}
