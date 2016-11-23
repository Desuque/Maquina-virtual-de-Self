#include "server_sum.h"
#include "server_virtual_machine.h"

Sum::Sum(int id):Object(id){}

Slot* Sum::execute(VM& vm, p_objects& args){
        return args[0] -> add(vm, args[1]);
}
