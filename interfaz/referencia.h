#ifndef Referencia_H
#define Referencia_H
#include "morph.h"
#include "slot.h"
class Referencia : public Morph{
public:
	Morph* apuntoAMorph;
	Slot* perteneceASlot;
	/*int posX;
	int posY;*/
	/*int width;
	int height;*/

public:	
	Referencia(Morph* apuntoAMorph, Slot* perteneceASlot){
		this->apuntoAMorph = apuntoAMorph;
		this->perteneceASlot = perteneceASlot;
		this->posX = apuntoAMorph->posX-8;
		this->posY = apuntoAMorph->posY;
		this->width = 8;
		this->height = 8;
	}
	//solo para comparar PREGUNTAR
	Referencia(int posX, int posY) : Morph(posX,posY) {

	}

	void draw(const Cairo::RefPtr<Cairo::Context>& cr){
		cr->set_line_width(0.5);
		cr->set_source_rgb(1.0, 0.0, 0.0);

		//std::cout << apuntoAMorph->nombreObjeto << std::endl;
		cr->move_to(perteneceASlot->posX + perteneceASlot->width - 6, perteneceASlot->posY + (perteneceASlot->height/2));
		cr -> line_to(this->posX,this->posY+4);

		cr -> rectangle(this->posX, this -> posY,this->width, this->height);
		
		cr-> stroke();
	}

	void apuntaAEsteMorph(Morph* apuntar){
		this->apuntoAMorph = apuntar;
	}

	void borrarReferenciaAnterior(){
		//std::vector<Referencia*> refs = this->apuntoAMorph->referencias;
		this->apuntoAMorph->borrarReferencia(this);
	}

	//PREGUNTAR POR QUE NO ANDA EL OPERADOR DE MORPH
	/*bool operator==(const Referencia& rhs) const {
		if ((rhs.posX >= this->posX) && (rhs.posY >= this->posY)
			&& (rhs.posX <= ((this->posX) + this->width)) 
			&& (rhs.posY <= ((this->posY) + this->height))){
			return true;
		}
		return false;
	}

	bool operator!=(const Referencia& rhs) const { 
		return !(*this==rhs);
	}*/
	
	void actualizar_posicion(double x, double y){
		this->posX = x;
		this->posY = y;	
	}

	virtual ~Referencia(){}
};

#endif