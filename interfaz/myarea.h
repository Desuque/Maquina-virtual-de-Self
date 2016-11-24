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
  Gtk::Menu m_Menu_Popup;
  Gtk::MenuItem MenuItemAgregarSlot;

  Gtk::Label* lNombreObjeto;
	Gtk::Button* m_pButton;
  Gtk::Button* m_pButton2;
  Gtk::Button* m_pButton3;

  sigc::connection sigcButtonGuardar;
  Gtk::TextView* m_TextView;
  Gtk::TextView* textViewCodAsociado;

  bool resetFlag;
  bool moveFlag;
  Morph* actual;
  std::vector<Morph*> morphs;

  Referencia* refenciaActual = nullptr;
  std::vector<Referencia*> referencias;

  int offXMouse, offYMouse;

  ProxyServer* proxyServer;

public:
  MyArea(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder, bool warning);
  MyArea(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
  MyArea();
  void iniciar();
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
  void liberarMemoria();
  void agregarSlots(std::vector<InterfaceSlot*> i_slots);
  void borrarSlots(std::vector<InterfaceSlot*> i_slots);
  void crearMorphs(std::vector<InterfaceSlot*> i_slots);
  void mostrarEsteSlotComoMorph(int id_morph, int id_slot);
  void setProxy(ProxyServer* proxy){
    this->proxyServer = proxy;
  }

  void actualizarPosicionAMoprh(int id, int posX, int posY);
  void closeMorph(int id_morph);

  virtual ~MyArea();

protected:
  //Override default signal handler:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

private:
  Morph* obtenerMorphPorId(int id_morph);
  void borrarReferenciasDeMorph(Morph* morph);
  void borrarReferenciasDeLosSlotsDeMorph(Morph* morph);
  void borrarMorph(Morph* morph);

};

#endif