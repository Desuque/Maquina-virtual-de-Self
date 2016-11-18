#include "dialogoInicial.h"
#include "client_json_reader.h"

#include <iostream>
#include <vector>

#define CREAR_MAQUINA_VIRTUAL 0x01
#define CARGAR_MAQUINA_VIRTUAL 0x06

DialogoInicial::DialogoInicial(){

}

DialogoInicial::DialogoInicial(BaseObjectType* cobject, 	
					const Glib::RefPtr<Gtk::Builder>& refGlade) : 
	Gtk::Dialog(cobject),
	m_builder(refGlade), 
	proxy(nullptr) {
	Gtk::Button* botonNuevoLobby = nullptr;
	m_builder-> Gtk::Builder::get_widget("button8", botonNuevoLobby);
  	if (botonNuevoLobby == nullptr) std::cout << "error" << std::endl;
    botonNuevoLobby->signal_clicked().connect(sigc::mem_fun(*this,&DialogoInicial::nuevoLobbyClick));

	Gtk::Button* botonCargarLobby = nullptr;
	m_builder-> Gtk::Builder::get_widget("button9", botonCargarLobby);
  	if (botonCargarLobby == nullptr) std::cout << "error" << std::endl;
    botonCargarLobby->signal_clicked().connect(sigc::mem_fun(*this,&DialogoInicial::cargarLobbyClick));
    this->signal_delete_event().connect(sigc::mem_fun(*this,&DialogoInicial::cerrarDialogoClick));
}

DialogoInicial::DialogoInicial(BaseObjectType* cobject, 
			const Glib::RefPtr<Gtk::Builder>& refGlade, bool is_glad):
			DialogoInicial(cobject, refGlade) {

}

void DialogoInicial::nuevoLobbyClick(){
    std::cout << "Click en nuevo lobby" << std::endl;
    //Gtk::Dialog dialogoNombreLobby = nullptr;
	//m_builder->get_widget("dialog2", dialogoNombreLobby);
    proxy->enviar(CREAR_MAQUINA_VIRTUAL,sizeof(char));
	hide();
}

void DialogoInicial::cargarLobbyClick(){
    std::cout << "Click en cargar lobby" << std::endl;
    proxy->enviar(CARGAR_MAQUINA_VIRTUAL,sizeof(char));
    std::string json = proxy->recibirJson();
	std::vector<string> names;
	JsonReader reader;
	reader.read_names(json, names);
	
	for(int i=0; i<names.size(); ++i){
		std::cout << names[i] << std::endl;
	}

	hide();
}

bool DialogoInicial::cerrarDialogoClick(GdkEventAny*){
    std::cout << "Click en cerrar dialogo" << std::endl;
	hide();
	return false;
}

void DialogoInicial::setProxy(ProxyServer* proxy){
	this->proxy = proxy;
}

DialogoInicial::~DialogoInicial(){

}