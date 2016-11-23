#include "cliente.h"
#include "client_json_reader.h"

#define TAM_COD 1
#define ACTUALIZAR_VISTA 8
#define GET_IT 9
#define DO_IT 10
#define PEDIR_MORPH 11

Cliente::Cliente(ProxyServer& proxy, MyArea* myArea) : proxy(proxy), myArea(myArea) {}
		
void Cliente::run() {
	uint32_t codigo = 0;
	while (true){
		try {
			codigo = proxy.recibirCodigo(TAM_COD);
		} catch (const std::exception &e){
			// error de conexion.
			return;
		}
		switch (codigo){	
			case 2: {
				std::cout << "recibi slot" << std::endl;
				uint32_t tamMensaje = proxy.recibirCodigo(4);
				std::cout << tamMensaje << std::endl;
				std::string json = proxy.recibir(tamMensaje);
				std::cout << json << std::endl;
				std::vector<InterfaceSlot*> i_slots;
				JsonReader slots_reader;
				slots_reader.read(i_slots, json);
				myArea->agregarSlots(i_slots);
				int size = i_slots.size();
			    for (std::vector<InterfaceSlot*>::iterator it = i_slots.begin(); it != i_slots.end();){  
			    	delete* it;  
			    	it = i_slots.erase(it);
			  	}
				break;
			}
			case 3: {
				// recibe el tamanio del mensaje, cambiar nombre
				uint32_t tamMensaje = proxy.recibirCodigo(4);
				std::string json = proxy.recibir(tamMensaje);
				std::cout << json << std::endl;
				std::vector<InterfaceSlot*> i_slots;
				JsonReader slots_reader;
				slots_reader.read(i_slots, json);
				myArea->agregarSlots(i_slots);
			    for (std::vector<InterfaceSlot*>::iterator it = i_slots.begin(); it != i_slots.end();){  
			    	delete* it;  
			    	it = i_slots.erase(it);
			  	}
				break;
		    }
		    case 4: {
				uint32_t tamMensaje = proxy.recibirCodigo(4);
				std::string json = proxy.recibir(tamMensaje);
				std::cout << json << std::endl;

				JsonReader slots_reader;
				std::vector<InterfaceSlot*> i_slots;
				slots_reader.read(i_slots, json);
				myArea->borrarSlots(i_slots);

			    for (std::vector<InterfaceSlot*>::iterator it = i_slots.begin(); it != i_slots.end();){  
			    	delete* it;  
			    	it = i_slots.erase(it);
			  	}
				break;
		    }
			case 5: { 
				uint32_t tamMensaje = proxy.recibirCodigo(4);
				std::cout << "tamanio mensaje: " << tamMensaje << std::endl;
				std::string json = proxy.recibir(tamMensaje);
				std::cout << json << std::endl;
				std::vector<InterfaceSlot*> i_slots;
				JsonReader slots_reader;
				std::cout << i_slots.size() << std::endl;
				slots_reader.read(i_slots, json);
				myArea->crearMorphs(i_slots);
				for (std::vector<InterfaceSlot*>::iterator it = i_slots.begin(); it != i_slots.end();){  
			    	delete* it;  
			    	it = i_slots.erase(it);
			  	}
				break;
			}
			case PEDIR_MORPH: {
				std::cout << "pedi morph" << std::endl;
				uint32_t tamMensaje = proxy.recibirCodigo(4);
				std::string json = proxy.recibir(tamMensaje);
				std::cout << json << std::endl;
				std::vector<InterfaceSlot*> i_slots;
				JsonReader ids_reader;
				int id_morph = -1, id_slot = -1;
				ids_reader.read_id_morph_id_slot(json, id_morph, id_slot);
				//myArea->agregarSlots(i_slots);
				myArea->mostrarEsteSlotComoMorph(id_morph, id_slot);
				break;
			}
			case GET_IT : {
				//....
				break;
			}					
			case DO_IT : {
				//....
				break;
			}					
			case ACTUALIZAR_VISTA : {
				//....
				uint32_t tamMensaje = proxy.recibirTamanioDeMensaje(4);
				std::string json = proxy.recibir(tamMensaje);	
				JsonReader jsonReader;
				int id = -1;
				double posX = 0, posY = 0;
				jsonReader.read_position(json, id, posX, posY);
				myArea->actualizarPosicionAMoprh(id, posX, posY);
				break;
			}
			default: {
				std::cout << "recibi comando desconocido\n";
				std::cout << "recibi comando desconocido: " << codigo << std::endl;
				break;
			}
		}
	}		
}

Cliente::~Cliente(){}