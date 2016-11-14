#include "myarea.h"
#include <iostream>
#include <cairomm/context.h>
#include <cairomm/enums.h>
#include "morph.h"
#include "slot.h"
#include "client_interface_slot.h"
#include "client_json_reader.h"

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
    std::string infoSlots = proxyServer.recibirSlotsDe("0");

    /***DECODIFICACION DEL JSON (CLIENTE)***/
    std::vector<InterfaceSlot*> i_slots;
    JsonReader slots_reader;
    slots_reader.read(i_slots, infoSlots);

    Morph* lobby= new Morph("lobby",0,10.,10.,m_TextView, textViewCodAsociado);
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
    Morph* lobby = new Morph("lobby",0,10.,10.,m_TextView,textViewCodAsociado);
    actual=lobby;
    lNombreObjeto -> set_text(actual->nombreObjeto);
    morphs.push_back(lobby);    
  }

}

MyArea::MyArea(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder, bool warning)
: MyArea(cobject,builder){

}

void MyArea::liberarMemoria(){
  std::cout << "libero memoria de morphs" << std::endl;
  for (int i=0; i < morphs.size(); ++i){  
    delete morphs[i];
  }
}


MyArea::~MyArea()
{
}

void MyArea::botonGuardarNuevoSlotEvent(){
  std::cout << "guardar slot" << std::endl;

  Gtk::TextView* textSlot = nullptr;
  m_builder-> Gtk::Builder::get_widget("textview3", textSlot);
  if (textSlot == nullptr) std::cout << "error" << std::endl;
  std::string textoAEnviar = textSlot->get_buffer()->get_text();
  actual -> agregarSlot(textoAEnviar);
  //proxyServer.agregarSlotA(actual->get_id_to_string(), textoAEnviar);
  // enviar esto al server.
  std::cout << actual->nombreObjeto << " __addSlots:(|"
            << textoAEnviar << "|)."  << std::endl;

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
  /*Morph* morph = new Morph("que?",200,200,m_TextView,textViewCodAsociado);
  actual = morph->get_it();
  //morph->nombreObjeto=objeto;
  morphs.push_back(actual); 
  actual -> mostrarDescripcionMorph();*/
  std::string textoAEnviar = actual->get_it();
  uint32_t codigoMensaje = proxyServer.enviarCodigoAEjecutar(actual->get_id_to_string(), textoAEnviar);  
  
  std::cout << codigoMensaje << std::endl;

  switch(codigoMensaje) {
    case 5: { 
      uint32_t tamMensaje = proxyServer.recibirCodigoRespuesta(4);
      std::cout << "tamanio mensaje: " << tamMensaje << std::endl;
      std::string json = proxyServer.recibir(tamMensaje);
      std::cout << json << std::endl;
      std::vector<InterfaceSlot*> i_slots;
      JsonReader slots_reader;
      slots_reader.read(i_slots, json);
      Morph* nuevoMorph = new Morph(i_slots[0]->get_name(),0,250.,550.,m_TextView, textViewCodAsociado);
      morphs.push_back(nuevoMorph);
      break;
    }
    default: { 
      std::cout << "error en default switch MyArea::do_it_event" << std::endl;
      std::cout << "recibio: " << codigoMensaje << std::endl;
      break;
    }
  }
  queue_draw();
}

void borrarSlot(Morph* actual, std::vector<Referencia*> referencias){
    if (actual!= nullptr){
    //std::vector<Slot*> listaSlots = ;
      for (int i = 0; i < (actual->slots).size(); ++i){
        if ((actual->slots)[i]->id == 500){
          std::cout << "elimine slot" << std::endl;
          if ((actual->slots)[i]->estaDibujadoComoMorph()){
            // si esta dibujado borro la referencia al morph
            // que apunta
            std::cout << " esta dibujado como morph\n";
            for (int j = 0; j < referencias.size(); ++j){
              if ((actual->slots)[i]->id
                   == referencias[j]->perteneceASlot->id ){
                for (int v = 0; v < referencias[j]->apuntoAMorph->referencias.size(); ++v){
                  // la comparacion se hace por referencias de memoria
                  if (referencias[j]->apuntoAMorph->referencias[v]
                      == referencias[j]){
                    std::cout << " borro ref de lista de ref de morph\n";
                    referencias[j]->apuntoAMorph->referencias.
                              erase(referencias[j]->apuntoAMorph->referencias.begin()+v);
                  } 
                }
                delete referencias[j];
                std::cout << referencias.size() << std::endl;
                referencias.erase(referencias.begin()+j);
                std::cout << referencias.size() << std::endl;

              }
            }
          }
          delete (actual->slots)[i];
          (actual->slots).erase((actual->slots).begin()+i);
        }
      }
  }
}

void MyArea::do_it_event(){
  std::string textoAEnviar = actual -> do_it();
  
  uint32_t codigoMensaje = proxyServer.enviarCodigoAEjecutar(actual->get_id_to_string(), textoAEnviar);  
  
  std::cout << codigoMensaje << std::endl;

  switch(codigoMensaje) {
    case 5: { 
      uint32_t tamMensaje = proxyServer.recibirCodigoRespuesta(4);
      std::cout << "tamanio mensaje: " << tamMensaje << std::endl;
      std::string json = proxyServer.recibir(tamMensaje);
      std::cout << json << std::endl;
      std::vector<InterfaceSlot*> i_slots;
      JsonReader slots_reader;
      slots_reader.read(i_slots, json);
      /*Morph* nuevoMorph = new Morph(i_slots[0]->get_name(),0,250.,550.,m_TextView, textViewCodAsociado);
      morphs.push_back(nuevoMorph);*/
      std::cout << i_slots[0]->get_name() << std::endl;
      break;
    }
    default: { 
      std::cout << "error en default switch MyArea::do_it_event" << std::endl;
      std::cout << "recibio: " << codigoMensaje << std::endl;
      break;
    }
  }
  /*std::string respuesta = proxyServer.recibirRespuesta();

  std::cout << respuesta << std::endl;
  
  std::vector<InterfaceSlot*> i_slots;
  JsonReader slots_reader;
  slots_reader.read(i_slots, respuesta);*/


  /*int size = 1;
  //int size = i_slots.size();
  for (int i = 0; i < size ; i++){
    i_slots[i] -> print_attr();
    actual->agregarSlot(i_slots[i]);
  }*/
  // borrar slot
  //borrarSlot(actual, referencias);
  
  queue_draw();    
}

void MyArea::close_event(){
  if (actual == nullptr) return;
  /*for (int i =0; i < morphs.size() ; ++i){
      if(*(morphs[i]) == *actual){
        for (int j=0; j < morphs[i]->referencias.size(); ++j){
          for (int v=0; v<referencias.size(); ++v){
            if ((morphs[i]->referencias)[j] == referencias[v]){
              referencias[v]->perteneceASlot->setEstaDibujadoComoMorph(false);
              delete referencias[v];
              referencias.erase(referencias.begin()+v);
            }
          }
        }
        delete morphs[i];
        morphs.erase(morphs.begin()+i);
        break;
      }
  }*/
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
    if (refenciaActual){
      for (int i =0; i < morphs.size() ; ++i){
        if(*(morphs[i]) == Morph(event->x,event->y)){
          refenciaActual->borrarReferenciaAnterior();
          refenciaActual->apuntaAEsteMorph(morphs[i]);
          morphs[i]->referencias.push_back(refenciaActual);
        }
      }
    } 
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
        refenciaActual=nullptr;
        Morph* morphDeSlot = actual->clikEnObtenerSlot(event->x,event->y);
        if (morphDeSlot){
          Slot* slot = actual-> obtenerSlot(event->x,event->y);
          if (!slot){
            std::cout << "error obtenerSlot devolvio null" << std::endl;
          }
          for (int j = 0; j < morphs.size() ; ++j){
            if (morphs[j]->id == morphDeSlot->id){
              std::cout << " ya hay un morph con ese ID" << std::endl;
              Referencia* referenciaNueva = new Referencia(morphs[j],slot);
              morphs[j]->referencias.push_back(referenciaNueva);
              referencias.push_back(referenciaNueva);
              delete morphDeSlot;
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
          std::string infoSlots="";
          if (slot->value == "object"){
            std::string infoSlots = proxyServer.recibirSlotsDe(slot->get_id_to_string());

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
          /*if (morphDeSlot->referencia){ 
           referencias.push_back(morphDeSlot->referencia);
          }*/
          std::vector<Referencia*> refs = morphDeSlot->referencias;
          for (int i=0; i < refs.size(); ++i){
            referencias.push_back(refs[i]);
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
    for (int i =0; i < referencias.size() ; ++i){
      if(*(referencias[i]) == Referencia(event->x,event->y)){
          refenciaActual = referencias[i];
          actual = nullptr;
          offXMouse = refenciaActual->posX - event->x;
          offYMouse = refenciaActual->posY - event->y;
          // Start moving the view
          moveFlag=true;
          return true;
      }
    }
    return false;
  }
  // Check if the event is a right button click.
  if(event->button == 3)
  {
    for (int i =0; i < morphs.size() ; ++i){      
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

bool MyArea::on_motion_notify_event(GdkEventMotion*event)
{

  // If the left button is pressed, move the view
  if (moveFlag){
    // Get mouse coordinates
    int XMouse=event->x;
    int YMouse=event->y;
    if(actual != nullptr){
      actual->actualizar_posicion((event->x+offXMouse),(event->y+offYMouse ) );
    }
    if(refenciaActual != nullptr){
      refenciaActual->actualizar_posicion((event->x+offXMouse),(event->y+offYMouse ) );
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
  for (int i =0; i < referencias.size() ; ++i){
    referencias[i]->draw(cr);
  }
  return true;
}

