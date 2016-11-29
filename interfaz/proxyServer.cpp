#include "proxyServer.h"
#include "common_socket.h"
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include "server_json_writer.h"

#define TAM_CODIGO_MENSAJE 1
#define TAM_INT_MENSAJE 4
/* codigos de comunicacion */
#define PEDIR_SLOTS 2
#define AGREGAR_SLOT 3
#define GENERIC 5
#define PEDIR_MORPH 11
#define BORRAR_MORPH 12


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

void ProxyServer::enviarCodigoMensaje(uint32_t codigoMensaje){
	char codigo = (char) codigoMensaje;
	sktCliente.send(&codigo, TAM_CODIGO_MENSAJE);
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


void ProxyServer::enviarCodigoAEjecutar(std::string idObjeto, std::string textoAEnviar, int accion){
    this->enviarCodigoMensaje(GENERIC);
	this->enviarCodigoMensaje(accion);
	this->enviarJson(idObjeto);
	this->enviarJson(textoAEnviar);
}

std::string ProxyServer::recibirJson(){
	char buff[5];
	bzero(buff,5);

	uint32_t tamMensaje = recibirTamanioDeMensaje();

	char* infoSlots = new char[tamMensaje+1];
	bzero(infoSlots,tamMensaje+1);

	sktCliente.receive(infoSlots,tamMensaje);

	std::string str_infoSlots = std::string(infoSlots);

	delete[] infoSlots;
	return str_infoSlots;
}

std::string ProxyServer::recibirSlotsDe(std::string objeto){
    this->enviarCodigoMensaje(PEDIR_SLOTS);
	this->enviarJson(objeto);
	// solo para completar el protocolo de conexion.
	this->recibirCodigoMensaje();
	std::string infoSlots = recibirJson();
	return infoSlots;
}

void ProxyServer::pedirMorphDeEsteSlot(int id_morph, std::string nombre_slot){
	
	JsonWriter writer;
	std::string json = writer.write_id_morph_nombre_slot(id_morph, nombre_slot);	

    this->enviarCodigoMensaje(PEDIR_MORPH);
    this->enviarJson(json);
}

void ProxyServer::pedirSlotsDe(std::string idObjeto){
    this->enviarCodigoMensaje(PEDIR_SLOTS);
	this->enviarJson(idObjeto);
}

uint32_t ProxyServer::recibirCodigoMensaje(){
	char respuestaComando='0';
	if (sktCliente.receive(&respuestaComando,TAM_CODIGO_MENSAJE) < 0){
		throw std::exception();
	}
	return (uint32_t)respuestaComando;
}


uint32_t ProxyServer::recibirTamanioDeMensaje(){
	char respuestaComando[TAM_INT_MENSAJE+1];
	bzero(respuestaComando,TAM_INT_MENSAJE+1);
	if (sktCliente.receive(respuestaComando,TAM_INT_MENSAJE) < 0){
		throw std::exception();
	}
	uint32_t ui=0;
	memcpy(&ui, respuestaComando,TAM_INT_MENSAJE);
	return ntohl(ui);
}

void ProxyServer::cerrarConexion(){
	sktCliente.shutdown();
}

ProxyServer::~ProxyServer(){
	sktCliente.shutdown();
}
