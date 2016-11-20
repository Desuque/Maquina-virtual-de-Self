#include "dialogoSeleccionLobby.h"
#include "listaDeLobbys.h"

DialogoSeleccionLobby::DialogoSeleccionLobby(){}

DialogoSeleccionLobby::DialogoSeleccionLobby(BaseObjectType* cobject, 	
						const Glib::RefPtr<Gtk::Builder>& refGlade) : 
	Gtk::Dialog(cobject),
	m_builder(refGlade) { 
	Gtk::Button* buttonOk = nullptr; 
	m_builder->Gtk::Builder::get_widget("button12", buttonOk);
    buttonOk->signal_clicked().connect(sigc::mem_fun(*this,&DialogoSeleccionLobby::botonOkLobby));

   
    /*std::vector<std::string> names = {"lobby1", "lobby2"};
    ListaDeLobbys listaDeLobbys(names);*/
}

DialogoSeleccionLobby::DialogoSeleccionLobby(BaseObjectType* cobject, 
			const Glib::RefPtr<Gtk::Builder>& refGlade,
			bool is_glad) : DialogoSeleccionLobby(cobject, refGlade) {
}

void DialogoSeleccionLobby::botonOkLobby(){
	std::cout << "Ok" << std::endl;
	ListaDeLobbys* listaDeLobbys = nullptr; 
	m_builder->Gtk::Builder::get_widget_derived("treeview2", listaDeLobbys);
	std::string seleccion = listaDeLobbys->obtenerLobbySeleccionado();
	std::cout << seleccion << std::endl;
	proxy -> enviar(7,1);
	proxy->enviarString(seleccion);
	hide();
}

void DialogoSeleccionLobby::setListaLobbys(std::vector<std::string> listaLobbys){
	ListaDeLobbys* listaDeLobbys = nullptr; 
	m_builder->Gtk::Builder::get_widget_derived("treeview2", listaDeLobbys);
	listaDeLobbys->setLista(listaLobbys);
}


DialogoSeleccionLobby::~DialogoSeleccionLobby(){

}