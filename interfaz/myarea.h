#ifndef MYAREA_H
#define MYAREA_H
#include <gtkmm.h>
#include "morph.h"
#include "common_proxyServer.h"
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
  MyArea(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
  MyArea();
  void iniciar();
  //void enviarMorphAlobby();
  // Override mouse events
  bool on_button_press_event(GdkEventButton *event);
  bool on_button_release_event(GdkEventButton *event);
  // Signal handlers (run when a popup item is clicked)
  bool on_motion_notify_event (GdkEventMotion*event);
  void get_it_event();
  void close_event();
  void do_it_event();
  void agregarSlot_event();
  void botonSalirNuevoSlotEvent();
  void botonGuardarNuevoSlotEvent();
  void agregarSlots(std::vector<InterfaceSlot*> i_slots);
  void borrarSlots(std::vector<InterfaceSlot*> i_slots);
  void crearMorphs(std::vector<InterfaceSlot*> i_slots);
  void crearMorphsError(std::vector<InterfaceSlot*> i_slots);
  void mostrarEsteSlotComoMorph(int id_morph, std::string nombre_slot);
  void setProxy(ProxyServer* proxy){
    this->proxyServer=proxy;
  }
  void actualizarPosicionAMoprh(int id, int posX, int posY);
  void closeMorph(int id_morph);
  void recolectarMorphs(std::vector<int> vectorIds);
  void liberarMemoria();
  virtual ~MyArea();

protected:
  //Override default signal handler:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

private:
  void borrarSlot(int idSlot);
  Morph* obtenerMorphPorId(int id_morph);
  void borrarReferenciasDeMorph(Morph* morph);
  void borrarReferenciasDeLosSlotsDeMorph(Morph* morph);
  void borrarMorph(Morph* morph);
  void botonOkSeleccionLobby();
  void actualizarVisualizacionDeSlot(Morph* morph, Slot* slot);
  std::vector<Slot*> obtenerSlotsConEsteId(int id_slot);

};

#endif