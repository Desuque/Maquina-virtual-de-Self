#include "server_virtual_machine.h"
#include "server_slot.h"
#include "../interfaz/client_interface_slot.h"
#include "../interfaz/client_json_reader.h"
#include "server_server.h"
#include "common_proxyClient.h"
#include <iostream>
#include <string.h>
#include "common_thread.h"
#include <thread>

void example();
void read_exit();

static const char char_final = 'q';

int main(int argc, char** argv){
	Server server;	
	if (argc != 3){
		printf("Error: La cantidad de parametros necesarios es 2.\n");
		example();
	}else{
		if (!strcmp(argv[1],"-p")){
                        server.bind(atoi(argv[2]));
                        Thread* thread_server = &server;
                        thread_server -> start();
                        read_exit();
                        server.shutdown();
                        thread_server -> join();
		}else if(!strcmp(argv[1],"-f")){
			string file = argv[2];
                        if (server.execute(file))
                                std::cout << "Error: " << file << " no existe." << std::endl;
		}else{
			std::cout << "Opcion Incorrecta " << argv[1] << std::endl;
			example();
		}
	}
	
	return 0;
}

void example(){
	printf("Ejemplos: \n");;
	printf("\t ./server -p <port>\n");
	printf("\t ./server -f <file>\n");
}

void read_exit(){
        char c = 0;
        while ( c != char_final ){
            std::cin.get(c);
        }
}
