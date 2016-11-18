//#include "examplewindow.h"
#include <gtkmm.h>
#include "myarea.h"
#include "ventanaPrincipal.h"
#include "dialogoInicial.h"
#include "dialogoNombreLobby.h"

/*static Gtk::Entry* entryNombreLobby;
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

}*/

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

	/*if (argc != 3){
		printf("Error: La cantidad de parametros necesarios es 2.\n");
		example();
	}else{
		if (!strcmp(argv[1],"-p")){
                        server.bind(atoi(argv[2]));
                        Thread* thread_server = &server;
                        thread_server -> start();
                        read_exit();
                        server.shutdown();
                        thread_server -> join();
		}else if(!strcmp(argv[1],"-f")){
			string file = argv[2];
                        if (server.execute(file))
                                std::cout << "Error: " << file << " no existe." << std::endl;
		}else{
			std::cout << "Opcion Incorrecta " << argv[1] << std::endl;
			example();
		}
	}*/


	DialogoInicial* dialogoInicial = nullptr;
	refBuilder->Gtk::Builder::get_widget_derived("dialog1", dialogoInicial);
	dialogoInicial->setProxy(&proxy);
	dialogoInicial->run();

	DialogoNombreLobby* dialogNombre = nullptr;
	refBuilder->Gtk::Builder::get_widget_derived("dialog2", dialogNombre);
	dialogNombre->setProxy(&proxy);
	dialogNombre->run();

	MyArea* myArea = nullptr;
	refBuilder->Gtk::Builder::get_widget_derived("drawingarea1", myArea);
	myArea->setProxy(&proxy);

  	app->run(*window);
	
	return 0;
}