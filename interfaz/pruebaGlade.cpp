//#include "examplewindow.h"
#include <gtkmm.h>
#include "myarea.h"


int main(int argc, char *argv[])
{
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

  	auto refBuilder = Gtk::Builder::create();

  	refBuilder->add_from_file("pGlade.glade");

	Gtk::Window* window = nullptr;
	refBuilder->get_widget("window1", window);
	window->set_default_size(800,800);
	
	window->add_events( Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK );

	MyArea* myArea = nullptr;
	refBuilder-> Gtk::Builder::get_widget_derived("drawingarea1", myArea);

  	app->run(*window);
	
	return 0;
}