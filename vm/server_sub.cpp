#include "server_sub.h"
#include "server_virtual_machine.h"

Sub::Sub(int id):Object(id){}

Slot* Sub::execute(VM& vm, p_objects& args){
        return args[0] -> sub(vm, args[1]);
}
