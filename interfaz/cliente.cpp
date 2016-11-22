#include "cliente.h"

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
		std::cout << "codigo respuesta: " << codigo << std::endl;
		switch (codigo){	
			case 2: {
				uint32_t tamMensaje = proxy.recibirCodigo(4);
				std::string json = proxy.recibir(tamMensaje);
				std::cout << json << std::endl;
				std::vector<InterfaceSlot*> i_slots;
				JsonReader slots_reader;
				slots_reader.read(i_slots, json);
				myArea->agregarSlots(i_slots);
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
				break;
		    }
		    case 4: {
				uint32_t tamMensaje = proxy.recibirCodigo(4);
				std::string json = proxy.recibir(tamMensaje);
				std::cout << json << std::endl;
				std::vector<InterfaceSlot*> i_slots;
				JsonReader slots_reader;
				slots_reader.read(i_slots, json);
				myArea->borrarSlots(i_slots);
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