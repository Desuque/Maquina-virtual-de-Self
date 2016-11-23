#include "server_mult.h"
#include "server_virtual_machine.h"

Mult::Mult(int id):Object(id){}

Slot* Mult::execute(VM& vm, p_objects& args){
        return args[0] -> mult(vm, args[1]);
}
