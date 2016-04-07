#ifndef __GEOMETRY__
#define __GEOMETRY__

#include "Utility/PCH.h"
#include "Mathematic/Vector2f.h"

namespace EyeStereo{

	class Geometry {
	public:
		virtual float distanceShiftDot(float x, float y) = 0;
		virtual void Select() = 0;
		virtual int getType() = 0;

		// Or, should it be write as depth buffer
		bool InGeometry(Vector2f &A, Vector2f &B, Vector2f &C, Vector2f &P) {
			Vector2f v0 = C - A;
			Vector2f v1 = B - A;
			Vector2f v2 = P - A;

			float dot00 = v0.Dot(v0);
			float dot01 = v0.Dot(v1);
			float dot02 = v0.Dot(v2);
			float dot11 = v1.Dot(v1);
			float dot12 = v1.Dot(v2);

			float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);

			float u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
			if (u < 0 || u > 1) // if u out of range, return directly
			{
				return false;
			}

			float v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
			if (v < 0 || v > 1) // if v out of range, return directly
			{
				return false;
			}

			if (u + v <= 1) {
				return true;
			}

		}

	};

}

#endif //__GEOMERTY__