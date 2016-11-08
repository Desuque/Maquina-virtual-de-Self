#include "myarea.h"
#include <iostream>
#include <cairomm/context.h>
#include <cairomm/enums.h>
#include "morph.h"
#include "slot.h"
#include "../vm/client_interface_slot.h"
#include "../vm/client_json_reader.h"

#define CREAR_MAQUINA_VIRTUAL 0x01

MyArea::MyArea()
{
}

MyArea::MyArea(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
: Gtk::DrawingArea(cobject),
  m_builder(builder),
  m_pButton(nullptr),
  m_pButton2(nullptr),
  m_TextView(nullptr),
  textViewCodAsociado(nullptr)
{
  add_events(Gdk::BUTTON_PRESS_MASK |
         Gdk::BUTTON_RELEASE_MASK |
         Gdk::SCROLL_MASK |
         Gdk::SMOOTH_SCROLL_MASK |
         Gdk::POINTER_MOTION_MASK);
  // ::: Create popup menu :::

  // conecto agregarSlot
  MenuItemAgregarSlot.set_label("Agregar Slot");
  MenuItemAgregarSlot.signal_activate().connect(sigc::mem_fun(*this,&MyArea::agregarSlot_event));
  m_Menu_Popup.append(MenuItemAgregarSlot);

  m_Menu_Popup.show_all();

  // Connect the menu to this Widget
  //m_Menu_Popup.accelerate(*this);
  
  m_builder-> Gtk::Builder::get_widget("LabelnombreObjeto", lNombreObjeto);
  if (lNombreObjeto == NULL) std::cout << "error" << std::endl;

  m_builder-> Gtk::Builder::get_widget("button1", m_pButton);
  if (m_pButton == NULL) std::cout << "error" << std::endl;
  m_pButton->signal_clicked().connect( sigc::mem_fun(*this,&MyArea::get_it_event));

  m_builder-> Gtk::Builder::get_widget("button2", m_pButton2);
  if (m_pButton2 == NULL) std::cout << "error" << std::endl;
  m_pButton2->signal_clicked().connect( sigc::mem_fun(*this,&MyArea::do_it_event));

  m_builder-> Gtk::Builder::get_widget("button3", m_pButton3);
  if (m_pButton3 == NULL) std::cout << "error" << std::endl;
  m_pButton3->signal_clicked().connect( sigc::mem_fun(*this,&MyArea::close_event));

  m_builder-> Gtk::Builder::get_widget("textview2", m_TextView);
  if (m_TextView == NULL) std::cout << "error" << std::endl;

  m_builder-> Gtk::Builder::get_widget("textview1", textViewCodAsociado);
  if (textViewCodAsociado == nullptr) std::cout << "error" << std::endl; 
	
  resetFlag=true;
  queue_draw();
  moveFlag=false;

  try{
    proxyServer.connect("127.0.0.1",8080);
    proxyServer.enviar(CREAR_MAQUINA_VIRTUAL,sizeof(char));
    //uint32_t lobbyId = proxyServer.recibirId(sizeof(uint32_t));
    // cambiar recibirId por recibirTam
    //uint32_t tamMensaje = proxyServer.recibirId(sizeof(uint32_t));
    std::string infoSlots = proxyServer.recibirSlotsDe("lobby");

    /***DECODIFICACION DEL JSON (CLIENTE)***/
    std::vector<InterfaceSlot*> i_slots;
    JsonReader slots_reader;
    slots_reader.read(i_slots, infoSlots);

    Morph* lobby= new Morph("lobby",10.,10.,m_TextView, textViewCodAsociado);
    actual=lobby;
    lNombreObjeto -> set_text(actual->nombreObjeto);
    morphs.push_back(lobby);

    int size = i_slots.size();
    for (int i = 0; i < size ; i++){
      i_slots[i] -> print_attr();
      actual->agregarSlot(i_slots[i]);
    }
    
    for (std::vector<InterfaceSlot*>::iterator it = i_slots.begin(); it != i_slots.end();){  
      delete* it;  
      it = i_slots.erase(it);
    }

  } catch (const std::exception &e) {
    Morph* lobby= new Morph("lobby",10.,10.,m_TextView,textViewCodAsociado);
    actual=lobby;
    lNombreObjeto -> set_text(actual->nombreObjeto);
    morphs.push_back(lobby);    
  }

}

MyArea::MyArea(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder, bool warning)
: MyArea(cobject,builder){

}

MyArea::~MyArea()
{
}

void MyArea::botonGuardarNuevoSlotEvent(){
  std::cout << "guardar slot" << std::endl;

  Gtk::TextView* textSlot = nullptr;
  m_builder-> Gtk::Builder::get_widget("textview3", textSlot);
  if (textSlot == nullptr) std::cout << "error" << std::endl;
  std::string nombreSlot = textSlot->get_buffer()->get_text();
  actual -> agregarSlot(nombreSlot);
  
  // enviar esto al server.
  std::cout << actual->nombreObjeto << " __addSlots:(|"
            << nombreSlot << "|)."  << std::endl;

  sigcButtonGuardar.disconnect();
  queue_draw();
}

void MyArea::agregarSlot_event(){
  std::cout << "agregar slot" << std::endl;
  Gtk::Button* botonGuardar = nullptr;
  m_builder-> Gtk::Builder::get_widget("button4", botonGuardar);
  if (botonGuardar == NULL) std::cout << "error" << std::endl;
  sigcButtonGuardar = botonGuardar->signal_clicked().connect( sigc::mem_fun(*this,&MyArea::botonGuardarNuevoSlotEvent));
}

void MyArea::get_it_event(){
  Morph* morph = new Morph("que?",200,200,m_TextView,textViewCodAsociado);
  actual = morph->get_it();
  //morph->nombreObjeto=objeto;
  morphs.push_back(actual); 
  actual -> mostrarDescripcionMorph();
  queue_draw();
}

void MyArea::do_it_event(){
  actual -> do_it();
}

void MyArea::close_event(){
  if (actual == nullptr) return;
  for (int i =0; i < morphs.size() ; ++i){
      if(*(morphs[i]) == *actual){
        // preguntar si alguien lo apunta 
        // para borrar flechas
        delete morphs[i];
        morphs.erase(morphs.begin()+i);
        break;
      }
  }
  actual = nullptr;
  queue_draw();
}

//released : process mouse button event
bool MyArea::on_button_release_event(GdkEventButton *event)
{
  // Check if it is the left button
  if (event->button==1 && moveFlag)
  {
    // agrega la referencia para el drag and drop
    // faltaria borrar 
    /*for (int i =0; i < morphs.size() ; ++i){
    //draw_text(cr, morphs[i].first, morphs[i].second);
      if(*(morphs[i]) == Morph(event->x,event->y)){
        // PREGUNTAR ESUNAREFERENCIA
        if(*actual != *morphs[i] && actual->esUnaReferencia()){
          //morphs[i]->slots.push_back(actual);
          std::cout << "borrar ref" << std::endl;
          morphs[i]->referencia = actual;
          //actual -> slotPadre -> referencia = nullptr;
          //actual = morphs[i];
          break;
        }
      }
    }*/
    // End of motion
    moveFlag=false;    
    // Update display
    queue_draw();
    return true;
  }
  // Event has been handled
  return false;
}

// Mouse button pressed : process mouse button event
bool MyArea::on_button_press_event(GdkEventButton *event)
{
  // Check if the event is a left button click.
  if (event->button == 1)
  {
    for (int i =0; i < morphs.size() ; ++i){
     //draw_text(cr, morphs[i].first, morphs[i].second);
      if(*(morphs[i]) == Morph(event->x,event->y)){
        actual = morphs[i];
        Morph* morphDeSlot = actual->clikEnObtenerSlot(event->x,event->y);
        if (morphDeSlot){
          Slot* slot = actual-> obtenerSlot(event->x,event->y);
          if (!slot){
            std::cout << "error obtenerSlot devolvio null" << std::endl;
          }
          std::string infoSlots="";
          
          if (slot->value == "object"){
            std::string infoSlots = proxyServer.recibirSlotsDe(slot->name);
            std::cout << slot->name << std::endl;
            std::cout << slot->value << std::endl;

            //DECODIFICACION DEL JSON (CLIENTE)
            std::vector<InterfaceSlot*> i_slots;
            JsonReader slots_reader;
            slots_reader.read(i_slots, infoSlots);

            int size = i_slots.size();
            for (int i = 0; i < size ; i++){
              i_slots[i] -> print_attr();
              morphDeSlot->agregarSlot(i_slots[i]);
            }
          }
    
          morphs.push_back(morphDeSlot); 
          if (morphDeSlot->referencia){ 
            morphs.push_back(morphDeSlot->referencia);
          }
        }
        offXMouse = actual->posX - event->x;
        offYMouse = actual->posY - event->y;
        // Start moving the view
        moveFlag=true;
        // va en el morph
        lNombreObjeto->set_text(actual->nombreObjeto);
        actual -> mostrarDescripcionMorph();
        // Event has been handled
        return true;      
      }
    }
    return false;
  }
  // Check if the event is a right button click.
  if(event->button == 3)
  {
    for (int i =0; i < morphs.size() ; ++i){
     //draw_text(cr, morphs[i].first, morphs[i].second);
      
      if(*(morphs[i]) == Morph(event->x,event->y)){
        actual = morphs[i];
        lNombreObjeto->set_text(actual->nombreObjeto);
        actual -> mostrarDescripcionMorph();
        m_Menu_Popup.popup(event->button, event->time);
        // Event has been handled
        return true;
      }
    }
  }
  // Event has not been handled
  return false;
}

bool MyArea::on_motion_notify_event (GdkEventMotion*event)
{

  // If the left button is pressed, move the view
  if (moveFlag){
    // Get mouse coordinates
    int XMouse=event->x;
    int YMouse=event->y;
    if(actual != NULL){
      actual->actualizar_posicion((event->x+offXMouse),(event->y+offYMouse ) );
    }
    // Update view
    queue_draw();
    return true;
  }
  // Event has been handled
  return false;
}


bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{  
  for (int i =0; i < morphs.size() ; ++i){
    morphs[i]->draw(cr);
  }
  return true;
}

