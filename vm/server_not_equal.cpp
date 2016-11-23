#include "server_not_equal.h"
#include "server_virtual_machine.h"

NotEqual::NotEqual(int id):Object(id){}

Slot* NotEqual::execute(VM& vm, p_objects& args){
        return args[0] -> nequal(vm, args[1]);
}
