#include "server_server.h"
#include "server_json_writer.h"
#include "../interfaz/client_json_reader.h"
#include <iostream>
#include <cstdlib>
#include <fstream>

static const int cod_error = 0;
static const int cod_create_app = 1;
static const int cod_get_slots = 2;
static const int cod_gen = 5;
static const int cod_get_apps_name = 6;
static const int cod_load_app = 7;
static const int cod_pos = 8;
static const int cod_morph = 11;
static const int cod_del = 12;
static const int msg_size = 1;
static const int num_op = 3;
static const int flag_error = 0;
static const int flag_add = 3;
static const int flag_rm = 4;
static const int flag_gar = 14;
static string user = getenv("USER"); 
static string dir_file = "/home/"+user+"/Documents/data/";
static const char* file_ext = ".dat";

Server::Server(){
        load_file_names();
}

void Server::load_file_names(){
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (dir_file.c_str())) != NULL) {
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
        
                uint32_t codigoMensaje = 0;        
                try {
                    codigoMensaje = proxy->recibirCodigoMensaje();
                } catch (const std::exception e){continue;}

                if (codigoMensaje == cod_create_app){
                        recv_app_create(proxy);
                }else if (codigoMensaje == cod_get_apps_name){
                        recv_app_load(proxy);
                }
                join_threads();
        }
        delete proxy;
}

void Server::recv_app_create(ProxyClient* proxy){
        try {
        // le doy tres intentos al usuario para que 
        // ingrese el nombre de un lobby distinto.
            for(int i = 0; i < num_op ; ++i){
                string app_name = proxy->recibirJson();
                            
                if ( apps.find(app_name) == apps.end()){
                        proxy->enviarCodigoMensaje(cod_create_app);
                        App* new_app = new App(app_name, this, proxy);
                        apps.insert (std::pair<string,App*>(app_name, new_app));
                        proxys.insert(std::pair<string,ProxyClient*>(app_name, proxy));
                        new_app -> start();
                        break;
                } else {
                    proxy->enviarCodigoMensaje(cod_error);
                }
            }
        } catch (const std::exception e){
            // El cliente se desconecto sin 
            // ingresar el noombre del lobby.
            return;
        }
}

void Server::recv_app_load(ProxyClient* proxy){
        string json = get_json_apps_name();
        proxy->enviarJson(json);
        uint32_t codigoMensaje = proxy->recibirCodigoMensaje();
        if (codigoMensaje == cod_load_app){
                string app_name = proxy->recibirJson();
                App* new_app = new App(app_name, this, proxy);
                apps.insert (std::pair<string,App*>(app_name, new_app));
                proxys.insert(std::pair<string,ProxyClient*>(app_name, proxy));
                string file_name = dir_file+app_name+file_ext;
                execute_file(new_app, file_name);
                new_app -> start();
                proxy->enviarCodigoMensaje(cod_load_app);
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

void Server::update_lobby_data(App* or_app, int cod, string json, int flag, int action){
        for (map_proxys::iterator it = proxys.begin(); it != proxys.end(); ++it){
                if (or_app -> get_name() == it->first){                        
                        if (cod == cod_get_slots || cod == cod_pos || cod == cod_morph || cod == cod_del){
                                (it->second)->enviarCodigoMensaje(cod);
                                (it->second) -> enviarJson(json);
                        }else if (cod == cod_gen){
                                if ((flag == flag_error) || (flag == flag_add) || (flag == flag_rm) || (flag == flag_gar)) {
                                    (it->second)->enviarCodigoMensaje(flag);
                                } else {
                                    (it->second)->enviarCodigoMensaje(cod_gen);
                                    (it->second)->enviarCodigoMensaje(action);
                                }
                    
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

void Server::update_app(string str_parser_code_share, string lobby_des){
         for (map_apps::iterator it = apps.begin(); it != apps.end(); ++it){
                if (lobby_des == it->first)
                            (it->second) -> update_app(str_parser_code_share);
        }
}

void Server::update_clients(string json_share, string lobby_des){
         for (map_proxys::iterator it = proxys.begin(); it != proxys.end(); ++it){
                if (lobby_des == it->first){                   
                            (it->second) -> enviarCodigoMensaje(cod_get_slots);
                            (it->second) -> enviarJson(json_share);
                }
        }
}

void Server::share_obj_to(string str_parser_code_share, string json_share, string lobby_des){
        update_app(str_parser_code_share, lobby_des);
        update_clients(json_share, lobby_des);
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
    
        string str;
        string file_contents;
        while (std::getline(file, str)){
            file_contents += str;
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