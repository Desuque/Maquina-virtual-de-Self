#include "server_div.h"
#include "server_virtual_machine.h"

Div::Div(int id):Object(id){}

Slot* Div::execute(VM& vm, p_objects& args){
        return args[0] -> div(vm, args[1]);
}
