#include "server_server.h"
#include "server_json_writer.h"
#include "../interfaz/client_json_reader.h"
#include <iostream>
#include <cstdlib>
#include <fstream>

static const int cod_create_app = 1;
static const int cod_get_apps_name = 999;
static const int cod_load_app = 1000;
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
        ProxyClient* proxy;
	while (true){
                proxy = new ProxyClient();
                try{
                        proxyClient.aceptarCliente(proxy);
                }catch (const std::exception e){
			break;
		}
		
		uint32_t codigoMensaje = proxy->recibirCodigoMensaje(msg_size);
                //Tengo que recibir mensaje para saber si crear o agregar a uno ya existente
                if (codigoMensaje == cod_create_app){
                        //uint32_t tamMensaje = proxy->recibirTamMensaje(4);
                        //string app_name = proxy->recibir(tamMensaje);
                        //Verificar si nombre nuevo existe
                        /*if ( apps.find(app_name) == apps.end() ) {
                                // not found
                                App* new_app = new App(proxy);
                                apps.insert (std::pair<string,App*>("vm", new_app));
                                new_app -> start();
                        } else {
                                //Enviar Error, ese nombre ya existe
                        }*/
                        App* new_app = new App(proxy);
                        int v = rand() % 100000; 
                        apps.insert (std::pair<string,App*>(std::to_string(v), new_app));
                        //apps.at("vm") = new_app;
                        new_app -> start();
                }else if (codigoMensaje == cod_get_apps_name){
                        string json = get_json_apps_name();
                        proxy->enviarJson(json);
                        //VA EN EL CLIENTE PARA DECODIFICAR LOS NOMBRES 
                        //std::vector<string> names;
                        //JsonReader reader;
                        //reader.read_names(json, names);
                }else if (codigoMensaje == cod_load_app){
                        uint32_t tamMensaje = proxy->recibirTamMensaje(4);
                        string app_name = proxy->recibir(tamMensaje);
                        App* app_load = apps.at(app_name);
                        if (app_load){
                                std::cout << "Agregar al existente " << std::endl;
                                //app_load -> add_proxy(proxy);
                        }else{
                                app_load = new App(proxy);
                                execute_file(app_load, app_name);
                                app_load -> start();
                        }
                }
                join_threads();
        }
        delete proxy;
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

string Server::get_json_apps_name(){
        std::vector<string> names;
        for(map_apps::iterator it = apps.begin(); it != apps.end(); ++it)
                names.push_back(it->first);
        
        JsonWriter writer;
        return writer.write_files_name(names);
}

int Server::execute(string file_name){
        App app;
        return execute_file(&app, file_name);
}

int Server::execute_file(App* app, string file_name){
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
        app->execute_file(file_contents);
        return 0;
}

void Server::shutdown(){
        join_threads();
        this -> proxyClient.cerrarConexion();
}
