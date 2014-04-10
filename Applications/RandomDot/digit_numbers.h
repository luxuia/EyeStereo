#ifndef __DIGITAL_NUMBER__
#define __DIGITAL_NUMBER__


#include "Utility/PCH.h"
#include "Geometry/vertex.h"
#include "Geometry/Geometry.h"

#pragma once

namespace EyeStereo {
	static const int digitnum[] = {16, 4, 20, 16, 12, 20, 20, 8, 20, 20};
	class digit_numbers: public Geometry{

	public:
		digit_numbers();
		~digit_numbers();
		void random_number();
		void Scale(float scale);
		void Pos(float x, float y);
		virtual float distanceShiftDot(float x, float y);
		virtual void random();
		virtual int getType();
		void generate();

		int ChoosedNumber;
		Vertex*		ver[10];
		int randomList[10]; 
	
		
	};
}
#endif // __DIGITAL_NUMBER__