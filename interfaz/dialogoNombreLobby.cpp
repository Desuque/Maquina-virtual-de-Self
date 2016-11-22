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

DialogoNombreLobby::DialogoNombreLobby(BaseObjectType* cobject, 
			const Glib::RefPtr<Gtk::Builder>& refGlade,
			bool is_glad) : DialogoNombreLobby(cobject, refGlade) {

}

void DialogoNombreLobby::botonOkNombre(){
	if(!proxy) std::cout << "Error no hay proxy en DialogoNombreLobby" << std::endl;
	Gtk::Entry* entryNombreLobby = nullptr;
	m_builder->Gtk::Builder::get_widget("entry1", entryNombreLobby);
	if(!entryNombreLobby) std::cout << "Error m_builder DialogoNombreLobby::botonOkNombre" << std::endl;
	std::string nombreLobby = std::string(entryNombreLobby->get_text());
	std::cout << nombreLobby << std::endl;
	
	uint32_t respuesta = proxy->enviarString(nombreLobby);
	std::cout << "Respuesta de enviar nombreLobby: " 
			  << respuesta << std::endl;
	if (respuesta == 0){
		// el lobby ya existe lanzar un error y volver a pedir 
		// el nombre.
		//consultar
		//entryNombreLobby->override_background_color(Gdk::RGBA(0.0, 1.0, 0.0, 1.0), Gtk::STATE_FLAG_NORMAL);
		entryNombreLobby->set_text("NOMBRE EXISTENTE REINICIE LA APP");
		std::cout << "ERROR: Nombre de lobby existente, por favor reinicie la aplicacion\n";
		proxy->cerrarConexion();
		return;
	}
	hide();
}

void DialogoNombreLobby::botonSalir(){
	proxy->cerrarConexion();
	hide();
}

DialogoNombreLobby::~DialogoNombreLobby(){

}