//#include "examplewindow.h"
#include <gtkmm.h>
#include "myarea.h"
#include "ventanaPrincipal.h"
#include "dialogoInicial.h"

static Gtk::Entry* entryNombreLobby;
static MyArea* myArea;
static Gtk::Dialog* dialogNombre;

void on_click_button_ok(){

	std::cout << " ok " << std::endl;
	if(entryNombreLobby){
		std::cout << entryNombreLobby->get_text() << std::endl;
		myArea->enviarNombreLobby(entryNombreLobby->get_text());
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

	//MyArea* myArea = nullptr;
	refBuilder->Gtk::Builder::get_widget_derived("drawingarea1", myArea);

	DialogoInicial* dialogoInicial = nullptr;
	refBuilder->Gtk::Builder::get_widget_derived("dialog1", dialogoInicial);
	dialogoInicial->run();

	refBuilder->Gtk::Builder::get_widget("dialog2", dialogNombre);
	Gtk::Button* buttonOk = nullptr; 
	refBuilder->Gtk::Builder::get_widget("button10", buttonOk);
	refBuilder->Gtk::Builder::get_widget("entry1", entryNombreLobby);
	buttonOk->signal_clicked().connect(sigc::ptr_fun(&on_click_button_ok));
	dialogNombre->run();


  	app->run(*window);
	
	return 0;
}