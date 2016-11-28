#include "dialogoSeleccionLobby.h"
#include "listaDeLobbys.h"

DialogoSeleccionLobby::DialogoSeleccionLobby(){}

DialogoSeleccionLobby::DialogoSeleccionLobby(BaseObjectType* cobject, 	
						const Glib::RefPtr<Gtk::Builder>& refGlade) : 
	Gtk::Dialog(cobject),
	m_builder(refGlade) {}

void DialogoSeleccionLobby::setListaLobbys(std::vector<std::string> listaLobbys){
	ListaDeLobbys* listaDeLobbys = nullptr; 
	m_builder->Gtk::Builder::get_widget_derived("treeview2", listaDeLobbys);
	listaDeLobbys->setLista(listaLobbys);
}

DialogoSeleccionLobby::~DialogoSeleccionLobby(){

}