#include "common_socket.h"
#include "common_proxy.h"
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>

#define PEDIR_SLOTS 2

void Proxy::enviarCantidad(Socket& socket, uint32_t entero, size_t cantidad){
	char buff[5];
	bzero(buff,5);
	size_t enteroEnviar = entero;
	if (cantidad == 4){
		enteroEnviar = htonl(entero);
	}
	memcpy(buff,&enteroEnviar ,cantidad);
	socket.send(buff,cantidad);
}

void Proxy::enviar(Socket& socket, std::string mensaje, size_t cantidad){
	socket.send(mensaje.c_str(),cantidad);
}

std::string Proxy::recibir(Socket& socket,size_t cantidad){
	char buff[cantidad+1];
	bzero(buff,cantidad+1);
	socket.receive(buff,cantidad);
	return std::string(buff);
}

int Proxy::recibirCantidad(Socket& socket,size_t cantidad){
	char respuestaComando[5];
	if (socket.receive(respuestaComando,cantidad) < 0){
		//return -1;
		throw std::exception();
	}
	uint32_t ui=0;
	memcpy(&ui, respuestaComando,cantidad);
	if (cantidad == 1){
		return ui;
	}
	return ntohl(ui);
}

