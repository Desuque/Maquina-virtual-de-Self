//#include "examplewindow.h"
#include <gtkmm.h>
#include "myarea.h"
#include "ventanaPrincipal.h"
#include "dialogoInicial.h"
#include "dialogoNombreLobby.h"
#include "cliente.h"
void example();

int main(int argc, char *argv[])
{
	ProxyServer proxy;
	try{
		if (argc != 5){
			printf("Error: La cantidad de parametros necesarios es 4.\n");
			example();
			return 0;
		}else{
			if (!strcmp(argv[1],"-ip")){
				if (!strcmp(argv[3],"-p")){
					proxy.connect(argv[2], atoi(argv[4]));
				}                        
			}else{
				std::cout << "Opcion Incorrecta " << argv[1] << std::endl;
				example();
				return 0;
			}
		}
	
	auto app = Gtk::Application::create();

  	auto refBuilder = Gtk::Builder::create();

  	refBuilder->add_from_file("pGlade.glade");

	VentanaPrincipal* window = nullptr;
	refBuilder->get_widget_derived("window1", window);
	window->setProxy(&proxy);
	window->iniciarDialogos();

	MyArea* myArea = nullptr;
	refBuilder->Gtk::Builder::get_widget_derived("drawingarea1", myArea);
	myArea->setProxy(&proxy);
	myArea->iniciar();
	
	Cliente cliente(proxy, myArea);
	cliente.start();

  	app->run(*window);

  	proxy.cerrarConexion();
  	cliente.join();
	
	return 0;

	} catch (const std::exception &e){
		std::cout << "Error de conexion con el server" << std::endl;
		return 0;
	}
}

void example(){
	printf("Ejemplos: \n");;
	printf("\t ./client_self -ip <direccion ip> -p <port>\n");
	printf("\t ./client_self -ip 127.0.0.1 -p 8080 \n");
}