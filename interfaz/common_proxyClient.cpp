#include "common_socket.h"
#include "common_proxyClient.h"
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>


ProxyClient::ProxyClient(const unsigned int puerto){
	server.bindAndListen(puerto);
}

void ProxyClient::bindAndListen(const unsigned int puerto){
	server.bindAndListen(puerto);
}

ProxyClient::ProxyClient(Socket& socket):server(std::move(socket)){}

ProxyClient::ProxyClient(ProxyClient&& other):server(std::move(other.server)){}

ProxyClient& ProxyClient::operator=(ProxyClient&& other) {
	this->server = std::move(other.server);
	return *this;
}

void ProxyClient::enviarSlots(std::string slots){
	uint32_t tamMensaje = slots.length();
	char buff[5];
	bzero(buff,5);
	tamMensaje = htonl(tamMensaje);
	memcpy(buff,&tamMensaje ,sizeof(uint32_t));
	
	server.send(buff,sizeof(uint32_t));
	server.send(slots.c_str(), slots.length());
}


void ProxyClient::aceptarCliente(ProxyClient* proxy){
	Socket socket = server.accept();
        proxy -> set_socket(socket);
}

void ProxyClient::set_socket(Socket& socket){
        this -> server = std::move(socket);
}

void ProxyClient::cerrarConexion(){
	server.shutdown();
}

/*void ProxyClient::enviar(uint32_t entero, size_t cantidad){
	Proxy::enviarCantidad(server, entero, cantidad);
}*/

/*void ProxyClient::enviar(std::string mensaje, size_t cantidad){
	Proxy::enviar(server, mensaje, cantidad);
}*/

void ProxyClient::enviar(uint32_t entero, size_t cantidad){
	Proxy::enviarCantidad(server, entero, cantidad);
}

std::string ProxyClient::recibir(size_t cantidad){
	return Proxy::recibir(server,cantidad);
}

uint32_t ProxyClient::recibirCodigoMensaje(size_t cantidad){
	return Proxy::recibirCantidad(server,cantidad);
}

uint32_t ProxyClient::recibirTamMensaje(size_t cantidad){
	return Proxy::recibirCantidad(server,cantidad);
}

ProxyClient::~ProxyClient(){
	server.shutdown();
}
