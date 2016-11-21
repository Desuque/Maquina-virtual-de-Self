#include "server_garbage_slot.h"
#include "server_json_writer.h"

GarbageSlot::GarbageSlot(int id, string name, v_ids& ids):Slot(id, name){
        this -> ids = std::move(ids);
}

string GarbageSlot::json(int id_base){
        JsonWriter writer;
        return writer.write_garbage_ids(ids);
}
