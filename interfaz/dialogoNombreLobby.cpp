#include "dialogoNombreLobby.h"
#include <gtkmm/entry.h>
DialogoNombreLobby::DialogoNombreLobby(){}

DialogoNombreLobby::DialogoNombreLobby(BaseObjectType* cobject, 	
						const Glib::RefPtr<Gtk::Builder>& refGlade) : 
	Gtk::Dialog(cobject),
	m_builder(refGlade) { 
	Gtk::Button* buttonOk = nullptr; 
	m_builder->Gtk::Builder::get_widget("button10", buttonOk);
    buttonOk->signal_clicked().connect(sigc::mem_fun(*this,&DialogoNombreLobby::botonOkNombre));

    Gtk::Button* buttonSalir = nullptr; 
	m_builder->Gtk::Builder::get_widget("button11", buttonSalir);
    buttonSalir->signal_clicked().connect(sigc::mem_fun(*this,&DialogoNombreLobby::botonSalir));

}

void DialogoNombreLobby::botonOkNombre(){
	if(!proxy){
		std::cout << "Error no hay proxy en DialogoNombreLobby" << std::endl;
		return;
	}	
	Gtk::Entry* entryNombreLobby = nullptr;
	m_builder->Gtk::Builder::get_widget("entry1", entryNombreLobby);
	if(!entryNombreLobby){
		std::cout << "Error m_builder DialogoNombreLobby::botonOkNombre" << std::endl;
		throw new std::exception();
	}
	std::string nombreLobby = std::string(entryNombreLobby->get_text());
	std::cout << nombreLobby << std::endl;
	
	proxy->enviarJson(nombreLobby);
	uint32_t respuesta = proxy->recibirCodigoMensaje();

	if (respuesta == 0){
		// el lobby ya existe lanzar un error y volver a pedir 
		// el nombre.
		//consultar
		//entryNombreLobby->override_background_color(Gdk::RGBA(0.0, 1.0, 0.0, 1.0), Gtk::STATE_FLAG_NORMAL);
		entryNombreLobby->set_text("");
		//std::cout << "ERROR: Nombre de lobby existente, por favor ingrese otro\n";
		Gtk::MessageDialog dialog(*this, "ERROR: Nombre de lobby existente, por favor ingrese otro");
  		dialog.run();
		//proxy->cerrarConexion();
		return;
	}
	hide();
}

void DialogoNombreLobby::botonSalir(){
	if(proxy){
		proxy->cerrarConexion();
	}
	hide();
}

DialogoNombreLobby::~DialogoNombreLobby(){

}