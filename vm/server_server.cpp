#include "server_server.h"
#include <iostream>
#include <cstdlib>
#include <fstream>

void Server::bind(int port){
	this -> proxyClient.bindAndListen(port);
}

void Server::run(int* fin){
        ProxyClient* proxy = new ProxyClient();
        bool start = 1;
	while (true){
                try{
                        proxyClient.aceptarCliente(proxy);
                }catch (const std::exception e){
			break;
		}
                //Tengo que recibir mensaje para saber si crear o agregar a uno ya existente
                if (start){
                        start = 0;
                        int id = 1;
                        App* new_app = new App(proxy);
                        apps.insert (std::pair<int,App*>(id, new_app));
                        new_app -> start();
                }
                join_threads();
        }
}

void Server::join_threads(){
        for(map_apps::iterator it = apps.begin(); it!=apps.end(); ) {
                if ((it->second)-> termino_thread()){
                        (it->second) -> save_vm();
                        (it -> second) ->  join();
                        delete  it->second;
                        it = apps.erase(it);
                }else{
                        it++;
                }
        }
}

int Server::execute_file(string file_name){
	std::ifstream file;
	file.open(file_name, std::ifstream::in);
	if (!file.is_open())
		return 1;
	
	std::string str;
	std::string file_contents;
	while (std::getline(file, str)){
		file_contents += str;
		file_contents.push_back('\n');
	}
	App app;
        app.execute_file(file_contents);
        return 0;
}

void Server::shutdown(){
        this -> proxyClient.cerrarConexion();
        join_threads();
}
