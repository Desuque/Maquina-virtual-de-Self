//#include "examplewindow.h"
#include <gtkmm.h>
#include "myarea.h"
#include "ventanaPrincipal.h"
#include "dialogoInicial.h"
#include "manejadorVista.h"
#include "dialogoNombreLobby.h"

static Gtk::Entry* entryNombreLobby;
static MyArea* myArea;
static DialogoNombreLobby* dialogNombre;

void on_click_button_ok(){

	std::cout << " ok " << std::endl;
	if(entryNombreLobby){
		std::string nombreLobby = std::string(entryNombreLobby->get_text());
		std::cout << nombreLobby << std::endl;
		//myArea->enviarNombreLobby(nombreLobby);
	}
	if(dialogNombre){
		dialogNombre->hide();
	}

}

int main(int argc, char *argv[])
{
	auto app = Gtk::Application::create();

  	auto refBuilder = Gtk::Builder::create();

  	refBuilder->add_from_file("pGlade.glade");

	VentanaPrincipal* window = nullptr;
	refBuilder->get_widget_derived("window1", window);
	window->set_default_size(800,800);
	
	window->add_events( Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK );

	ProxyServer proxy;
	proxy.connect("127.0.0.1",8080);

	DialogoInicial* dialogoInicial = nullptr;
	refBuilder->Gtk::Builder::get_widget_derived("dialog1", dialogoInicial);
	dialogoInicial->setProxy(&proxy);
	dialogoInicial->run();

	refBuilder->Gtk::Builder::get_widget_derived("dialog2", dialogNombre);
	dialogNombre->setProxy(&proxy);
	dialogNombre->run();

	//MyArea* myArea = nullptr;
	refBuilder->Gtk::Builder::get_widget_derived("drawingarea1", myArea);
	myArea->setProxy(&proxy);

  	app->run(*window);
	
	return 0;
}