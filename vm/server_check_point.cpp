#include "server_check_point.h"

CheckPoint::CheckPoint(int id, string name):Slot(id, name){}

bool CheckPoint::is_check(){
	return true;
}
