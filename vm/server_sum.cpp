#include "server_sum.h"
#include "server_virtual_machine.h"

Sum::Sum(int id):Object(id){}

Slot* Sum::execute(VM& vm, p_objects& args){
	int res = args[0] -> as_int() +  args[1] -> as_int();
	Slot* result = vm.create_int(res);
	return result;
}
