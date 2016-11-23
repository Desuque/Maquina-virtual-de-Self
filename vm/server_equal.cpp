#include "server_equal.h"
#include "server_virtual_machine.h"

Equal::Equal(int id):Object(id){}

Slot* Equal::execute(VM& vm, p_objects& args){
        return args[0] -> equal(vm, args[1]);
}
