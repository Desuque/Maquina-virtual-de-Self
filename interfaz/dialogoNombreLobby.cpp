#include "dialogoNombreLobby.h"
#include "ventanaPrincipal.h"
#include <gtkmm/entry.h>
#include <sstream>

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
		std::cout << "Error no hay proxy" << std::endl;
		throw new std::exception();
		return;
	}	
	Gtk::Entry* entryNombreLobby = nullptr;
	m_builder->Gtk::Builder::get_widget("entry1", entryNombreLobby);
	if(!entryNombreLobby){
		std::cout << "Error m_builder DialogoNombreLobby::botonOkNombre" << std::endl;
		throw new std::exception();
	}
	std::string nombreLobby = std::string(entryNombreLobby->get_text());
	
	std::string nombreSinEspacios;
	std::istringstream iss(nombreLobby);
	iss >> std::skipws;
	iss >> std::ws >> nombreSinEspacios;

	if (nombreSinEspacios==""){
		Gtk::MessageDialog dialog(*this, "ERROR: Ingrese algun nombre");
  		dialog.run();
  		return;
	}

	proxy->enviarJson(nombreSinEspacios);
	uint32_t respuesta = proxy->recibirCodigoMensaje();

	if (respuesta == 0){
		// el lobby ya existe lanzar un error y volver a pedir 
		// el nombre.
		Gtk::MessageDialog dialog(*this, "ERROR: Nombre de lobby existente, por favor ingrese otro");
		std::string nombreRepetido = entryNombreLobby->get_text();
		// agregue un numero para que su nombre funcione
		// Ejemplo: nombre de lobby: lobbyExample, 
		// quieren crear un nuevo ERROR: Nombre de lobby existente, por favor ingrese otro, 
		// Error pruebe con lobbyExample1.
		nombreRepetido += "1";
		entryNombreLobby->set_text(nombreRepetido);
		dialog.set_secondary_text("	   Intente con este '" + nombreRepetido + "' .");

  		dialog.run();
		return;
	}

	VentanaPrincipal* ventanaPrincipal = nullptr;
	m_builder->Gtk::Builder::get_widget_derived("window1", ventanaPrincipal);
	if(ventanaPrincipal==nullptr){
		std::cout << "Error Glade" << std::endl;
		throw new std::exception();
	}
	ventanaPrincipal->setLabel(nombreSinEspacios);
	hide();
}

void DialogoNombreLobby::botonSalir(){
	if(proxy){
		proxy->cerrarConexion();
	}
	hide();
}

DialogoNombreLobby::~DialogoNombreLobby(){}