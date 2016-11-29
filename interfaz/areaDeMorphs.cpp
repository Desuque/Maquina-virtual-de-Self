#include "areaDeMorphs.h"
#include <iostream>
#include <cairomm/context.h>
#include <cairomm/enums.h>
#include "morph.h"
#include "slot.h"
#include "client_interface_slot.h"
#include "client_json_reader.h"
#include "server_json_writer.h"

#define ACTUALIZAR_VISTA 8
#define COD_SHARE_OBJ 13
#define PEDIR_LISTA_LOBBYS 15
#define GET_IT 16
#define DO_IT 17
#define BORRAR_MORPH 12
// el id de lobby por defecto es 0.
#define ID_DE_LOBBY 0


AreaDeMorphs::AreaDeMorphs(){}

AreaDeMorphs::AreaDeMorphs(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
: Gtk::DrawingArea(cobject), m_builder(builder), botonGetIt(nullptr),
  botonDoIt(nullptr), botonClose(nullptr), textoShell(nullptr),
  textoCodigoAsociado(nullptr), morphSeleccionado(nullptr), 
  referenciaSeleccionada(nullptr), proxyServer(nullptr) {
  
  add_events(Gdk::BUTTON_PRESS_MASK |
         Gdk::BUTTON_RELEASE_MASK |
         Gdk::SCROLL_MASK |
         Gdk::SMOOTH_SCROLL_MASK |
         Gdk::POINTER_MOTION_MASK);
  // ::: Create popup menu :::

  // conecto agregarSlot
  itemAgregarSlot.set_label("Agregar Slot");
  itemAgregarSlot.signal_activate().connect(sigc::mem_fun(*this,&AreaDeMorphs::agregarSlot_event));
  menuEmergente.append(itemAgregarSlot);
  
  /*itemEnviarMorphA.set_label("Enviar a...");
  itemEnviarMorphA.signal_activate().connect(sigc::mem_fun(*this,&AreaDeMorphs::enviarMorphAlobby));
  menuEmergente.append(itemEnviarMorphA);*/  

  itemCloseMorph.set_label("Close");
  itemCloseMorph.signal_activate().connect(sigc::mem_fun(*this,&AreaDeMorphs::close_event));
  menuEmergente.append(itemCloseMorph);

  menuEmergente.show_all();

  // Connect the menu to this Widget
  //menuEmergente.accelerate(*this);

  Gtk::Button* botonGuardar = nullptr;
  m_builder-> Gtk::Builder::get_widget("button6", botonGuardar);
  if (botonGuardar == nullptr){
    std::cout << "Error Glade" << std::endl;
    throw new std::exception();
  } 
  botonGuardar->signal_clicked().connect(sigc::mem_fun(*this,&AreaDeMorphs::botonGuardarNuevoSlotEvent));  

  Gtk::Button* botonSalir = nullptr;
  m_builder-> Gtk::Builder::get_widget("button7", botonSalir);
  if (botonGuardar == nullptr){
    std::cout << "Error Glade" << std::endl;
    throw new std::exception();
  } 
  botonSalir->signal_clicked().connect
            ( sigc::mem_fun(*this,&AreaDeMorphs::botonSalirNuevoSlotEvent));
  
  m_builder-> Gtk::Builder::get_widget("LabelnombreObjeto", lNombreObjeto);
  if (lNombreObjeto == nullptr){
    std::cout << "Error Glade" << std::endl;
    throw new std::exception();
  } 

  m_builder-> Gtk::Builder::get_widget("button1", botonGetIt);
  if (botonGetIt == nullptr) {
    std::cout << "Error Glade" << std::endl;
    throw new std::exception();
  } 
  botonGetIt->signal_clicked().connect( sigc::mem_fun(*this,&AreaDeMorphs::get_it_event));

  m_builder-> Gtk::Builder::get_widget("button2", botonDoIt);
  if (botonDoIt == nullptr) {
    std::cout << "Error Glade" << std::endl;
    throw new std::exception();
  } 
  botonDoIt->signal_clicked().connect( sigc::mem_fun(*this,&AreaDeMorphs::do_it_event));

  m_builder-> Gtk::Builder::get_widget("button3", botonClose);
  if (botonClose == nullptr) {
    std::cout << "Error Glade" << std::endl;
    throw new std::exception();
  } 
  botonClose->signal_clicked().connect( sigc::mem_fun(*this,&AreaDeMorphs::close_event));

  m_builder-> Gtk::Builder::get_widget("textview2", textoShell);
  if (textoShell == nullptr){
    std::cout << "Error Glade" << std::endl;
    throw new std::exception();
  } 

  m_builder-> Gtk::Builder::get_widget("textview1", textoCodigoAsociado);
  if (textoCodigoAsociado == nullptr) {
    std::cout << "Error Glade" << std::endl;
    throw new std::exception();
  } 
  
  resetFlag=true;
  queue_draw();
  moveFlag=false;
}

void AreaDeMorphs::iniciar(){
  if(!proxyServer){
    std::cout << "Error: el proxy no fue seteado" << std::endl;
    throw new std::exception();
  }  

  // pido los slots de lobby, por defecto es 0.
  std::string infoSlots = proxyServer->recibirSlotsDe("0");

  Morph* lobby = new Morph("lobby", ID_DE_LOBBY, 10., 10., textoShell, textoCodigoAsociado);
  morphSeleccionado = lobby;
  lNombreObjeto->set_text(morphSeleccionado->getNombreParaMostrar());
  morphSeleccionado->mostrarDescripcionMorph();
  morphs.push_back(lobby);

  std::vector<InterfaceSlot*> i_slots;
  JsonReader slots_reader;
  slots_reader.read(i_slots, infoSlots);

  int size = i_slots.size();
  for (int i = 0; i < size ; i++){
    morphSeleccionado->agregarSlot(i_slots[i]);
  }
  
  for (std::vector<InterfaceSlot*>::iterator it = i_slots.begin(); it != i_slots.end();){  
    delete* it;  
    it = i_slots.erase(it);
  }
}

void AreaDeMorphs::setProxy(ProxyServer* proxy){
  this->proxyServer=proxy;
}


void AreaDeMorphs::liberarMemoria(){
  for (int i=0; i < morphs.size(); ++i){  
    delete morphs[i];
  }
}

AreaDeMorphs::~AreaDeMorphs(){}

void AreaDeMorphs::botonSalirNuevoSlotEvent(){
  Gtk::Dialog* dialogoAddSlot = nullptr;
  m_builder->Gtk::Builder::get_widget("dialog4", dialogoAddSlot);
  dialogoAddSlot->hide();
}

void AreaDeMorphs::botonGuardarNuevoSlotEvent(){
  Gtk::TextView* textSlot = nullptr;
  m_builder-> Gtk::Builder::get_widget("textview3", textSlot);
  if (textSlot == nullptr) {
    std::cout << "Error Glade." << std::endl;
    throw new std::exception();
  }
  std::string textoDeSlot = textSlot->get_buffer()->get_text();

  Gtk::Dialog* dialogoAddSlot = nullptr;
  m_builder->Gtk::Builder::get_widget("dialog4", dialogoAddSlot);
  dialogoAddSlot->hide();

  std::string textoAEnviar = morphSeleccionado->getNombre();
  textoAEnviar +=  " _AddSlots: (| ";
  textoAEnviar += textoDeSlot;
  textoAEnviar += " |) .";

  JsonWriter writer;
  string json = writer.write_code(morphSeleccionado->get_id_to_string(), textoAEnviar);
  
  //se manda DO_IT para respetar el protocolo pero no varia en este caso.
  proxyServer->enviarCodigoAEjecutar(morphSeleccionado->get_id_to_string(), json, DO_IT);  

  textSlot->get_buffer()->set_text("");
  queue_draw();
}

void AreaDeMorphs::agregarSlot_event(){
  Gtk::Dialog* dialogoAddSlot = nullptr;
  m_builder->Gtk::Builder::get_widget("dialog4", dialogoAddSlot);
  if (dialogoAddSlot == nullptr) {
    std::cout << "Error Glade." << std::endl;
    throw new std::exception();
  }
  dialogoAddSlot->run();
}




void AreaDeMorphs::crearMorphs(std::vector<InterfaceSlot*> i_slots){
  if(i_slots.size()){
    Morph* morph = obtenerMorphPorId(i_slots[0]->get_id());
    if (!morph){
      Morph* nuevoMorph = new Morph(i_slots[0],250.,300.,textoShell, textoCodigoAsociado);
      morphs.push_back(nuevoMorph);
      if (!(i_slots[0]->elValorEsPrimitivo())){
        proxyServer->pedirSlotsDe(nuevoMorph->get_id_to_string());
      }
      queue_draw();
      return;
    }
    queue_draw();
    return;
  }
}

void AreaDeMorphs::crearMorphsError(std::vector<InterfaceSlot*> i_slots){
  crearMorphs(i_slots);
}

void AreaDeMorphs::get_it_event(){
  std::string textoAEnviar = morphSeleccionado->get_it();
  
  if (textoAEnviar == ""){
    std::cout << "Error: Por favor, ingresar Codigo" << std::endl;
    return;
  }
  JsonWriter writer;
  string json = writer.write_code(morphSeleccionado->get_id_to_string(), textoAEnviar);

  proxyServer->enviarCodigoAEjecutar(morphSeleccionado->get_id_to_string(), json, GET_IT);  
}

void AreaDeMorphs::do_it_event(){

  std::string textoAEnviar = morphSeleccionado -> do_it();

  if (textoAEnviar == ""){
    std::cout << "Error: Por favor, ingresar Codigo" << std::endl;
    return;
  }

  JsonWriter writer;
  string json = writer.write_code(morphSeleccionado->get_id_to_string(), textoAEnviar);

  proxyServer->enviarCodigoAEjecutar(morphSeleccionado->get_id_to_string(), json, DO_IT);  
}

Morph* AreaDeMorphs::obtenerMorphPorId(int id_morph){
  for (int i=0; i < morphs.size() ; ++i){
    if(morphs[i]->get_id() == id_morph){
      return morphs[i];
    }
  }
  return nullptr;
}

void AreaDeMorphs::borrarReferenciasDeMorph(Morph* morph){
  std::vector<Referencia*> referecniasDeMorph = morph->getReferencias();
  for (int v=0; v < referencias.size(); ++v){
    for (int j=0; j < referecniasDeMorph.size(); ++j){
      if ((referecniasDeMorph)[j] == referencias[v]){
        referencias[v]->setSlotEstaDibujadoComoMorph(false);
        referencias.erase(referencias.begin()+v);
        --v;
      }
    }
  }
}


void AreaDeMorphs::borrarReferenciasDeLosSlotsDeMorph(Morph* morph){
  std::vector<Slot*> slotsDeMorph = morph->getSlots();
  for (int j=0; j < slotsDeMorph.size(); ++j){
    // pido la lista de slots de cada morph.
    for (int v=0; v < referencias.size(); ++v){
        if ((slotsDeMorph)[j]->tieneEstaReferencia(referencias[v])){
          (slotsDeMorph)[j]->setEstaDibujadoComoMorph(false);
          referencias.erase(referencias.begin()+v);
          --v;
        }
    }
  }
}

void AreaDeMorphs::recolectarMorphs(std::vector<int> vectorIds){
  for(int i = 0; i < vectorIds.size(); ++i){
    closeMorph(vectorIds[i]);
  }
}

void AreaDeMorphs::closeMorph(int id_morph){
  Morph* morph = obtenerMorphPorId(id_morph);
  if(!morph) return;


  if(morphSeleccionado){
    if(morphSeleccionado->get_id() == morph->get_id()){
      // si el que se borra es el que tengo seleccionado
      // queda como seleccionado el lobby(su id por defecto siempre es 0).
      morphSeleccionado = obtenerMorphPorId(ID_DE_LOBBY);
    }
  }

  borrarReferenciasDeMorph(morph);
  borrarReferenciasDeLosSlotsDeMorph(morph);
  borrarMorph(morph);
  
  queue_draw();
}

void AreaDeMorphs::borrarMorph(Morph* morph){
  for (int i=0; i < morphs.size() ; ++i){
    if(*(morphs[i]) == *morph){
      delete morphs[i];
      morphs.erase(morphs.begin()+i);
      break;
    }
  }
}

void AreaDeMorphs::close_event(){
  if (morphSeleccionado == nullptr){
    std::cout << "Seleccione el Morph que desea borrar\n";
    return;
  }
  if(morphSeleccionado->get_id() == ID_DE_LOBBY){
    // pregunto si quieren cerrar Lobby no se lo permito.
    //std::cout << "Error: No se puede cerrar el objeto Lobby";
    return; 
  }
  JsonWriter writer;
  std::string json = writer.write_id_morph(morphSeleccionado->get_id());

  proxyServer->enviarCodigoMensaje(BORRAR_MORPH);
  proxyServer->enviarJson(json);
}



// detecta que se solto el boton del mouse
bool AreaDeMorphs::on_button_release_event(GdkEventButton *event)
{
  // chequeo si es el boton izquierdo del mouse
  if (event->button==1 && moveFlag)
  {
    if (referenciaSeleccionada){
      for (int i =0; i < morphs.size() ; ++i){
        if(*(morphs[i]) == Morph(event->x,event->y)){
          referenciaSeleccionada->borrarReferenciaAlMorphApuntado();
          referenciaSeleccionada->apuntaAEsteMorph(morphs[i]);
          morphs[i]->agregarReferencia(referenciaSeleccionada);
        }
      }
    } 
    // fin del movimiento
    moveFlag=false;    
    // actualizo 
    queue_draw();
    return true;
  }
  return false;
}

std::vector<Slot*> AreaDeMorphs::obtenerSlotsConEsteId(int id_slot){
  std::vector<Slot*> listaSlots;
  for (int i =0; i < morphs.size() ; ++i){
    std::vector<Slot*> lista = morphs[i]->obtenerSlotsConId(id_slot);
    // concanteno las listas.
    for (int j=0; j < lista.size(); ++j){
      listaSlots.push_back(lista[j]);
    }
  }
  return listaSlots;
}

void AreaDeMorphs::actualizarVisualizacionDeSlot(Morph* morph, Slot* slot){    
  if(slot->estaDibujadoComoMorph()){
      slot->borrarReferenciaAlMorphApuntado();
      // borro la referencia de la lista de dibujadas
      // para que no se dibuje.
      for (int j=0; j < referencias.size(); ++j){
        if (slot->tieneEstaReferencia(referencias[j])){
          slot->setEstaDibujadoComoMorph(false);
          referencias.erase(referencias.begin()+j);
          --j;
        }
      }
      mostrarEsteSlotComoMorph(morph->get_id(), slot->get_name());
    }
}

void AreaDeMorphs::agregarSlots(std::vector<InterfaceSlot*> i_slots){
  for (int i = 0; i < i_slots.size() ; i++){
    Morph* morphAux = obtenerMorphPorId(i_slots[i]->get_id_base());
    if(morphAux){
      int size = i_slots.size();
      for (int i = 0; i < size ; i++){
        // me fijo si hay un slot con ese nombre, lo debo modificar.
        Slot* slot = morphAux->obtenerSlotConEsteNombre(i_slots[i]->get_name());
        if(slot){
          slot->actualizarSlot(i_slots[i]);
          if(slot->estaDibujadoComoMorph()){
            slot->borrarReferenciaAlMorphApuntado();
            for (int j=0; j < referencias.size(); ++j){
              if (slot->tieneEstaReferencia(referencias[j])){
                slot->setEstaDibujadoComoMorph(false);
                referencias.erase(referencias.begin()+j);
                --j;
              }
            }
            mostrarEsteSlotComoMorph(morphAux->get_id(), slot->get_name());
          }
          continue;
        }
        morphAux->agregarSlot(i_slots[i]);
      }
    }  
  }
  queue_draw();
}

void AreaDeMorphs::borrarSlotDeEsteMorph(Morph* morph, int idSlot){
  // pido una referencia a lista de slots por que necesito
  // eliminarlas las coincidencias.
  if (morph!= nullptr){
    // recibo una referencia a la lista por que la debo modificar.
    std::vector<Slot*>& listaSlots = morph->getSlots();
    for (int i = 0; i < (listaSlots).size(); ++i){
      if ((listaSlots)[i]->get_id() == idSlot){         
        if ((listaSlots)[i]->estaDibujadoComoMorph()){
          // si esta dibujado borro la referencia al morph
          // que apunta
          for (int j = 0; j < referencias.size(); ++j){
              if ((listaSlots)[i]->tieneEstaReferencia(referencias[j])){
                referencias.erase(referencias.begin()+j);
                // este delete se hace cuando le borro la referencia
                // al morph, ya que comparten la referencia.
                --j;
              }
          }
          (listaSlots)[i]->borrarReferenciaAlMorphApuntado();
        }
        delete (listaSlots)[i];
        (listaSlots).erase((listaSlots).begin()+i);
        --i;
      }
    }
  }
}

void AreaDeMorphs::borrarSlots(std::vector<InterfaceSlot*> i_slots){
  if(i_slots.size() == 0){
    return;
  }
  Morph* aux = nullptr;
  for (int i =0; i < morphs.size() ; ++i){
    if((morphs[i]->get_id() == i_slots[0]->get_id_base())){
      aux = morphs[i];
    }
  }
  if(aux){
    int size = i_slots.size();
    for (int i = 0; i < size ; i++){
      borrarSlotDeEsteMorph(aux, i_slots[i]->get_id());
    }
  }
  queue_draw();
}


void AreaDeMorphs::mostrarEsteSlotComoMorph(int id_morph, std::string nombreSlot){
  Morph* morph = nullptr;
  for (int i = 0; i < morphs.size() ; ++i){
    if(morphs[i]->get_id() == id_morph){
      morph = morphs[i];
    }
  }
  if(!morph) return;
  
  Slot* slot = morph->obtenerSlotConEsteNombre(nombreSlot);
  if(!slot) return;
  if(slot->estaDibujadoComoMorph()) return;

  slot->setEstaDibujadoComoMorph(true);
  for (int j = 0; j < morphs.size() ; ++j){
    if (morphs[j]->tieneElMismoIdQueEsteSlot(slot)){
      Referencia* referenciaNueva = new Referencia(morphs[j],slot);
      morphs[j]->agregarReferencia(referenciaNueva);
      referencias.push_back(referenciaNueva);
      slot->setReferencia(referenciaNueva);
      queue_draw();
      return;
    }
  }
  Morph* nuevoMorph = new Morph(slot, textoShell, textoCodigoAsociado);
  morphs.push_back(nuevoMorph); 
  Referencia* referenciaNueva = new Referencia(nuevoMorph,slot);
  nuevoMorph->agregarReferencia(referenciaNueva);
  referencias.push_back(referenciaNueva);
  slot->setReferencia(referenciaNueva);
  queue_draw();

  if (!(slot->elValorEsPrimitivo())){
    proxyServer->pedirSlotsDe(slot->get_id_to_string());
  }
}

// Mouse button pressed : process mouse button event
bool AreaDeMorphs::on_button_press_event(GdkEventButton *event)
{
  // Check if the event is a left button click.
  if (event->button == 1)
  {
    for (int i =0; i < morphs.size() ; ++i){
      if(*(morphs[i]) == Morph(event->x,event->y)){
        morphSeleccionado = morphs[i];
        referenciaSeleccionada=nullptr;
        Slot* slot = morphSeleccionado-> obtenerSlot(event->x,event->y);
        if (slot && !(slot->estaDibujadoComoMorph())){
          proxyServer->pedirMorphDeEsteSlot(morphSeleccionado->get_id(), slot->get_name());
        }
        offXMouse = morphSeleccionado->getPosX() - event->x;
        offYMouse = morphSeleccionado->getPosY() - event->y;
        // Start moving the view
        moveFlag=true;
        // va en el morph
        lNombreObjeto->set_text(morphSeleccionado->getNombreParaMostrar());
        morphSeleccionado -> mostrarDescripcionMorph();
        // Event has been handled
        return true;    
      }
    }    
    for (int i =0; i < referencias.size() ; ++i){
      if(*(referencias[i]) == Referencia(event->x,event->y)){
          referenciaSeleccionada = referencias[i];
          morphSeleccionado = nullptr;
          offXMouse = referenciaSeleccionada->getPosX() - event->x;
          offYMouse = referenciaSeleccionada->getPosY() - event->y;
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
        morphSeleccionado = morphs[i];
        lNombreObjeto->set_text(morphSeleccionado->getNombreParaMostrar());
        morphSeleccionado -> mostrarDescripcionMorph();
        menuEmergente.popup(event->button, event->time);
        // Event has been handled
        return true;
      }
    }
  }
  // Event has not been handled
  return false;
}

void AreaDeMorphs::actualizarPosicionAMoprh(int id, int posX, int posY){
  for (int i =0; i < morphs.size() ; ++i){      
    if((morphs[i])->get_id() == id){
      morphs[i]->actualizar_posicion(posX,posY);       
    }
  }
  queue_draw();
}

bool AreaDeMorphs::on_motion_notify_event(GdkEventMotion*event)
{
  if (moveFlag){
    if(morphSeleccionado != nullptr){
      double posXFinal = event->x + offXMouse;
      double posYFinal = event->y + offYMouse;
      if (event->y < 0){
        posYFinal=0;
      }
      if (event->x < 0){
        posXFinal=0;
      }
      proxyServer->enviarCodigoMensaje(ACTUALIZAR_VISTA);
      JsonWriter jsonwriter;
      std::string json = jsonwriter.write_position(morphSeleccionado->get_id(), posXFinal, posYFinal);
      proxyServer->enviarJson(json);

    }
    if(referenciaSeleccionada != nullptr){
      referenciaSeleccionada->actualizar_posicion((event->x+offXMouse),(event->y+offYMouse));
    }
    queue_draw();
    return true;
  }
  return false;
}

bool AreaDeMorphs::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{  
  for (int i =0; i < morphs.size() ; ++i){
    morphs[i]->draw(cr);
  }
  for (int i =0; i < referencias.size() ; ++i){
    referencias[i]->draw(cr);
  }
  if(morphSeleccionado){
    morphSeleccionado->resaltarMorph(cr);
  }
  return true;
}



