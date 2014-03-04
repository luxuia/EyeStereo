//--------------------------------------------------------------------------------
// This file is a portion of the Eye Stereo Project.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Luxuia
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// PCH.h
//
// Used for generated a precompiled header file
//--------------------------------------------------------------------------------
#ifndef PCH_h
#define PCH_h
//--------------------------------------------------------------------------------

#ifndef STRICT
#define STRICT
#endif

// Standard C++ Library includes
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>
#include <exception>
#include <iostream>
#include <iomanip>

// Standard C Library includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory>
#include <assert.h>
#include <time.h>


// DirectX includes and DXUT include
#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTmisc.h"
#include "DXUTCamera.h"
#include "DXUTSettingsDlg.h"
#include "SDKmisc.h"
#include "SDKmesh.h"
//#include "resource.h"
//#include <d3d11_1.h>
//#include <d3dCompiler.h>

#pragma comment(lib, "d3dx10d.lib")
#pragma comment(lib, "d3dx9d.lib")
#pragma comment(lib, "dxerr.lib")

//#include <wrl.h>

#define SAFE_RELEASE( x ) {if(x){(x)->Release();(x)=NULL;}}
#define SAFE_DELETE( x ) {if(x){delete (x);(x)=NULL;}}
#define SAFE_DELETE_ARRAY( x ) {if(x){delete[] (x);(x)=NULL;}}


#define EYE_PI 3.14159265f

// Returns random float in [0, 1).
D3DX10INLINE float RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}

// Returns random float in [a, b).
D3DX10INLINE float RandF(float a, float b)
{
	return a + RandF()*(b-a);
}

// Returns random vector on the unit sphere.
D3DX10INLINE D3DXVECTOR3 RandUnitVec3()
{
	D3DXVECTOR3 v(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f));
	D3DXVec3Normalize(&v, &v);
	return v;
}
// FLT_MAX = largest float number
const float Elapse	= 1e-5f;

const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const D3DXCOLOR YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR CYAN(0.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);

const D3DXCOLOR BEACH_SAND(1.0f, 0.96f, 0.62f, 1.0f);
const D3DXCOLOR LIGHT_YELLOW_GREEN(0.48f, 0.77f, 0.46f, 1.0f);
const D3DXCOLOR DARK_YELLOW_GREEN(0.1f, 0.48f, 0.19f, 1.0f);
const D3DXCOLOR DARKBROWN(0.45f, 0.39f, 0.34f, 1.0f);



inline void printMatrix(float* mat) {
	for (int i = 0; i<4; ++i) {
		for(int j = 0; j < 4; ++j) {
			printf("%0.2f\t", mat[i*4+j]);
		}
		puts("");
	}

}

inline float Vec3Len(D3DXVECTOR3 &a, D3DXVECTOR3 &b) {
	return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) + (a.z - b.z)*(a.z - b.z);
}

inline void PrintMatrixDEBUG(D3DXMATRIX &mat) {
	printf("%0.2f %0.2f %0.2f %0.f\n"
		"%0.2f %0.2f %0.2f %0.f\n"
		"%0.2f %0.2f %0.2f %0.f\n"
		"%0.2f %0.2f %0.2f %0.f\n", 
		mat(0, 0), mat(0, 1), mat(0, 2), mat(0, 3)
		, mat(1, 0), mat(1, 1), mat(1, 2), mat(1, 3)
		, mat(2, 0), mat(2, 1), mat(2, 2), mat(2, 3)
		, mat(3, 0), mat(3, 1), mat(3, 2), mat(3, 3));
}



#endif // PCH_h