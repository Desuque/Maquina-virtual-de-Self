#ifndef Referencia_H
#define Referencia_H
#include "morph.h"

class Referencia : public Morph{
public:
	Morph* referencia;
	int posX;
	int posY;
	int width;
	int height;

public:	
	Referencia(Morph* referencia, int posX, int posY, int width, int height){
		this->referencia = referencia;
		this->posX = posX;
		this->posY = posY;
		this->width = width;
		this->height = height;
	}

	virtual ~Referencia(){}
};

#endif