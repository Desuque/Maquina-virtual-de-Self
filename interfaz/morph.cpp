#include "morph.h"
#include "slot.h"
#include "referencia.h"
#include <gtkmm.h>
#include <pangomm/layout.h>

Morph::Morph(double posX, double posY) : m_TextView(nullptr), refTextViewConsola(nullptr) {
	this->posX = posX;
	this->posY = posY;
	this -> nombreObjeto = "que?";
	//this->referencia=nullptr;
}

Morph::Morph(double posX, double posY, int width, int height,Gtk::TextView* m_TextView){
	this->posX = posX;
	this->posY = posY;
	this->width = width;
	this->height = height;
	this -> nombreObjeto = "";
	this->m_TextView=m_TextView;
	refTextViewConsola = Gtk::TextBuffer::create();
	refTextViewConsola->set_text("");
	//this->referencia=nullptr;
}

Morph::Morph(std::string nombreObjeto, double posX, double posY) : 
	nombreObjeto(nombreObjeto), posX(posX), posY(posY), /*referencia(nullptr),*/ m_TextView(nullptr) {
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

}

// SE USA ESTE SIEMPRE VER LOS DEMAS ELIMINAR
Morph::Morph(std::string nombreObjeto, int id, double posX, double posY, 
		Gtk::TextView* m_TextView, Gtk::TextView* codigoAsociado) : 
		nombreObjeto(nombreObjeto), posX(posX), 
		posY(posY), m_TextView(m_TextView),
		textViewCodigoAsociado(codigoAsociado)/*,
		referencia(nullptr)*/ {
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
	//referencia = new Morph(posX-8,posY,8,8,m_TextView);
}

Morph::Morph(std::string nombreObjeto, double posX, double posY, int width, int height) 
	: m_TextView(nullptr) {
	Pango::FontDescription font;
	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_LIGHT);
	// http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
	auto layout = create_pango_layout(this->nombreObjeto);
	layout->set_font_description(font);
	int text_width;
	int text_height;
	layout->get_pixel_size(text_width, text_height);
	this->height = text_height;
	this->posX = posX;
	this->posY = posY;
	this->width = width;
	this->nombreObjeto = nombreObjeto;
	refTextViewConsola = Gtk::TextBuffer::create();
	refTextViewConsola->set_text("");
	//this->referencia=nullptr;
	//referencia = new Morph(posX-8,posY,8,8,m_TextView);
}

Morph::Morph(){
}

Morph* Morph::get_it(){
	std::string objeto;
	std::string mensaje;
	if (!m_TextView) std::cout << "error" << std::endl;
	refTextViewConsola = m_TextView -> get_buffer();
	std::string str = m_TextView->get_buffer()->get_text();
	std::istringstream iss(str);
	iss >> std::noskipws;
	iss >> objeto  >> mensaje;  
	std::cout << objeto << std::endl;
	//Morph* morph = nullptr;
	if (str != ""){
		//morph = new Morph("que?",200,200,m_TextView);
		this->nombreObjeto=objeto;
		
	}/* else {
		morph = new Morph(objeto,200,200,m_TextView);
	}*/

	// ENVIAR A SERVIDOR
	std::cout << this->nombreObjeto << " " << str
			  << std::endl;

	return this;
}

void Morph::do_it(){
	std::string objeto;
	std::string mensaje;
	if (!m_TextView) std::cout << "error" << std::endl;
	//refTextViewConsola = m_TextView -> get_buffer();
	std::string str = m_TextView->get_buffer()->get_text();
	std::cout << str << std::endl;
	std::istringstream iss(str);
	iss >> std::noskipws;
	iss >> objeto  >> mensaje;  
	std::cout << objeto << std::endl;
	// ENVIAR A SERVIDOR
	std::cout << this->nombreObjeto << " " << str
			  << std::endl;
}



void Morph::agregarSlot(std::string nombreSlot){
	//Morph* morph = new Morph(nombreSlot, this->posX, (this->posY)+(this->height), this->width, this->height);
	Slot* slot = new Slot(nombreSlot, this->posX, (this->posY)+(this->height), this->width, this->height);
	//this->height+=morph->height;
	this->height+=slot->height;
	slots.push_back(slot);
	//slots.push_back(morph);
}

void Morph::actualizarAlturaMorph(size_t alturaDeSlot){
	this->height += alturaDeSlot;
}


void Morph::agregarSlot(InterfaceSlot* interface_slot){
	Slot* slot = new Slot(interface_slot,this->posX, (this->posY)+(this->height), this->width, this->height);
	this->actualizarAlturaMorph(slot->get_height());
	slots.push_back(slot);
}

void Morph::draw(const Cairo::RefPtr<Cairo::Context>& cr){

	cr->set_line_width(2.5);
	cr->set_source_rgb(0.0, 0.0, 0.0);
	
	cr -> rectangle(this->posX, this -> posY,this->width, this->height);
	//cr ->fill();
	cr-> stroke();

	for (int i=0; i < slots.size(); ++i){
		slots[i] -> draw(cr);
	}

	Pango::FontDescription font;
	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_BOLD);
	auto layout = create_pango_layout(this->nombreObjeto);

	int text_width;
	int text_height;

	layout->get_pixel_size(text_width, text_height);
	
	cr->move_to((this->posX)+(text_width/2), (this->posY)+(text_height/2));

	layout->show_in_cairo_context(cr);
}

Morph* Morph::clikEnObtenerSlot(int posX,int posY){     
	for (int i=0; i < slots.size(); ++i){       
		if (slots[i]->clikEnObtenerSlot(posX,posY)
			 && !(slots[i]->estaDibujadoComoMorph())) {   
				slots[i]->setEstaDibujadoComoMorph(true);
				Morph* morph = nullptr;
				if(slots[i]->code){
					morph = new Morph(slots[i]->name,slots[i]->id,slots[i]->posX + slots[i]->width +20, slots[i]->posY+4,m_TextView,textViewCodigoAsociado);             
					morph -> refTextViewCodigoAsociado->set_text(slots[i]->value);
				} else {
					morph = new Morph(slots[i]->value,slots[i]->id,slots[i]->posX + slots[i]->width +20, slots[i]->posY+4,m_TextView,textViewCodigoAsociado);             
				}
				//Morph* morph = new (slots[i]->value,slots[i]->posX + slots[i]->width +20, slots[i]->posY+4,m_TextView);             
				//morph -> referencia = new Morph(morph->posX-8,morph->posY,8,8,m_TextView);                   
				//morph -> referencia = new Referencia(morph, slots[i]);
				morph->referencias.push_back(new Referencia(morph, slots[i]));
				//slots[i] -> referencia = morph->referencia;
				//morph -> referencia -> slotPadre = morph;
				//slots[i] -> referencia -> slotPadre = morph;
				return morph;         
			}     
		}     
	return nullptr; 
}

Slot* Morph::obtenerSlot(int posX,int posY){     
	for (int i=0; i < slots.size(); ++i){         
		// guardar cuando agregue poli         
		if ((posX >= (slots[i]->posX) + (slots[i]->width) -10) && (posY >= (slots[i]->posY) + 2)
			&& (posX <= (((slots[i]->posX) + (slots[i]->width))-2)) && (posY <=
			((slots[i]->posY)+slots[i]->height-2))) {   
				return slots[i];     
			}     
		}     
	return nullptr; 
}

Morph* Morph::clickEnReferenciaAMorph(int posX,int posY){
		/*if ((posX >= this->posX) && (posY >= this->posY)
	    		&& (posX <= ((this->posX)+8)) && (posY <= ((this->posY)+8)))
		{
			std::cout << " referencia a morph" << std::endl;		
      		//Morph* morph = new Morph("",this->posX-8,this->posY,m_TextView);
      		referencia=nullptr;
      		return referencia;
      	}*/
	return nullptr;
}

void Morph::mostrarDescripcionMorph(){
	//Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer1;
	//m_refTextBuffer1 = Gtk::TextBuffer::create();
	//refTextViewConsola->set_text("");
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
	/*if(referencia != nullptr){
		referencia->posX=x-8;
		referencia->posY=y;
	}*/
	this->posX = x;
	this->posY = y;		
}



Morph::~Morph(){}
