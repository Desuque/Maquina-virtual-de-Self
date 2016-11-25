#include "common_socket.h"
#include "common_proxyClient.h"
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>

#define TAM_INT_MENSAJE 4
#define TAM_CODIGO_MENSAJE 1

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

void ProxyClient::enviarJson(std::string json){
	uint32_t tamMensaje = json.length();
	char buff[TAM_INT_MENSAJE+1];
	bzero(buff,TAM_INT_MENSAJE+1);

	tamMensaje = htonl(tamMensaje);
	memcpy(buff,&tamMensaje ,sizeof(uint32_t));
	
	server.send(buff,sizeof(uint32_t));
	server.send(json.c_str(), json.length());
}

std::string ProxyClient::recibirJson(){
	uint32_t tamMensaje = this->recibirTamMensaje();

	char* infoSlots = new char[tamMensaje+1];
	bzero(infoSlots,tamMensaje+1);

	server.receive(infoSlots,tamMensaje);

	std::string str_infoSlots = std::string(infoSlots);

	delete[] infoSlots;
	return str_infoSlots;
}

uint32_t ProxyClient::recibirCodigoMensaje(){
	char respuestaComando='0';
	if (server.receive(&respuestaComando,TAM_CODIGO_MENSAJE) < 0){
		throw std::exception();
	}
	return (uint32_t)respuestaComando;
}

void ProxyClient::enviarCodigoMensaje(uint32_t codigoMensaje){
	char codigo = (char) codigoMensaje;
	server.send(&codigo, TAM_CODIGO_MENSAJE);
}

uint32_t ProxyClient::recibirTamMensaje(){
	char respuestaComando[TAM_INT_MENSAJE+1];
	bzero(respuestaComando,TAM_INT_MENSAJE+1);
	if (server.receive(respuestaComando,TAM_INT_MENSAJE) < 0){
		throw std::exception();
	}
	uint32_t ui=0;
	memcpy(&ui, respuestaComando,TAM_INT_MENSAJE);
	return ntohl(ui);
}

ProxyClient::~ProxyClient(){
	server.shutdown();
}
