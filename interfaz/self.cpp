//#include "examplewindow.h"
#include <gtkmm.h>
#include "myarea.h"
#include "ventanaPrincipal.h"
// hacer en la clase windows.
static MyArea* myArea;
static Gtk::Dialog* dialog1;

bool onWindowDelete(GdkEventAny*){
	std::cout << "cerrar" << std::endl;
	if (myArea){
		myArea->liberarMemoria();
	}
	return false;
}

void on_button_clicked()
{
    std::cout << "Hello World" << std::endl;
    dialog1->hide();
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
	refBuilder-> Gtk::Builder::get_widget_derived("drawingarea1", myArea);
	window->signal_delete_event().connect(sigc::ptr_fun(onWindowDelete));
	
	refBuilder->Gtk::Builder::get_widget("dialog1", dialog1);

	Gtk::Button* botonNuevoLobby = nullptr;
	refBuilder-> Gtk::Builder::get_widget("button8", botonNuevoLobby);
  	if (botonNuevoLobby == nullptr) std::cout << "error" << std::endl;
    botonNuevoLobby->signal_clicked().connect(sigc::ptr_fun(&on_button_clicked));

	dialog1->run();

  	app->run(*window);
	
	return 0;
}