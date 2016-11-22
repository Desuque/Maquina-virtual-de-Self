#include "cliente.h"
#include "client_json_reader.h"
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
				uint32_t tamMensaje = proxy.recibirCodigo(4);
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
				double posX, posY;
				jsonReader.read_position(json, id, posX, posY);
				myArea->actualizarPosicionAMoprh(id, posX, posY);
				break;
			}
			default: {
				std::cout << "recibi comando desconocido\n";
				break;
			}
		}
	}		
}

Cliente::~Cliente(){}