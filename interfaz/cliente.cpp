#include "cliente.h"
#include "client_json_reader.h"
#include "server_json_writer.h"
#include "dialogoSeleccionLobby.h"

#define ERROR 0
#define TAM_COD 1
#define PEDIR_SLOT 2
#define AGREGAR_SLOT 3
#define BORRAR_SLOT 4
#define GENERIC 5
#define ACTUALIZAR_VISTA 8
#define GET_IT 16
#define DO_IT 17
#define PEDIR_MORPH 11
#define BORRAR_MORPH 12
#define GARBAGE 14
#define PEDIR_LISTA_LOBBYS 15

typedef  std::vector<int> v_ints;

Cliente::Cliente(ProxyServer& proxy, MyArea* myArea) : 
		proxy(proxy), myArea(myArea) {}
		
void Cliente::run() {
	uint32_t codigo = 0;
	while (true){
		try {
			codigo = proxy.recibirCodigoMensaje();
		} catch (const std::exception &e){
			// se cerro la conexion.
			return;
		}
		//std::cout << "codigo recibido: " << codigo << std::endl;
		switch (codigo){	
			case PEDIR_SLOT: {
				std::string json = proxy.recibirJson();
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
			case AGREGAR_SLOT : {
				std::string json = proxy.recibirJson();
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
		    case BORRAR_SLOT : {
				std::string json = proxy.recibirJson();
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
			case GENERIC : { 
				int accion = proxy.recibirCodigoMensaje();
				std::string json = proxy.recibirJson();
				std::vector<InterfaceSlot*> i_slots;
				JsonReader slots_reader;
				std::cout << i_slots.size() << std::endl;
				slots_reader.read(i_slots, json);
				if(accion == GET_IT){
					myArea->crearMorphs(i_slots);
				} else {
					if (accion == DO_IT) {
						if (i_slots.size())
							std::cout << i_slots[0]->get_value() << std::endl;
					}
				}
				for (std::vector<InterfaceSlot*>::iterator it = i_slots.begin(); it != i_slots.end();){  
			    	delete* it;  
			    	it = i_slots.erase(it);
			  	}
				break;
			}
			case GARBAGE: {
				JsonReader reader;
				v_ints vec;
				std::string json = proxy.recibirJson();
				reader.read_garbage_ids(json, vec);
				myArea->recolectarMorphs(vec);
				break;
			}
			case PEDIR_MORPH: {
				std::string json = proxy.recibirJson();
				std::vector<InterfaceSlot*> i_slots;
				JsonReader ids_reader;
				int id_morph = -1;
				std::string nombre_slot;
				ids_reader.read_id_morph_nombre_slot(json, id_morph, nombre_slot);
				myArea->mostrarEsteSlotComoMorph(id_morph, nombre_slot);
				break;
			}
			case BORRAR_MORPH : {	
				std::string json = proxy.recibirJson();	
				JsonReader jsonReader;
				int id = -1;
				jsonReader.read_id_morph(json, id);
				std::cout << id << std::endl;
				myArea->closeMorph(id);
				break;
			}					
			case ACTUALIZAR_VISTA : {
				std::string json = proxy.recibirJson();
				JsonReader jsonReader;
				int id = -1;
				double posX = 0, posY = 0;
				jsonReader.read_position(json, id, posX, posY);
				myArea->actualizarPosicionAMoprh(id, posX, posY);
				break;
			}
			case ERROR :{
				std::string json = proxy.recibirJson();
				std::vector<InterfaceSlot*> i_slots;
				JsonReader slots_reader;
				std::cout << i_slots.size() << std::endl;
				slots_reader.read(i_slots, json);
				myArea->crearMorphsError(i_slots);
				for (std::vector<InterfaceSlot*>::iterator it = i_slots.begin(); it != i_slots.end();){  
			    	delete* it;  
			    	it = i_slots.erase(it);
			  	}				break;
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