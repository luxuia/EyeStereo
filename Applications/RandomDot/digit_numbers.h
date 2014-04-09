#ifndef __DIGITAL_NUMBER__
#define __DIGITAL_NUMBER__


#include "Utility/PCH.h"
#include "Geometry/vertex.h"
#include "Geometry/Geometry.h"

#pragma once

const int digitnum[] = {16, 4, 20, 16, 12, 20, 20, 8, 20, 20};

namespace EyeStereo {

	class digit_numbers {

	public:
		digit_numbers();
		~digit_numbers();
		void random_number();

		int tmp;
		Vertex*		ver[10];
	};
}
#endif // __DIGITAL_NUMBER__