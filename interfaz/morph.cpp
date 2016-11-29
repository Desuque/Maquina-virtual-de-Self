#include "morph.h"
#include "slot.h"
#include "referencia.h"
#include <gtkmm.h>
#include <pangomm/layout.h>

Morph::Morph(){}

// se usa para comparar morph por posiciones.
Morph::Morph(double posX, double posY) : 
	m_TextView(nullptr), refTextViewConsola(nullptr), 
	posX(posX), posY(posY){}

Morph::Morph(std::string nombreObjeto, int id, double posX, double posY, 
		Gtk::TextView* m_TextView, Gtk::TextView* codigoAsociado) : 
		nombreObjeto(nombreObjeto), nombreParaMostrar(nombreObjeto), 
		posX(posX), posY(posY), m_TextView(m_TextView),
		textViewCodigoAsociado(codigoAsociado) {
	Pango::FontDescription font;
	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_BOLD);
	// http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
	auto layout = create_pango_layout(this->nombreObjeto);
	layout->set_font_description(font);
	int text_width;
	int text_height;
	layout->get_pixel_size(text_width, text_height);
	this->width = 2*text_width;
	this->height = 2*text_height;
	refTextViewConsola = Gtk::TextBuffer::create();
	refTextViewConsola->set_text("");
	refTextViewCodigoAsociado = Gtk::TextBuffer::create();
	refTextViewCodigoAsociado->set_text("");
	this->id=id;
}

Morph::Morph(Slot* unSlot){
	this->id = unSlot->get_id();
	this->nombreObjeto = unSlot->get_name();
	if(unSlot->has_code()){
		this->nombreParaMostrar = unSlot->get_name();
		refTextViewCodigoAsociado = Gtk::TextBuffer::create();
		refTextViewCodigoAsociado->set_text(unSlot->get_value());
	}else{
		this->nombreParaMostrar = unSlot->get_value();
	}
	Pango::FontDescription font;
	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_BOLD);
	auto layout = create_pango_layout(this->nombreParaMostrar);
	layout->set_font_description(font);
	int text_width;
	int text_height;
	layout->get_pixel_size(text_width, text_height);
	this->width = 2*text_width;
	this->height = 2*text_height;
}

Morph::Morph(Slot* unSlot, Gtk::TextView* m_TextView, Gtk::TextView* codigoAsociado) :
	m_TextView(m_TextView), textViewCodigoAsociado(codigoAsociado) {
	this->posX = unSlot->posX + unSlot->width + 20;
	this->posY = unSlot->posY+4;
	this->id = unSlot->get_id();
	refTextViewCodigoAsociado = Gtk::TextBuffer::create();
	refTextViewCodigoAsociado->set_text("");
	this->nombreObjeto = unSlot->get_name();
	if(unSlot->has_code()){
		this->nombreParaMostrar = unSlot->get_name();
		refTextViewCodigoAsociado = Gtk::TextBuffer::create();
		refTextViewCodigoAsociado->set_text(unSlot->get_value());
	}else{
		this->nombreParaMostrar = unSlot->get_value();
	}
	Pango::FontDescription font;
	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_BOLD);
	auto layout = create_pango_layout(this->nombreParaMostrar);
	layout->set_font_description(font);
	int text_width;
	int text_height;
	layout->get_pixel_size(text_width, text_height);
	this->width = 2*text_width;
	this->height = 2*text_height;

	refTextViewConsola = Gtk::TextBuffer::create();
	refTextViewConsola->set_text("");
} 

Morph::Morph(InterfaceSlot* unSlot,double posX, double posY, Gtk::TextView* m_TextView, Gtk::TextView* codigoAsociado) :
	m_TextView(m_TextView), textViewCodigoAsociado(codigoAsociado) {
	this->posX = posX;
	this->posY = posY;
	this->id = unSlot->get_id();
	refTextViewCodigoAsociado = Gtk::TextBuffer::create();
	refTextViewCodigoAsociado->set_text("");
	this->nombreObjeto = unSlot->get_name();
	if(unSlot->has_code()){
		this->nombreParaMostrar = unSlot->get_name();
		std::cout << unSlot->get_name() << std::endl;
		refTextViewCodigoAsociado = Gtk::TextBuffer::create();
		refTextViewCodigoAsociado->set_text(unSlot->get_value());
	}else{
		this->nombreParaMostrar = unSlot->get_value();
	}
	Pango::FontDescription font;
	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_BOLD);
	auto layout = create_pango_layout(this->nombreParaMostrar);
	layout->set_font_description(font);
	int text_width;
	int text_height;
	layout->get_pixel_size(text_width, text_height);
	this->width = 2*text_width;
	this->height = 2*text_height;

	refTextViewConsola = Gtk::TextBuffer::create();
	refTextViewConsola->set_text("");
} 

void Morph::actualizarValores(Slot* unSlot){
	this->id = unSlot->get_id();
	this->nombreObjeto = unSlot->get_name();
	if(unSlot->has_code()){
		this->nombreParaMostrar = unSlot->get_name();
		refTextViewCodigoAsociado = Gtk::TextBuffer::create();
		refTextViewCodigoAsociado->set_text(unSlot->get_value());
	}else{
		this->nombreParaMostrar = unSlot->get_value();
	}
	Pango::FontDescription font;
	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_BOLD);
	auto layout = create_pango_layout(this->nombreParaMostrar);
	layout->set_font_description(font);
	int text_width;
	int text_height;
	layout->get_pixel_size(text_width, text_height);
	this->width = 2*text_width;
	this->height = 2*text_height;
}

std::string Morph::get_id_to_string(){
	return std::to_string(this->id);
}

int Morph::get_id(){
	return this->id;
}

// devuelve una referencia a la lista de slots.
// si o si la necesito modificar desde el area de dibujo.
std::vector<Slot*>& Morph::getSlots(){
	return this->slots;
}

std::vector<Referencia*> Morph::getReferencias(){
	return this->referencias;
}

double Morph::getPosX(){
	return this->posX;
}

std::string Morph::getNombreParaMostrar(){
	return this->nombreParaMostrar;
}

std::string Morph::getNombre(){
	return this->nombreObjeto;
}

double Morph::getPosY(){
	return this->posY;
}

int Morph::getWidth(){
	return this->width;
}

int Morph::getHeight(){
	return this->height;
}

std::string Morph::get_it(){
	if (!m_TextView) std::cout << "error" << std::endl;
	std::string textoAEnviar = m_TextView->get_buffer()->get_text();
	return textoAEnviar;
}

std::string Morph::do_it(){
	if (!m_TextView) std::cout << "error" << std::endl;
	std::string textoAEnviar = m_TextView->get_buffer()->get_text();
	return textoAEnviar;
}

void Morph::actualizarAlturaMorph(size_t alturaDeSlot){
	this->height += alturaDeSlot;
}

void Morph::agregarSlot(InterfaceSlot* interface_slot){
	for(int i=0 ; i < slots.size() ; ++i){
		if(slots[i]->get_name()==interface_slot->get_name()){
			
			return;
		}
	}
	Slot* slot = new Slot(interface_slot,this->posX, (this->posY)+(this->height), this->width, this->height);
	this->actualizarAlturaMorph(slot->getHeight());
	slots.push_back(slot);
}

void Morph::resaltarMorph(const Cairo::RefPtr<Cairo::Context>& cr){
	cr->set_source_rgba(0.0, 0.5, 0.5,0.5);
	cr -> rectangle(this->posX, this -> posY,this->width, this->height);
	cr-> fill();
}

void Morph::draw(const Cairo::RefPtr<Cairo::Context>& cr){

	cr->set_line_width(2.5);
	cr->set_source_rgb(0.0, 0.0, 0.0);
	
	cr -> rectangle(this->posX, this -> posY,this->width, this->height);
	//cr ->fill();
	cr-> stroke();
	
	// solo para dibujar los errores.
	if(this->nombreObjeto=="Sintax error." ){	
		cr->set_source_rgba(0.5, 0, 0,0.5);
		cr -> rectangle(this->posX, this -> posY,this->width, this->height);
		cr-> fill();
	}

	for (int i=0; i < slots.size(); ++i){
		slots[i] -> draw(cr);
	}

	Pango::FontDescription font;
	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_BOLD);
	auto layout = create_pango_layout(this->nombreParaMostrar);

	int text_width;
	int text_height;

	layout->get_pixel_size(text_width, text_height);
	
	cr->move_to((this->posX)+(text_width/2), (this->posY)+(text_height/2));

	layout->show_in_cairo_context(cr);
}

Slot* Morph::obtenerSlot(int posX,int posY){     
	for (int i=0; i < slots.size(); ++i){         
		if (slots[i]->clikEnObtenerSlot(posX, posY)) {   
				return slots[i];     
		}     
	}     
	return nullptr; 
}

Slot* Morph::obtenerSlotConId(int id_slot){     
	for (int i=0; i < slots.size(); ++i){         
		// guardar cuando agregue poli         
		if (slots[i]->tieneEsteId(id_slot)){   
			return slots[i];    
		}      
	}     
	return nullptr; 
}

std::vector<Slot*> Morph::obtenerSlotsConId(int id_slot){     
	std::vector<Slot*> listaSlots;
	for (int i=0; i < slots.size(); ++i){         
		// guardar cuando agregue poli         
		if (slots[i]->tieneEsteId(id_slot)){   
			listaSlots.push_back(slots[i]);    
		}      
	}     
	return listaSlots; 
}

Slot* Morph::obtenerSlotConEsteNombre(std::string nombre){     
	for (int i=0; i < slots.size(); ++i){         
		// guardar cuando agregue poli         
		if (slots[i]->tieneEsteNombre(nombre)){   
			return slots[i];    
		}      
	}     
	return nullptr; 
}


bool Morph::tieneElMismoIdQueEsteSlot(Slot* unSlot){
	return (unSlot->tieneEsteId(this->id));
}

void Morph::mostrarDescripcionMorph(){
	if (m_TextView && refTextViewConsola){
		m_TextView -> set_buffer(refTextViewConsola);
	}
	if (textViewCodigoAsociado && refTextViewCodigoAsociado){
		textViewCodigoAsociado->set_buffer(refTextViewCodigoAsociado);
	}
}

void Morph::actualizar_posicion(double x, double y){
	
	// para no dibujar fuera de la ventana
	/*if (x + width > 400 || y + height > 800){
		if (x + width > 400 ){
			this->posX = 400;
		} 
		if (y + height > 800 ){
			this -> posY = 800;
		}
		return;
	}
	if (x < 0 || y < 0){
		if (x < 0){
			this->posX = 0;
		}
		if (y < 0){
			this -> posY = 0;
		}
		return;
	}*/
	for (int i=0; i < slots.size(); ++i){
		slots[i] -> posX = x;
		slots[i] -> posY = y + (slots[i]->posY - this->posY);
	}
	for (int i=0; i < referencias.size(); ++i){
		referencias[i]->posX=x-8;
		referencias[i]->posY=y;
	}
	this->posX = x;
	this->posY = y;		
}

void Morph::agregarReferencia(Referencia* unaReferencia){
	this->referencias.push_back(unaReferencia);
}

// borro la referencia de la lista.
void Morph::borrarEstaReferencia(Referencia* referencia){
	for (std::vector<Referencia*>::iterator it = referencias.begin(); it != referencias.end();){  
		if(*it == referencia){	
			delete* it;  
			it = referencias.erase(it);
		} else {
			++it;
		}
	}	
}

Morph::~Morph(){
	for (int i=0; i < slots.size(); ++i){
		delete slots[i];
	}
	for (int i=0; i < referencias.size(); ++i){
		delete referencias[i];
	}
}
