#include "ventanaPrincipal.h"
#include <iostream>

VentanaPrincipal::VentanaPrincipal(){

}

VentanaPrincipal::~VentanaPrincipal(){

}

VentanaPrincipal::VentanaPrincipal(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
: Gtk::Window(cobject),
  m_builder(builder) /*,
  m_pButton(nullptr)*/
{
  //Get the Glade-instantiated Button, and connect a signal handler:
  /*m_refGlade->get_widget("quit_button", m_pButton);
  if(m_pButton)
  {
    m_pButton->signal_clicked().connect( sigc::mem_fun(*this, &DerivedDialog::on_button_quit) );
  }*/
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
