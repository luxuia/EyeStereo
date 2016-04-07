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
	class Vector3f;
	class RandomDot {
	public:
		RandomDot(int maxbound);
		~RandomDot();

		/**
			@param[in] pDevice 设备资源
			@param[in] x x轴边界
			@param[in] y y轴边界
			@param[in] cnt 随机点数目
			@param[in] gout 立体图
			初始化随机点图
		*/
		bool init(ID3D10Device *pDevice, float x, float y, int cnt, Geometry* gout);
		void draw();

		/**
			根据 Geometry::distanceShiftDot() 的实现确定点是否需要偏移量
		*/
		void dotShiftNumber(Vertex** vertices);

		/**
			@param[out] vertices 生成的随机点，vertices[0]是左眼，vertices[1]是右眼
			@param[in] x x边界
			@param[in] y y边界
			产生随机点
 		*/
		void makeRandomDot(Vertex** vertices, float x, float y);
		void makeRandomDotTest(Vertex** vertices, float x, float y);

		/** 被放弃 */
		bool inGeometry(Vertex &p, Geometry *g);
		/** 被放弃 */
		bool inGeometry(float x, float y, float z, Geometry *g);

		///> 更新随机点位置
		bool updateVertex(float ftime);

		///> 更新顶点缓冲区
		bool updateBuffer();

		///> 判断是否出了边界
		bool outBound(Vector3f, Vector3f&, int );

		ID3D10Device*	pd3dDevice;
		ID3D10Buffer*	vBuffer[2];

		//vector< Vector3 > randomForce;
		bool*			binGeometry;
		Vector3f*		pointStat;
		Vertex*			vertices[2];
		Vector3f*		velocity;
		Vector3f*		velocityL;
		Geometry*		pGeo;
		int iVertexNum;


		float boundX, boundY;
		float dotDist;
		float pointSize ;
		int maxPointNum;

	};

}



#endif //__RANDOMDOT__