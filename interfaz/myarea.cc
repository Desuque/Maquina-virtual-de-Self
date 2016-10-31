#include "myarea.h"
#include <iostream>
#include <cairomm/context.h>
#include <cairomm/enums.h>
#include "morph.h"

MyArea::MyArea()
{
}

MyArea::MyArea(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
: Gtk::DrawingArea(cobject),
  m_builder(builder),
  m_pButton(nullptr),
  m_pButton2(nullptr),
  m_TextView(nullptr)
{
    // Set masks for mouse events
  //add_events(Gdk::BUTTON_PRESS_MASK);
  add_events(Gdk::BUTTON_PRESS_MASK |
         Gdk::BUTTON_RELEASE_MASK |
         Gdk::SCROLL_MASK |
         Gdk::SMOOTH_SCROLL_MASK |
         Gdk::POINTER_MOTION_MASK);
  // ::: Create popup menu :::

  // Add and connect action 1
  MenuItemAgregarSlot.set_label("Agregar Slot");
  MenuItemAgregarSlot.signal_activate().connect(sigc::mem_fun(*this,&MyArea::agregarSlot_event));
  m_Menu_Popup.append(MenuItemAgregarSlot);

  // Show the menu
  m_Menu_Popup.show_all();

  // Connect the menu to this Widget
  //m_Menu_Popup.accelerate(*this);
  
  m_builder-> Gtk::Builder::get_widget("LabelnombreObjeto", lNombreObejto);
  if (lNombreObejto == NULL) std::cout << "error" << std::endl;

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

  resetFlag=true;
  queue_draw();
  moveFlag=false;

  Morph* shell= new Morph("shell",5.,5.,m_TextView);
  actual=shell;
  lNombreObejto -> set_text(actual->nombreObjeto);
  morphs.push_back(shell);
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
  if (m_TextView == NULL) std::cout << "error" << std::endl;
  std::string nombreSlot = textSlot->get_buffer()->get_text();
  actual -> agregarSlot(nombreSlot);
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

// Signal handlers (run when a popup item is clicked)
void MyArea::get_it_event(){
  /*std::cout << "Action 1 selected" << std::endl;
  std::string objeto;
  std::string mensaje;
  std::string str = m_TextView->get_buffer()->get_text();
  std::istringstream iss(str);
  iss >> std::noskipws;
  iss >> objeto  >> mensaje;  
  std::cout << objeto << std::endl;
  if (str == "") objeto = "Hacer que?!";*/
  Morph* morph = new Morph("que?",200,200,m_TextView);
  actual = morph->get_it();
  morphs.push_back(actual); 
  actual -> mostrarDescripcionMorph();
  queue_draw();
}

void MyArea::do_it_event(){
  /*std::cout << "object 2" << std::endl;
  std::string objeto;
  std::string mensaje;
  std::string str = m_TextView->get_buffer()->get_text();
  std::istringstream iss(str);
  iss >> std::noskipws;
  iss >> objeto  >> mensaje;  
  std::cout << objeto << std::endl;*/
  actual -> do_it();
}

void MyArea::close_event(){
  std::cout << "Action 3 selected" << std::endl;
  if (actual == nullptr) return;
  for (int i =0; i < morphs.size() ; ++i){
      if(*(morphs[i]) == *actual){
        // preguntar si alguien lo apunta 
        // para borrar flechas
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
        Morph* slot = actual->clikEnObtenerSlot(event->x,event->y);
        if (slot){
          morphs.push_back(slot);  
        }
        Morph* referencia = actual->clickEnReferenciaAMorph(event->x,event->y);
        if (referencia){
          morphs.push_back(referencia);  
        }
        //actual = morphs[i];
        lastXMouse=actual->posX - event->x;
        lastYMouse=actual->posY - event->y;
        // Start moving the view
        moveFlag=true;
        // va en el morph
        lNombreObejto->set_text(actual->nombreObjeto);
        actual -> mostrarDescripcionMorph();
        // Event has been handled
        return true;      
      }
    }
    /*for (int i =0; i < referencias.size() ; ++i){
     //draw_text(cr, morphs[i].first, morphs[i].second);
      if(*(referencias[i]) == Morph(event->x,event->y)){
        //actual = morphs[i];
        
        lastXMouse=actual->posX - event->x;
        lastYMouse=actual->posY - event->y;
        // Start moving the view
        moveFlag=true;
        // va en el morph
        // Event has been handled
        return true;      
      }
    }*/
     return false;
  }
  // Check if the event is a right button click.
  if(event->button == 3)
  {
    for (int i =0; i < morphs.size() ; ++i){
     //draw_text(cr, morphs[i].first, morphs[i].second);
      
      if(*(morphs[i]) == Morph(event->x,event->y)){
        actual = morphs[i];
        lNombreObejto->set_text(actual->nombreObjeto);
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
      actual->actualizar_posicion((event->x+lastXMouse),(event->y+lastYMouse));
    }
    // Update view
    queue_draw();
    return true;
  }
  // Event has been handled
  return false;
}

// Reset view to fit in the drawing area
void MyArea::fitImage(int winWidth,int winHeight, int imgWidth, int imgHeight)
{
  imgFocusX=imgWidth/2.;
  imgFocusY=imgHeight/2.;

  // Reset is done
  resetFlag=false;
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{  
  for (int i =0; i < morphs.size() ; ++i){
    //draw_text(cr, morphs[i].first, morphs[i].second);
    morphs[i]->draw(cr);
  }
  return true;
}

// no se usa mas.
void MyArea::draw_text(const Cairo::RefPtr<Cairo::Context>& cr,
                       int OffsetX, int OffsetY)
{
  // http://developer.gnome.org/pangomm/unstable/classPango_1_1FontDescription.html
  //Pango::FontDescription font;

  //font.set_family("Monospace");
  //font.set_weight(Pango::WEIGHT_BOLD);

  // http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
  //auto layout = create_pango_layout("Object 2");

  //layout->set_font_description(font);

  //int text_width;
  //int text_height;

  //get the text dimensions (it updates the variables -- by reference)
  //layout->get_pixel_size(text_width, text_height);
  //cr->translate(OffsetX, OffsetY);

  // Position the text in the middle
  //cr->set_source_rgb(0.0, 0.0, 0.0);
  //draw_rectangle(cr, 2*text_width, 2*text_height);
  cr->set_source_rgb(0.0, 0.0, 0.0);

  cr -> rectangle(OffsetX, OffsetY,2*20, 2*20);
  cr-> stroke();
  std::cout << OffsetX << " " << OffsetY << " " << std::endl;
  // and some white text
  cr->set_source_rgb(1.0, 1.0, 1.0);
  cr->move_to((20)/2, (20)/2);

  //layout->show_in_cairo_context(cr);


  /*// Position the text in the middle
  cr->translate(80.0, 80.0);
  
  layout = create_pango_layout("Object 1");

  layout->set_font_description(font);


  //get the text dimensions (it updates the variables -- by reference)
  layout->get_pixel_size(text_width, text_height);

  imgFocusX = text_width/2;
  imgFocusY = text_height/2;

  // Position the text in the middle
  cr->set_source_rgb(0.0, 0.0, 0.0);
  draw_rectangle(cr, 2*text_width, 2*text_height);

  // and some white text
  cr->set_source_rgb(1.0, 1.0, 1.0);
  cr->move_to((text_width)/2, (text_height)/2);

  layout->show_in_cairo_context(cr);*/
}

// NO SE USA!
// Display a target at coordinates x,y
void MyArea::displayTarget(const Cairo::RefPtr<Cairo::Context>& cr,double x, double y)
{
  // Set color to black
  cr->set_source_rgba(0,0,0,0.5);

  // Display black quaters
  cr->arc(x,y,15,0,M_PI/2.);
  cr->line_to(x,y);
  cr->fill();
  cr->arc(x,y,15,M_PI,3.*M_PI/2.);
  cr->line_to(x,y);
  cr->fill();

  // Set color to white
  cr->set_source_rgba(1,1,1,0.5);

  // Display white quaters
  cr->arc(x,y,15,M_PI/2.,M_PI);
  cr->line_to(x,y);
  cr->fill();
  cr->arc(x,y,15,3.*M_PI/2.,0);
  cr->line_to(x,y);
  cr->fill();

  // Set color to black
  cr->set_source_rgba(0,0,0,0.8);

  // Display the cross
  cr->move_to(x-20,y);
  cr->line_to(x+20,y);
  cr->stroke();
  cr->move_to(x,y-20);
  cr->line_to(x,y+20);
  cr->stroke();

  // Display the circle
  cr->arc(x,y,15,0,2*M_PI);
  cr->stroke();
}

// no se usa.
void MyArea::draw_rectangle(const Cairo::RefPtr<Cairo::Context>& cr,
                            int width, int height) 
{
  cr->rectangle(0, 0, width, height);
  cr->fill();
}

