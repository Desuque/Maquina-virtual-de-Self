#ifndef Referencia_H
#define Referencia_H
#include "morph.h"
#include "slot.h"
class Referencia : public Morph {
private:
	Morph* apuntoAMorph;
	Slot* perteneceASlot;

public:	
	Referencia(Morph* apuntoAMorph, Slot* perteneceASlot) {
		// getters de morph agregados por que el compilador
		// no me deja acceder a los atributos "protected" de Morph.
		this->posX = apuntoAMorph->getPosX()-8;
		this->posY = apuntoAMorph->getPosY();
		this->width = 8;
		this->height = 8;
		this->apuntoAMorph = apuntoAMorph;
		this->perteneceASlot = perteneceASlot;
	}
	//solo para comparar PREGUNTAR
	Referencia(int posX, int posY) : Morph(posX,posY) {}

	void draw(const Cairo::RefPtr<Cairo::Context>& cr){
		cr->set_line_width(0.5);
		cr->set_source_rgb(1.0, 0.0, 0.0);

		//std::cout << apuntoAMorph->nombreObjeto << std::endl;
		cr->move_to(perteneceASlot->getPosX() + perteneceASlot->getWidth() - 6, 
						perteneceASlot->getPosY() + (perteneceASlot->getHeight()/2));
		
		cr -> line_to(this->posX,this->posY+4);

		cr -> rectangle(this->posX, this -> posY,this->width, this->height);
		
		cr-> stroke();
	}

	void setSlotEstaDibujadoComoMorph(bool flag){
		this->perteneceASlot->setEstaDibujadoComoMorph(flag);
	}

	void apuntaAEsteMorph(Morph* apuntar){
		this->apuntoAMorph = apuntar;
	}

	void borrarReferenciaAnterior(){
		this->apuntoAMorph->borrarReferencia(this);
	}
	
	void actualizar_posicion(double x, double y){
		this->posX = x;
		this->posY = y;	
	}

	virtual ~Referencia(){}
};

#endif