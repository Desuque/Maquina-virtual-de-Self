#include "not_equal.h"
#include "virtual_machine.h"

Slot* NotEqual::execute(VM& vm, p_objects& args){
	bool res = (args[0] -> as_int() !=  args[1] -> as_int());
	Slot* result = vm.create_boolean(res);
	return result;
}
