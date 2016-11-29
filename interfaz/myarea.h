#ifndef MYAREA_H
#define MYAREA_H
#include <gtkmm.h>
#include "morph.h"
#include "proxyServer.h"
#include "referencia.h"
#include <gtkmm/drawingarea.h>

class MyArea : public Gtk::DrawingArea
{
protected:
	Glib::RefPtr<Gtk::Builder> m_builder;
  Gtk::Menu menuEmergente;
  Gtk::MenuItem itemAgregarSlot;
  Gtk::MenuItem itemEnviarMorphA;
  Gtk::MenuItem itemCloseMorph;

  Gtk::Label* lNombreObjeto;
	Gtk::Button* botonGetIt;
  Gtk::Button* botonDoIt;
  Gtk::Button* botonClose;

  Gtk::TextView* textoShell;
  Gtk::TextView* textoCodigoAsociado;

  bool resetFlag;
  bool moveFlag;
  int offXMouse, offYMouse;
  
  Morph* morphSeleccionado;
  std::vector<Morph*> morphs;

  Referencia* referenciaSeleccionada;
  std::vector<Referencia*> referencias;

  ProxyServer* proxyServer;

public:
  // este constructor me obliga Glade a definirlo asi, si es que quiero tener mi clase.
  // https://developer.gnome.org/gtkmm-tutorial/stable/sec-builder-using-derived-widgets.html.en
  MyArea(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
  MyArea();
  void iniciar();
  /* metodos Overrive de eventos */
  bool on_button_press_event(GdkEventButton *event);
  bool on_button_release_event(GdkEventButton *event);
  bool on_motion_notify_event (GdkEventMotion*event);
  /*******************************/
  void agregarSlots(std::vector<InterfaceSlot*> i_slots);
  void borrarSlots(std::vector<InterfaceSlot*> i_slots);
  void crearMorphs(std::vector<InterfaceSlot*> i_slots);
  void crearMorphsError(std::vector<InterfaceSlot*> i_slots);
  /* Cuando me obtienen el valor de un slot, lo muestro como morph */
  void mostrarEsteSlotComoMorph(int id_morph, std::string nombre_slot);
  void setProxy(ProxyServer* proxy);
  void actualizarPosicionAMoprh(int id, int posX, int posY);
  void closeMorph(int id_morph);
  /* borra los objetos que me envio el garbage collector */
  void recolectarMorphs(std::vector<int> vectorIds);
  /* cuando detecto que me cierran la ventana, antes de salir libero los recursos */
  void liberarMemoria();
  virtual ~MyArea();

protected:
  /* metodo de Gtk::DrawingArea Override 
     detecta los cambios y redibuja */
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

private:
  void borrarSlotDeEsteMorph(Morph* morph, int idSlot);
  Morph* obtenerMorphPorId(int id_morph);
  void borrarReferenciasDeMorph(Morph* morph);
  void borrarReferenciasDeLosSlotsDeMorph(Morph* morph);
  void borrarMorph(Morph* morph);
  void botonOkSeleccionLobby();
  void actualizarVisualizacionDeSlot(Morph* morph, Slot* slot);
  std::vector<Slot*> obtenerSlotsConEsteId(int id_slot);
  /*******************************/
  /*  eventos click en botones de shell */
  void get_it_event();
  void close_event();
  void do_it_event();
  /**************************************/
  /* captura el click en el item agregar slot */
  void agregarSlot_event();
  void botonSalirNuevoSlotEvent();
  /* captura el click en el boton guardar del dialogo agregar slot */
  void botonGuardarNuevoSlotEvent();

};

#endif