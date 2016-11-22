#include "dialogoInicial.h"
#include "client_json_reader.h"
#include "dialogoNombreLobby.h"
#include "dialogoSeleccionLobby.h"
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
    //this->signal_delete_event().connect(sigc::mem_fun(*this,&DialogoInicial::cerrarDialogoClick));

    
    Gtk::Button* botonSalir = nullptr;
	m_builder-> Gtk::Builder::get_widget("button13", botonSalir);
  	if (botonSalir == nullptr) std::cout << "error glade" << std::endl;
    botonSalir->signal_clicked().connect(sigc::mem_fun(*this,&DialogoInicial::salirClick));
}

DialogoInicial::DialogoInicial(BaseObjectType* cobject, 
			const Glib::RefPtr<Gtk::Builder>& refGlade, bool is_glad):
			DialogoInicial(cobject, refGlade) {

}

void DialogoInicial::salirClick(){
    std::cout << "Click en salir dialogo" << std::endl;
	proxy->cerrarConexion();
	hide();
}

void DialogoInicial::nuevoLobbyClick(){
    std::cout << "Click en nuevo lobby" << std::endl;
    //Gtk::Dialog dialogoNombreLobby = nullptr;
	//m_builder->get_widget("dialog2", dialogoNombreLobby);
    proxy->enviar(CREAR_MAQUINA_VIRTUAL,sizeof(char));
	hide();
	// proxy->recibirCodigo(1);

	DialogoNombreLobby* dialogNombre = nullptr;
	m_builder->Gtk::Builder::get_widget_derived("dialog2", dialogNombre);
	dialogNombre->setProxy(proxy);
	dialogNombre->run();
}

void DialogoInicial::cargarLobbyClick(){
    std::cout << "Click en cargar lobby" << std::endl;
    proxy->enviar(CARGAR_MAQUINA_VIRTUAL,sizeof(char));
    std::string json = proxy->recibirJson();
    std::cout << json << std::endl;
	std::vector<string> names;
	JsonReader reader;
	reader.read_names(json, names);
	// ejemplo de json
	//"{"lobbies":[{"name":"lobby1"},{"name":"lobby2"}]}"
    //std::vector<string> names = {"lobby1","lobby2"};
	for(int i=0; i<names.size(); ++i){
		std::cout << names[i] << std::endl;
	}
	hide();

	DialogoSeleccionLobby* dSeleccionarLobby = nullptr;
	m_builder->Gtk::Builder::get_widget_derived("dialog3", dSeleccionarLobby);
	dSeleccionarLobby->setListaLobbys(names);
	dSeleccionarLobby->setProxy(proxy);
	dSeleccionarLobby->run();

	/*if(names.size()){
		proxy->enviar(7, 1);
		proxy->enviarString(names[0]);
		std::cout << "envie"<<std::endl;
	}*/
	//dSeleccionarLobby->run();
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