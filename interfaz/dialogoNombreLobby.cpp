#include "dialogoNombreLobby.h"

DialogoNombreLobby::DialogoNombreLobby(){}

DialogoNombreLobby::DialogoNombreLobby(BaseObjectType* cobject, 	
						const Glib::RefPtr<Gtk::Builder>& refGlade) : 
	Gtk::Dialog(cobject),
	m_builder(refGlade) { 
	Gtk::Button* buttonOk = nullptr; 
	m_builder->Gtk::Builder::get_widget("button10", buttonOk);
    buttonOk->signal_clicked().connect(sigc::mem_fun(*this,&DialogoNombreLobby::botonOkNombre));
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
	
	std::cout << "Respuesta de enviar nombreLobby: " 
			  << proxy->enviarString(nombreLobby) << std::endl;
	hide();
}

DialogoNombreLobby::~DialogoNombreLobby(){

}