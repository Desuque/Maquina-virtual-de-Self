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
				std::cout << "recibi actualizacion" << std::endl;
				uint32_t tamMensaje = proxy.recibirCodigo(4);
				std::string json = proxy.recibir(tamMensaje);
				std::cout << json << std::endl;
				
				JsonReader jsonReader;
				int id;
				double posX, posY;
				jsonReader.read_position(json, id, posX, posY);
				std::cout << posX << std::endl;
				std::cout << posY << std::endl;

				myArea->actualizarPosicionAMoprh(id,posX,posY);

				//uint32_t posX = proxy.recibirCodigo(TAM_COD);
				//uint32_t posY = proxy.recibirCodigo(TAM_COD);
				//std::cout << "recibi posX: " << (posX) << std::endl;
				//std::cout << "recibi posY: " <<(posY )  << std::endl;
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