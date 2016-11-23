#include "server_int.h"
#include <iostream>
#include "server_virtual_machine.h"

Int::Int(int id, int value):Object(id){
	this -> value = value;
}

int Int::as_int(){
	return this -> value;
}

Slot* Int::print(VM& vm){
	std::cout << this -> value << std::endl;
	return vm.create_int(this -> value);
}

string Int::as_string(){
	return std::to_string(this -> value);
}

Slot* Int::clone(VM& vm){
	Slot* new_sl = vm.create_int(this -> value);
	return new_sl;
}

void Int::get_self_slots(string& slots_self){
	slots_self += " " + as_string() + " .";
}

Slot* Int::add(VM& vm, Object* op){
        int res = value +  op -> as_int();
	Slot* result = vm.create_int(res);
        return result;
}

Slot* Int::sub(VM& vm, Object* op){
        int res = value -  op -> as_int();
	Slot* result = vm.create_int(res);
        return result;
}

Slot* Int::mult(VM& vm, Object* op){
        int res = value *  op -> as_int();
	Slot* result = vm.create_int(res);
        return result;
}

Slot* Int::div(VM& vm, Object* op){
        int b = op -> as_int();
        if (b == 0)
                throw "NO SE PUEDE DIVIDIR POR 0";
        float res = (float)value / (float)b;
	Slot* result = vm.create_float(res);
        return result;
}

Slot* Int::equal(VM& vm, Object* op){
        bool res = (value ==  op -> as_int());
	Slot* result = vm.create_boolean(res);
	return result;
}

Slot* Int::nequal(VM& vm, Object* op){
        bool res = (value !=  op -> as_int());
	Slot* result = vm.create_boolean(res);
	return result;
}

float Int::as_float(){
        return value;
}
