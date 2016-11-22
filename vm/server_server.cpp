#include "server_server.h"
#include "server_json_writer.h"
#include "../interfaz/client_json_reader.h"
#include <iostream>
#include <cstdlib>
#include <fstream>

static const int cod_error = 0;
static const int cod_create_app = 1;
static const int cod_get_apps_name = 6;
static const int cod_load_app = 7;
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
                try{ proxyClient.aceptarCliente(proxy);}
                catch (const std::exception e){break;}
		/* ver como hacer para que quede un solo try
        // Esto pasa solo cuando un usuario desea salir antes
        // en el ingreso del nombre o en la seleccion de lobby
        // o en caso de error tambien. si no la atrapo el server 
           recibe una exception y se cierra de golpe. */
        uint32_t codigoMensaje = 0;        
        try {
            codigoMensaje = proxy->recibirCodigoMensaje(msg_size);
        } catch (const std::exception e){continue;}

                if (codigoMensaje == cod_create_app){
                        
                        uint32_t tamMensaje = proxy->recibirTamMensaje(4);
                        string app_name = proxy->recibir(tamMensaje);
                        
                        if ( apps.find(app_name) == apps.end()){
                                proxy->enviar(cod_create_app, 1);
                                App* new_app = new App(app_name, this, proxy);
                                apps.insert (std::pair<string,App*>(app_name, new_app));
                                proxys.insert(std::pair<string,ProxyClient*>(app_name, proxy));
                                new_app -> start();
                        } else {
                                //Enviar Error, ese nombre ya existe
                                proxy->enviar(cod_error, 1);
                                // ahora se queda esperando que le envien un nombre
                                // valido.
                        }
                }else if (codigoMensaje == cod_get_apps_name){
                        string json = get_json_apps_name();
                        proxy->enviarJson(json);
                        codigoMensaje = proxy->recibirCodigoMensaje(msg_size);
                        if (codigoMensaje == cod_load_app){
                                uint32_t tamMensaje = proxy->recibirTamMensaje(4);
                                string app_name = proxy->recibir(tamMensaje);
                                App* new_app = new App(app_name, this, proxy);
                                apps.insert (std::pair<string,App*>(app_name, new_app));
                                proxys.insert(std::pair<string,ProxyClient*>(app_name, proxy));
                                execute_file(new_app, app_name);
                                new_app -> start();
                                proxy->enviar(cod_load_app, 1);
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

void Server::update_lobby_data(App* or_app, int cod, string json, int flag){
        for (map_proxys::iterator it = proxys.begin(); it != proxys.end(); ++it){
                if (or_app -> get_name() == it->first){                        
                        if (cod == 2 || cod == 8){
                                (it->second)->enviar(cod, 1);
                                (it->second) -> enviarJson(json);
                        }else if (cod == 5){// podria ser cualquier cosa, ejemplo : agregarSlot remove
                                if (flag == 0) {
                                    (it->second)->enviar(flag, 1);
                                } else if (flag == 3) {
                                    (it->second)->enviar(flag, 1);
                                } else if (flag == 4) {
                                    (it->second)->enviar(flag, 1);
                                } else {
                                //hardcodeo el 5 porque devuelve -1 por defecto 
                                //    para tener un caso no seteado
                                    (it->second)->enviar(5, 1);
                                }
                                std::cout << "devolucion: " << json << std::endl;
                    
                                if(flag != 0)
                                    (it->second)->enviarJson(json);
                        }                        
                }
        }
}

string Server::get_json_apps_name(){
        std::vector<string> names;
        for (map_apps::iterator it = apps.begin(), end = apps.end(); it != end; it = apps.upper_bound(it->first)){
                names.push_back(it->first);
        }
        JsonWriter writer;
        return writer.write_files_name(names);
}

int Server::execute(string file_name){
        App app;
        return execute_file(&app, file_name);
}

int Server::execute_file(App* app, string file_name){
	std::ifstream file;
	file.open(folder+file_name+file_ext, std::ifstream::in);
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
        this -> proxyClient.cerrarConexion();
        join_threads();
}

Server::~Server(){
         for (map_proxys::iterator it = proxys.begin(); it != proxys.end(); ++it){
                delete it->second;
        }    
}
