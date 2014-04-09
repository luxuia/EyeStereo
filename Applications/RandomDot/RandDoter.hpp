//--------------------------------------------------------------------------------
// This file is a portion of the Eye Stereo Project.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Luxuia
//---------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
// RandomDot
// Used for Generate Random Dot Buffer from Geometry
//--------------------------------------------------------------------------------

#ifndef __RANDOMDOT__
#define __RANDOMDOT__


#include "Utility/PCH.h"
#include "Geometry/vertex.h"
#include "Geometry/Geometry.h"



namespace EyeStereo {

	class RandomDot {

	public:
		RandomDot(int maxbound);
		~RandomDot();

		bool init(ID3D10Device *pDevice, float x, float y, int cnt, Geometry* gout);
		void draw();
		void insert_number(Vertex** vertices);
		void makeRandomDot(Vertex** vertices, float x, float y);
		void makeRandomDotTest(Vertex** vertices, float x, float y);
		bool inGeometry(Vertex &p, Geometry *g);
		bool inGeometry(float x, float y, float z, Geometry *g);
		bool updateVertex(float ftime);
		bool updateBuffer();
		bool outBound(Vector3, Vector3&, int );

		ID3D10Device*	pd3dDevice;
		ID3D10Buffer*	vBuffer[2];

		//vector< Vector3 > randomForce;
		bool*			binGeometry;
		Vector3*		pointStat;
		Vertex*			vertices[2];
		Vector3*		velocity;
		Vector3		    velocityL;
		Geometry*		pGeo;
		int iVertexNum;


		float boundX, boundY;
		float dotDist;
		float pointSize ;
		int maxPointNum;

	};

}



#endif //__RANDOMDOT__