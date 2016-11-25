#include "ventanaPrincipal.h"
#include <iostream>
#include "myarea.h"
#include "dialogoInicial.h"
bool VentanaPrincipal::onWindowDelete(GdkEventAny*){
  std::cout << "cerre desde ventanaPrincipal" << std::endl;
  MyArea* myArea = nullptr;
  m_builder-> Gtk::Builder::get_widget_derived("drawingarea1", myArea);
  myArea->liberarMemoria();
  return false;
}

VentanaPrincipal::VentanaPrincipal(){

}

VentanaPrincipal::~VentanaPrincipal(){

}
void VentanaPrincipal::setProxy(ProxyServer* proxy){
  this->proxy = proxy;
}

void VentanaPrincipal::iniciarDialogos(){
  DialogoInicial* dialogoInicial = nullptr;
  m_builder->Gtk::Builder::get_widget_derived("dialog1", dialogoInicial);
  if(proxy){
    dialogoInicial->setProxy(proxy);
    dialogoInicial->run();
    std::cout << "termino la carga de datos";
  } else {
    std::cout << "Error: no hay conexion." << std::endl;
  }
}

VentanaPrincipal::VentanaPrincipal(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
: Gtk::Window(cobject),
  m_builder(builder),
  proxy(nullptr) /*,
  m_pButton(nullptr)*/ {
  set_default_size(800,800);
  add_events( Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK );

  this->signal_delete_event().connect(sigc::mem_fun(this,&VentanaPrincipal::onWindowDelete));
}

// The first two parameters are mandatory in a constructor that will be called
// from Gtk::Builder::get_widget_derived().
// Additional parameters, if any, correspond to additional arguments in the call
// to Gtk::Builder::get_widget_derived().
VentanaPrincipal::VentanaPrincipal(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,
  bool is_glad)
: VentanaPrincipal(cobject, builder) // Delegate to the other constructor
{
	// Show an icon.
	/*auto pImage = Gtk::manage(new Gtk::Image());
	pImage->set_from_icon_name(is_glad ? "face-smile" : "face-sad", Gtk::ICON_SIZE_DIALOG);
	pImage->show_all();
	get_content_area()->pack_start(*pImage);*/
}

