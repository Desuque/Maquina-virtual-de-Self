#include "myarea.h"
#include <iostream>
#include <cairomm/context.h>
#include <cairomm/enums.h>
#include "morph.h"
#include "slot.h"
#include "client_interface_slot.h"
#include "client_json_reader.h"
#include "server_json_writer.h"

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
  if (lNombreObjeto == nullptr) std::cout << "error" << std::endl;

  m_builder-> Gtk::Builder::get_widget("button1", m_pButton);
  if (m_pButton == nullptr) std::cout << "error" << std::endl;
  m_pButton->signal_clicked().connect( sigc::mem_fun(*this,&MyArea::get_it_event));

  m_builder-> Gtk::Builder::get_widget("button2", m_pButton2);
  if (m_pButton2 == nullptr) std::cout << "error" << std::endl;
  m_pButton2->signal_clicked().connect( sigc::mem_fun(*this,&MyArea::do_it_event));

  m_builder-> Gtk::Builder::get_widget("button3", m_pButton3);
  if (m_pButton3 == nullptr) std::cout << "error" << std::endl;
  m_pButton3->signal_clicked().connect( sigc::mem_fun(*this,&MyArea::close_event));

  m_builder-> Gtk::Builder::get_widget("textview2", m_TextView);
  if (m_TextView == nullptr) std::cout << "error" << std::endl;

  m_builder-> Gtk::Builder::get_widget("textview1", textViewCodAsociado);
  if (textViewCodAsociado == nullptr) std::cout << "error" << std::endl; 
	
  resetFlag=true;
  queue_draw();
  moveFlag=false;
}

void MyArea::iniciar(){
  if(!proxyServer) std::cout << "Error: el socket no fue seteado" << std::endl;
  
  std::string infoSlots = proxyServer->recibirSlotsDe("0");

  Morph* lobby = new Morph("lobby",0,10.,10.,m_TextView, textViewCodAsociado);
  actual = lobby;
  lNombreObjeto->set_text(actual->nombreParaMostrar);
  actual->mostrarDescripcionMorph();
  morphs.push_back(lobby);

  std::vector<InterfaceSlot*> i_slots;
  JsonReader slots_reader;
  slots_reader.read(i_slots, infoSlots);

  int size = i_slots.size();
  for (int i = 0; i < size ; i++){
    i_slots[i] -> print_attr();
    actual->agregarSlot(i_slots[i]);
  }

  for (std::vector<InterfaceSlot*>::iterator it = i_slots.begin(); it != i_slots.end();){  
    delete* it;  
    it = i_slots.erase(it);
  }
}

MyArea::MyArea(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder, bool warning)
: MyArea(cobject,builder){

}

void MyArea::liberarMemoria(){
  for (int i=0; i < morphs.size(); ++i){  
    delete morphs[i];
  }
}

MyArea::~MyArea(){}

void MyArea::botonGuardarNuevoSlotEvent(){

  Gtk::TextView* textSlot = nullptr;
  m_builder-> Gtk::Builder::get_widget("textview3", textSlot);
  if (textSlot == nullptr) std::cout << "error" << std::endl;
  std::string textoDeSlot = textSlot->get_buffer()->get_text();

  std::string textoAEnviar = actual->nombreObjeto;
  textoAEnviar +=  " _AddSlots: (| ";
  textoAEnviar += textoDeSlot;
  textoAEnviar += " |) .";

  std::cout << textoAEnviar << std::endl;
  //std::cout << "lobby _AddSlots: (| a <- 8 . |) ." << std::endl;

  proxyServer->enviarCodigoAEjecutar(actual->get_id_to_string(), textoAEnviar);  
  /*uint32_t codigoMensaje = proxyServer->enviarCodigoAEjecutar(actual->get_id_to_string(), textoAEnviar);  

  uint32_t tamMensaje = proxyServer->recibirCodigo(4);
  std::string json = proxyServer->recibir(tamMensaje);
  std::cout << json << std::endl;
  std::vector<InterfaceSlot*> i_slots;
  JsonReader slots_reader;
  slots_reader.read(i_slots, json);

  int size = 1;
  //int size = i_slots.size();
  for (int i = 0; i < size ; i++){
    i_slots[i] -> print_attr();
    actual->agregarSlot(i_slots[i]);
  }*/

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


void borrarSlot(Morph* actual,int idSlot, std::vector<Referencia*>& referencias){
  if (actual!= nullptr){
  //std::vector<Slot*> listaSlots = ;
    for (int i = 0; i < (actual->slots).size(); ++i){
      if ((actual->slots)[i]->get_id() == idSlot){         
        if ((actual->slots)[i]->estaDibujadoComoMorph()){
          // si esta dibujado borro la referencia al morph
          // que apunta
          for (int j = 0; j < referencias.size(); ++j){
              if ((actual->slots)[i]->referencia == referencias[j]){
                referencias.erase(referencias.begin()+j);
              }
          }
          (actual->slots)[i]->referencia->borrarReferenciaAnterior();
        }
        std::cout << "elimine slot" << std::endl;
        delete (actual->slots)[i];
        (actual->slots).erase((actual->slots).begin()+i);
      }
    }
  }
}

void MyArea::crearMorphs(std::vector<InterfaceSlot*> i_slots){
  if(i_slots.size()){
    Morph* nuevoMorph = new Morph(i_slots[0]->get_name(),i_slots[0]->get_id(),250.,550.,m_TextView, textViewCodAsociado);
    morphs.push_back(nuevoMorph);
    queue_draw();
  }
}

void MyArea::get_it_event(){
  std::string textoAEnviar = actual->get_it();
  
  if (textoAEnviar == ""){
    std::cout << "Error ingresar Codigo" << std::endl;
    return;
  }
  JsonWriter writer;
  string json = writer.write_code(actual->get_id_to_string(), textoAEnviar);

  proxyServer->enviarCodigoAEjecutar(actual->get_id_to_string(), json);  
}

void MyArea::do_it_event(){

  std::string textoAEnviar = actual -> do_it();

  if (textoAEnviar==""){
    std::cout << "Error ingresar Codigo" << std::endl;
    return;
  }

  JsonWriter writer;
  string json = writer.write_code(actual->get_id_to_string(), textoAEnviar);

  proxyServer->enviarCodigoAEjecutar(actual->get_id_to_string(), json);  
}

void MyArea::close_event(){
  if (actual == nullptr){
    std::cout << "Error: seleccione el Morph que desea borrar\n";
    return;
  }  
  for (int v=0; v < referencias.size(); ++v){
    for (int j=0; j < actual->referencias.size(); ++j){
      if ((actual->referencias)[j] == referencias[v]){
        referencias[v]->perteneceASlot->setEstaDibujadoComoMorph(false);
        referencias.erase(referencias.begin()+v);
      }
    }
  }
  for (int j=0; j < actual->slots.size(); ++j){
    for (int v=0; v < referencias.size(); ++v){
      if ((actual->slots)[j]->referencia != nullptr){    
        if ((actual->slots)[j]->referencia == referencias[v]){
          //referencias[v]->perteneceASlot->setEstaDibujadoComoMorph(false);
          (actual->slots)[j]->setEstaDibujadoComoMorph(false);
          referencias.erase(referencias.begin()+v);
        }
      }  
    }
  }
  for (int i =0; i < morphs.size() ; ++i){
    if(*(morphs[i]) == *actual){
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
    if (refenciaActual){
      for (int i =0; i < morphs.size() ; ++i){
        if(*(morphs[i]) == Morph(event->x,event->y)){
          refenciaActual->borrarReferenciaAnterior();
          refenciaActual->apuntaAEsteMorph(morphs[i]);
          // hacer dentro de apuntaAEsteMorph
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

void MyArea::agregarSlots(std::vector<InterfaceSlot*> i_slots){
  if(i_slots.size() == 0){
    return;
  }
  Morph* aux = nullptr;
  for (int i =0; i < morphs.size() ; ++i){
    //if((morphs[i]->tieneElMismoIdQueEsteSlot(i_slots[0]))){
    if((morphs[i]->id == i_slots[0]->get_id_base())){
      aux = morphs[i];
    }
  }
  if(aux){
    //int size = 1;
    int size = i_slots.size();
    for (int i = 0; i < size ; i++){
      i_slots[i] -> print_attr();
      aux->agregarSlot(i_slots[i]);
    }
  }
  queue_draw();
  /*int size = 1;
  //int size = i_slots.size();
  for (int i = 0; i < size ; i++){
    i_slots[i] -> print_attr();
    myArea->agregarSlot(i_slots[i]);
  }*/
}

void MyArea::borrarSlots(std::vector<InterfaceSlot*> i_slots){
  if(i_slots.size() == 0){
    return;
  }
  /*int size = 1;
  //int size = i_slots.size();
  for (int i = 0; i < morphs.size() ; ++i){
    if((morphs[i]->tieneElMismoIdQueEsteSlot(i_slots[0]))){
      i_slots[0] -> print_attr();
      borrarSlot(morphs[i], i_slots[0]->get_id(), referencias);
    }
  }*/
  Morph* aux = nullptr;
  for (int i =0; i < morphs.size() ; ++i){
    //if((morphs[i]->tieneElMismoIdQueEsteSlot(i_slots[0]))){
    if((morphs[i]->id == i_slots[0]->get_id_base())){
      aux = morphs[i];
    }
  }
  if(aux){
    //int size = 1;
    int size = i_slots.size();
    for (int i = 0; i < size ; i++){
      i_slots[i] -> print_attr();
      aux->agregarSlot(i_slots[i]);
      borrarSlot(aux, i_slots[i]->get_id(), referencias);
    }
  }
  queue_draw();
  /*int size = 1;
  //int size = i_slots.size();
  for (int i = 0; i < size ; i++){
    i_slots[i] -> print_attr();
    myArea->agregarSlot(i_slots[i]);
  }*/
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
        Slot* slot = actual-> obtenerSlot(event->x,event->y);
        //Morph* nuevoMorph = nullptr; 
        if (slot && !(slot->estaDibujadoComoMorph())){
          slot->setEstaDibujadoComoMorph(true);
          for (int j = 0; j < morphs.size() ; ++j){
            if (morphs[j]->tieneElMismoIdQueEsteSlot(slot)){
              Referencia* referenciaNueva = new Referencia(morphs[j],slot);
              morphs[j]->referencias.push_back(referenciaNueva);
              referencias.push_back(referenciaNueva);
              slot->setReferencia(referenciaNueva);

              offXMouse = actual->posX - event->x;
              offYMouse = actual->posY - event->y;
              // Start moving the view
              moveFlag=true;
              // va en el morph
              lNombreObjeto->set_text(actual->nombreParaMostrar);
              actual -> mostrarDescripcionMorph();
              // Event has been handled
              return true;                  
            }
          }
          Morph* nuevoMorph = new Morph(slot, m_TextView, textViewCodAsociado);
          morphs.push_back(nuevoMorph); 
          Referencia* referenciaNueva = new Referencia(nuevoMorph,slot);
          nuevoMorph->referencias.push_back(referenciaNueva);
          referencias.push_back(referenciaNueva);
          slot->setReferencia(referenciaNueva);

          if (!(slot->elValorEsPrimitivo())){
            
            proxyServer->pedirSlotsDe(slot->get_id_to_string());
            /*std::string infoSlots = proxyServer->recibirSlotsDe(slot->get_id_to_string());

            //DECODIFICACION DEL JSON (CLIENTE)
            std::vector<InterfaceSlot*> i_slots;
            JsonReader slots_reader;
            slots_reader.read(i_slots, infoSlots);

            int size = i_slots.size();
            for (int i = 0; i < size ; i++){
              i_slots[i] -> print_attr();
              nuevoMorph->agregarSlot(i_slots[i]);
            }*/
          }
        }
        offXMouse = actual->posX - event->x;
        offYMouse = actual->posY - event->y;
        // Start moving the view
        moveFlag=true;
        // va en el morph
        lNombreObjeto->set_text(actual->nombreParaMostrar);
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
        lNombreObjeto->set_text(actual->nombreParaMostrar);
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

void MyArea::actualizarPosicionAMoprh(int id, int posX, int posY){
  for (int i =0; i < morphs.size() ; ++i){      
    if((morphs[i])->get_id() == id){
      morphs[i]->actualizar_posicion(posX,posY);       
    }
  }
  queue_draw();
}

bool MyArea::on_motion_notify_event(GdkEventMotion*event)
{

  // If the left button is pressed, move the view
  if (moveFlag){
    // Get mouse coordinates
    int XMouse=event->x;
    int YMouse=event->y;
    if(actual != nullptr){
      proxyServer->enviar(8,1);
      JsonWriter jsonwriter;
      std::string json = jsonwriter.write_position(actual->get_id(), (event->x+offXMouse), (event->y+offYMouse));
      proxyServer->enviarJson(json);
      //actual->actualizar_posicion((event->x+offXMouse),(event->y+offYMouse));
    }
    if(refenciaActual != nullptr){
      refenciaActual->actualizar_posicion((event->x+offXMouse),(event->y+offYMouse));
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
  if(actual){
    actual->resaltarMorph(cr);
  }
  return true;
}

