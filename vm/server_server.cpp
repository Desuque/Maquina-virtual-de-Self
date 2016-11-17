#include "server_server.h"
#include <iostream>
#include <cstdlib>
#include <fstream>

static const int cod_create_vm = 1;
static const int cod_get_apps_name = 10;
static const int msg_size = 1;
static const char* file_ext = ".dat";
static const char* folder = "data/";

Server::Server(){
        load_file_names();
}

void Server::load_file_names(){
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (folder)) != NULL) {
            while ((ent = readdir (dir)) != NULL) {
                string file_name = ent->d_name;
                string name = file_name.substr(0, file_name.find(file_ext));
                if (name != file_name)
                        apps.insert (std::pair<string,App*>(name, NULL));
            }
            closedir (dir);
        }
}

void Server::bind(int port){
	this -> proxyClient.bindAndListen(port);
}

void Server::run(int* fin){
        ProxyClient* proxy = new ProxyClient();
	while (true){
                try{
                        proxyClient.aceptarCliente(proxy);
                }catch (const std::exception e){
			break;
		}
		
		uint32_t codigoMensaje = proxy->recibirCodigoMensaje(msg_size);
                //Tengo que recibir mensaje para saber si crear o agregar a uno ya existente
                if (codigoMensaje == cod_create_vm){
                        //Verificar si nombre nuevo existe
                        App* new_app = new App(proxy);
                        apps.insert (std::pair<string,App*>("vm", new_app));
                        new_app -> start();
                }//else if (codigoMensaje == cod_get_apps_name){
                //}
                join_threads();
        }
}

void Server::join_threads(){
        for(map_apps::iterator it = apps.begin(); it!=apps.end(); ) {
                App* app = it->second;
                if ( (app != NULL) && (app -> termino_thread()) ){
                        app -> save_vm(it->first);
                        app ->  join();
                        delete  app;
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
