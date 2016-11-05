#ifndef __boolean_h__
#define __boolean_h__

#include <vector>
#include "object.h"

class Boolean: public Object{
	private:
		bool value;
	public:
		Boolean(bool value);
		Slot* clone(VM& vm);
		Slot* print(VM& vm);
		string as_string();
};

#endif
