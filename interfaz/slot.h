#ifndef SLOT_H
#define SLOT_H
#include "morph.h"
#include "string.h"

#define ALTURA_SLOT 18

class Slot : public Morph{
private:
	bool dibujadoComoMorph;
	/* interface Slot */
	bool type;
	bool code;
	std::string name;
	std::string value;
	int id;
	int id_base;

public:	
	Referencia* referencia;

public:	
	//Slot(){}
	Slot(InterfaceSlot* interfaceSlot, double posX, double posY, int width, int height);
	void actualizarSlot(InterfaceSlot* interface_slot);
	std::string get_name();
	int getWidth();
	int getHeight();
	bool tieneEsteId(int id);
	bool tieneEsteNombre(std::string nombre);
	int get_id();
	bool has_code();
	std::string get_value();
	bool estaDibujadoComoMorph();
	void setEstaDibujadoComoMorph(bool estaDibujado);
	std::string get_id_to_string();
	bool elValorEsPrimitivo();
	void draw(const Cairo::RefPtr<Cairo::Context>& cr);
	void setReferencia(Referencia* unaReferencia);
	bool clikEnObtenerSlot(int posX,int posY);
	size_t get_height();

	virtual ~Slot(){}
};

#endif