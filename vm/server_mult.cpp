#include "server_mult.h"
#include "server_virtual_machine.h"

Slot* Mult::execute(VM& vm, p_objects& args){
	int res = args[0] -> as_int() *  args[1] -> as_int();
	Slot* result = vm.create_int(res);
	return result;
}
