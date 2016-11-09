#include "server_equal.h"
#include "server_virtual_machine.h"

Equal::Equal(int id):Object(id){}

Slot* Equal::execute(VM& vm, p_objects& args){
	bool res = (args[0] -> as_int() ==  args[1] -> as_int());
	Slot* result = vm.create_boolean(res);
	return result;
}
