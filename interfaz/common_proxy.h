#include "common_socket.h"
#ifndef PROXY_H_
#define PROXY_H_

class Proxy {
	public:
		Proxy(){}
		std::string recibir(Socket& socket, size_t cantidad);
		int recibirCantidad(Socket& socket, size_t cantidad);
		void enviarCantidad(Socket& socket,uint32_t entero, size_t cantidad);
		void enviar(Socket& socket,std::string mensaje, size_t cantidad);
		
		virtual ~Proxy(){}
};

#endif
