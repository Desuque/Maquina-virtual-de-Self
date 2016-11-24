#include "myarea.h"
#include <iostream>
#include <cairomm/context.h>
#include <cairomm/enums.h>
#include "morph.h"
#include "slot.h"
#include "client_interface_slot.h"
#include "client_json_reader.h"
#include "server_json_writer.h"


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

  Gtk::Button* botonGuardar = nullptr;
  m_builder-> Gtk::Builder::get_widget("button6", botonGuardar);
  if (botonGuardar == NULL) std::cout << "error" << std::endl;
  sigcButtonGuardar = botonGuardar->signal_clicked().connect
            ( sigc::mem_fun(*this,&MyArea::botonGuardarNuevoSlotEvent));  

  Gtk::Button* botonSalir = nullptr;
  m_builder-> Gtk::Builder::get_widget("button7", botonSalir);
  if (botonGuardar == NULL) std::cout << "error" << std::endl;
  botonSalir->signal_clicked().connect
            ( sigc::mem_fun(*this,&MyArea::botonSalirNuevoSlotEvent));
  
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

  Morph* lobby = new Morph("lobby", 0, 10., 10., m_TextView, textViewCodAsociado);
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

void MyArea::botonSalirNuevoSlotEvent(){
  Gtk::Dialog* d_add_slot = nullptr;
  m_builder->Gtk::Builder::get_widget("dialog4", d_add_slot);
  d_add_slot->hide();
}

void MyArea::botonGuardarNuevoSlotEvent(){
  std::cout << "guardar slot" << std::endl;

  Gtk::TextView* textSlot = nullptr;
  m_builder-> Gtk::Builder::get_widget("textview3", textSlot);
  if (textSlot == nullptr) std::cout << "error" << std::endl;
  std::string textoDeSlot = textSlot->get_buffer()->get_text();

  Gtk::Dialog* d_add_slot = nullptr;
  m_builder->Gtk::Builder::get_widget("dialog4", d_add_slot);
  d_add_slot->hide();

  std::string textoAEnviar = actual->nombreObjeto;
  textoAEnviar +=  " _AddSlots: (| ";
  textoAEnviar += textoDeSlot;
  textoAEnviar += " |) .";

  std::cout << textoAEnviar << std::endl;
  //std::cout << "lobby _AddSlots: (| a <- 8 . |) ." << std::endl;
  JsonWriter writer;
  string json = writer.write_code(actual->get_id_to_string(), textoAEnviar);
  
  proxyServer->enviarCodigoAEjecutar(actual->get_id_to_string(), json);  
  m_builder->Gtk::Builder::get_widget("dialog4", d_add_slot);


  textSlot->get_buffer()->set_text("");
  queue_draw();
}

void MyArea::agregarSlot_event(){
  std::cout << "agregar slot" << std::endl;
  Gtk::Dialog* d_add_slot = nullptr;
  m_builder->Gtk::Builder::get_widget("dialog4", d_add_slot);
  d_add_slot->run();
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
  /*if(i_slots.size()){
    Morph* nuevoMorph = new Morph(i_slots[0]->get_name(),i_slots[0]->get_id(),250.,550.,m_TextView, textViewCodAsociado);
    morphs.push_back(nuevoMorph);
    queue_draw();
  }*/
  if(i_slots.size()){
    Morph* morph = obtenerMorphPorId(i_slots[0]->get_id());
    if (!morph){
      Morph* nuevoMorph = new Morph(i_slots[0],250.,300.,m_TextView, textViewCodAsociado);
      morphs.push_back(nuevoMorph);
      queue_draw();
      return;
    }
    //actual = morph;
    queue_draw();
    return;
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

Morph* MyArea::obtenerMorphPorId(int id_morph){
  for (int i=0; i < morphs.size() ; ++i){
    if(morphs[i]->get_id() == id_morph){
      return morphs[i];
    }
  }
  return nullptr;
}

void MyArea::borrarReferenciasDeMorph(Morph* morph){
  for (int v=0; v < referencias.size(); ++v){
    for (int j=0; j < morph->referencias.size(); ++j){
      if ((morph->referencias)[j] == referencias[v]){
        referencias[v]->perteneceASlot->setEstaDibujadoComoMorph(false);
        referencias.erase(referencias.begin()+v);
        --v;
      }
    }
  }
}

void MyArea::borrarReferenciasDeLosSlotsDeMorph(Morph* morph){
  for (int j=0; j < morph->slots.size(); ++j){
    for (int v=0; v < referencias.size(); ++v){
      if ((morph->slots)[j]->referencia != nullptr){    
        if ((morph->slots)[j]->referencia == referencias[v]){
          (morph->slots)[j]->setEstaDibujadoComoMorph(false);
          referencias.erase(referencias.begin()+v);
          --v;
        }
      }  
    }
  }
}

void MyArea::closeMorph(int id_morph){
  Morph* morph = obtenerMorphPorId(id_morph);
  if(!morph) return;

  if(actual){
    if(actual->get_id() == morph->get_id()){
      // si el que se borra es el que tengo seleccionado
      // queda como seleccionado el lobby.
      actual = obtenerMorphPorId(0);
    }
  }

  borrarReferenciasDeMorph(morph);
  borrarReferenciasDeLosSlotsDeMorph(morph);
  borrarMorph(morph);
  
  // pongo a lobby como actual.
  //actual = obtenerMorphPorId(0);
  //actual = nullptr;
  queue_draw();
}

void MyArea::borrarMorph(Morph* morph){
  for (int i=0; i < morphs.size() ; ++i){
    if(*(morphs[i]) == *morph){
      delete morphs[i];
      morphs.erase(morphs.begin()+i);
      break;
    }
  }
}

void MyArea::close_event(){
  if (actual == nullptr){
    std::cout << "Error: seleccione el Morph que desea borrar\n";
    return;
  }
  JsonWriter writer;
  std::string json = writer.write_id_morph(actual->get_id());
  // refactorizar
  proxyServer->enviar(12,1);
  proxyServer->enviarJson(json);

  /*actual = obtenerMorphPorId(actual->get_id());  
  borrarReferenciasDeMorph(actual);
  borrarReferenciasDeLosSlotsDeMorph(actual);
  borrarMorph(actual);

  actual = nullptr;*/
  //queue_draw();
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
  std::cout << "agregarSlots" <<std::endl;
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
}

void MyArea::mostrarEsteSlotComoMorph(int id_morph, int id_slot){
  Morph* morph = nullptr;
  for (int i = 0; i < morphs.size() ; ++i){
    if(morphs[i]->get_id() == id_morph){
      morph = morphs[i];
    }
  }
  if(!morph) return;
  
  Slot* slot = morph->obtenerSlotConId(id_slot);
  if(!slot) return;
  if(slot->estaDibujadoComoMorph()) return;

  slot->setEstaDibujadoComoMorph(true);
  for (int j = 0; j < morphs.size() ; ++j){
    if (morphs[j]->tieneElMismoIdQueEsteSlot(slot)){
      Referencia* referenciaNueva = new Referencia(morphs[j],slot);
      morphs[j]->referencias.push_back(referenciaNueva);
      referencias.push_back(referenciaNueva);
      slot->setReferencia(referenciaNueva);
      queue_draw();
      return;
    }
  }
  Morph* nuevoMorph = new Morph(slot, m_TextView, textViewCodAsociado);
  morphs.push_back(nuevoMorph); 
  Referencia* referenciaNueva = new Referencia(nuevoMorph,slot);
  nuevoMorph->referencias.push_back(referenciaNueva);
  referencias.push_back(referenciaNueva);
  slot->setReferencia(referenciaNueva);
  queue_draw();

  if (!(slot->elValorEsPrimitivo())){
    proxyServer->pedirSlotsDe(slot->get_id_to_string());
  }
}

// Mouse button pressed : process mouse button event
bool MyArea::on_button_press_event(GdkEventButton *event)
{
  // Check if the event is a left button click.
  if (event->button == 1)
  {
    for (int i =0; i < morphs.size() ; ++i){
      if(*(morphs[i]) == Morph(event->x,event->y)){
        actual = morphs[i];
        std::cout << actual ->get_id() << std::endl;
        refenciaActual=nullptr;
        Slot* slot = actual-> obtenerSlot(event->x,event->y);
        if (slot && !(slot->estaDibujadoComoMorph())){
          proxyServer->pedirMorphDeEsteSlot(actual->get_id(), slot->get_id());
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

