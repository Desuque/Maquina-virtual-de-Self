#include "server_float.h"
#include <iostream>
#include "server_virtual_machine.h"

Float::Float(int id, float value):Object(id){
	this -> value = value;
}

int Float::as_int(){
	return this -> value;
}

Slot* Float::print(VM& vm){
	std::cout << this -> value << std::endl;
	return vm.create_float(this -> value);
}

string Float::as_string(){
	return std::to_string(this -> value);
}

Slot* Float::clone(VM& vm){
	Slot* new_sl = vm.create_float(this -> value);
	return new_sl;
}

void Float::get_self_slots(string& slots_self){
	slots_self += " "+ as_string() + " .";
}

Slot* Float::add(VM& vm, Object* op){
        float res = value +  op -> as_float();
        std::cout << "FLOTANTE " << res << std::endl;
	Slot* result = vm.create_float(res);
        return result;
}

Slot* Float::sub(VM& vm, Object* op){
        float res = value -  op -> as_float();
	std::cout << "FLOTANTE " << res << std::endl;
        Slot* result = vm.create_float(res);
        return result;
}

Slot* Float::mult(VM& vm, Object* op){
        float res = value *  op -> as_float();
        std::cout << "FLOTANTE " << res << std::endl;
	Slot* result = vm.create_float(res);
        return result;
}

Slot* Float::div(VM& vm, Object* op){
        float b = op -> as_float();
        std::cout << "FLOTANTE "  << std::endl;
        if (b == 0)
                throw "NO SE PUEDE DIVIDIR POR 0";
        
        float res = value /  b;
	Slot* result = vm.create_float(res);
        return result;
}

Slot* Float::equal(VM& vm, Object* op){
        bool res = (value ==  op -> as_float());
	Slot* result = vm.create_boolean(res);
	return result;
}

Slot* Float::nequal(VM& vm, Object* op){
        bool res = (value !=  op -> as_int());
	Slot* result = vm.create_boolean(res);
	return result;
}

float Float::as_float(){
        return value;
}
