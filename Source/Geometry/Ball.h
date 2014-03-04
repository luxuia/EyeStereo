#ifndef __BALL_H__
#define __BALL_H__

#include "Sphere.hpp"


namespace EyeStereo{
	class Ball : public Sphere {
	public:
		D3DXVECTOR3 pos;
		float r;

		D3DXVECTOR3 v;
		Ball() {
			pos.x = pos.y = pos.z = 0;
		}
	};
}


#endif //__BALL_H__