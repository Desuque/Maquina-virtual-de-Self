#include "dialogoInicial.h"
#include "client_json_reader.h"
#include "dialogoNombreLobby.h"
#include "dialogoSeleccionLobby.h"
#include <iostream>
#include <vector>
#include "listaDeLobbys.h"
#define CREAR_MAQUINA_VIRTUAL 0x01
#define CARGAR_MAQUINA_VIRTUAL 0x06
#define COD_LOAD_APP 0x07

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
  	if (botonCargarLobby == nullptr){
  		std::cout << "error glade" << std::endl;
    	throw new std::exception();
    }
    botonCargarLobby->signal_clicked().connect(sigc::mem_fun(*this,&DialogoInicial::cargarLobbyClick));
    //this->signal_delete_event().connect(sigc::mem_fun(*this,&DialogoInicial::cerrarDialogoClick));
 
    Gtk::Button* botonSalir = nullptr;
	m_builder-> Gtk::Builder::get_widget("button13", botonSalir);
  	if (botonSalir == nullptr){
  		std::cout << "error glade" << std::endl;
    	throw new std::exception();
    }
    botonSalir->signal_clicked().connect(sigc::mem_fun(*this,&DialogoInicial::salirClick));
}

void DialogoInicial::salirClick(){
	if(proxy){
		proxy->cerrarConexion();
	}
	hide();
}

void DialogoInicial::nuevoLobbyClick(){
    //Gtk::Dialog dialogoNombreLobby = nullptr;
	//m_builder->get_widget("dialog2", dialogoNombreLobby);
    proxy->enviarCodigoMensaje(CREAR_MAQUINA_VIRTUAL);
	hide();

	DialogoNombreLobby* dialogNombre = nullptr;
	m_builder->Gtk::Builder::get_widget_derived("dialog2", dialogNombre);
	dialogNombre->setProxy(proxy);
	dialogNombre->run();
}

void DialogoInicial::cargarLobbyClick(){
    proxy->enviarCodigoMensaje(CARGAR_MAQUINA_VIRTUAL);
    std::string json = proxy->recibirJson();
    std::cout << json << std::endl;
	std::vector<string> names;
	JsonReader reader;
	reader.read_names(json, names);

	hide();

	Gtk::Button* buttonOk = nullptr; 
	m_builder->Gtk::Builder::get_widget("button12", buttonOk);
    sigcBotonOk = buttonOk->signal_clicked().connect(sigc::mem_fun(*this,&DialogoInicial::botonOkLobby));

	DialogoSeleccionLobby* dSeleccionarLobby = nullptr;
	m_builder->Gtk::Builder::get_widget_derived("dialog3", dSeleccionarLobby);
	dSeleccionarLobby->setListaLobbys(names);
	//dSeleccionarLobby->setProxy(proxy);
	dSeleccionarLobby->run();
}

void DialogoInicial::botonOkLobby(){
	sigcBotonOk.disconnect();
	ListaDeLobbys* listaDeLobbys = nullptr; 
	m_builder->Gtk::Builder::get_widget_derived("treeview2", listaDeLobbys);
	std::string seleccion = listaDeLobbys->obtenerLobbySeleccionado();

	proxy -> enviarCodigoMensaje(COD_LOAD_APP);
	proxy->enviarJson(seleccion);
	
	if(COD_LOAD_APP != proxy->recibirCodigoMensaje()){
		std::cout << "Error al cargar lobby" << std::endl;
		throw new std::exception();
	}
	
	DialogoSeleccionLobby* dSeleccionarLobby = nullptr;
	m_builder->Gtk::Builder::get_widget_derived("dialog3", dSeleccionarLobby);
	dSeleccionarLobby->hide();
}

bool DialogoInicial::cerrarDialogoClick(GdkEventAny*){
	hide();
	return false;
}

void DialogoInicial::setProxy(ProxyServer* proxy){
	this->proxy = proxy;
}

DialogoInicial::~DialogoInicial(){

}