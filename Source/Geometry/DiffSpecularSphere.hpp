//--------------------------------------------------------------------------------
// This file is a portion of the Eye Stereo Project.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Luxuia
//---------------------------------------------------------------------------------


#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Utility\PCH.h"
struct Vertex14;

namespace EyeStereo {
	class DiffSpecularSphere {
	public:
		

		float mRadius;
		UINT  mNumSlices;
		UINT  mNumStacks;

		DWORD mNumVertices;
		DWORD mNumFaces;

		ID3D10Device* md3dDevice;
		ID3D10Buffer* mVB;
		ID3D10Buffer* mIB;
		ID3DX10Mesh*  mMesh;

		std::vector<Vertex14> vertices;
		std::vector<DWORD> indices;

		D3DXCOLOR mDiffColor;

		DiffSpecularSphere():mDiffColor(GREEN), md3dDevice(NULL), mVB(NULL), mIB(NULL), mMesh(NULL) {
		}

		~DiffSpecularSphere();


		void setColor(D3DXCOLOR color);

		void init(ID3D10Device* device, float radius, UINT numSlices, UINT numStacks);

		void draw();

		void buildStacks(std::vector<Vertex14> & vertices, std::vector<DWORD> & indices);

		bool CreateMesh(const D3D10_INPUT_ELEMENT_DESC* layout);

		ID3DX10Mesh* GetD3D10Mesh();

	};

}

#endif// __SPHERE_H__