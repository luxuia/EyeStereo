//--------------------------------------------------------------------------------
// This file is a portion of the Eye Stereo Project.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Luxuia
//---------------------------------------------------------------------------------


#include "DiffSpecularSphere.hpp"


using namespace EyeStereo;

DiffSpecularSphere::~DiffSpecularSphere() {

	SAFE_RELEASE(mVB);
	SAFE_RELEASE(mIB);
	SAFE_RELEASE(mMesh);
	indices.clear();
	vertices.clear();
}


void DiffSpecularSphere::setColor(D3DXCOLOR color) {
		mDiffColor = color;
	}

void DiffSpecularSphere::init(ID3D10Device* device, float radius, UINT numSlices, UINT numStacks)
	{

		md3dDevice = device;
		//md3dDevice->AddRef();
		mRadius    = radius;
		mNumSlices = numSlices;
		mNumStacks = numStacks;
		buildStacks(vertices, indices);

		mNumVertices = (UINT)vertices.size();
		mNumFaces    = (UINT)indices.size()/3;

		D3D10_BUFFER_DESC vbd;
		vbd.Usage = D3D10_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex14) * mNumVertices;
		vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		D3D10_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB);

		D3D10_BUFFER_DESC ibd;
		ibd.Usage = D3D10_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
		ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		D3D10_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB);

	}

void DiffSpecularSphere::draw() {
		UINT stride = sizeof(Vertex14);
		UINT offset = 0;
		md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
		md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
		md3dDevice->DrawIndexed(mNumFaces*3, 0, 0);
	}

void DiffSpecularSphere::buildStacks(std::vector<Vertex14> & vertices, std::vector<DWORD> & indices){
		float phiStep = EYE_PI/mNumStacks;

		// do not count the poles as rings
		UINT numRings = mNumStacks-1;

		// Compute vertices for each stack ring.
		for(UINT i = 1; i <= numRings; ++i)
		{
			float phi = i*phiStep;

			// vertices of ring
			float thetaStep = 2.0f*EYE_PI/mNumSlices;
			for(UINT j = 0; j <= mNumSlices; ++j)
			{
				float theta = j*thetaStep;

				Vertex14 v;

				// spherical to cartesian
				v.pos.x = mRadius*sinf(phi)*cosf(theta);
				v.pos.y = mRadius*cosf(phi);
				v.pos.z = mRadius*sinf(phi)*sinf(theta);

				D3DXVec3Normalize(&v.normal, &v.pos);

				v.normal = D3DXVECTOR3(v.pos);

				//v.texC.x = (float)i/numRings;
				//v.texC.y = (float)j/mNumSlices;

				v.diffuse = mDiffColor;
				v.spec = YELLOW;

				//v.texC.x = theta / (2.0f*PI);
				//v.texC.y = phi / PI;

				vertices.push_back( v );
			}
		}

		UINT northPoleIndex = (UINT)vertices.size();
		UINT southPoleIndex = (UINT)vertices.size();

		UINT numRingVertices = mNumSlices+1;

		// Compute indices for inner stacks (not connected to poles).
		for(UINT i = 0; i < mNumStacks-2; ++i)
		{
			for(UINT j = 0; j < mNumSlices; ++j)
			{
				indices.push_back(i*numRingVertices + j);
				indices.push_back(i*numRingVertices + j+1);
				indices.push_back((i+1)*numRingVertices + j);

				indices.push_back((i+1)*numRingVertices + j);
				indices.push_back(i*numRingVertices + j+1);
				indices.push_back((i+1)*numRingVertices + j+1);
			}
		}

		// Compute indices for top stack.  The top stack was written 
		// first to the vertex buffer.
		for(UINT i = 0; i < mNumSlices; ++i)
		{
			indices.push_back(northPoleIndex);
			indices.push_back(i+1);
			indices.push_back(i);
		}

		// Compute indices for bottom stack.  The bottom stack was written
		// last to the vertex buffer, so we need to offset to the index
		// of first vertex in the last ring.
		UINT baseIndex = (numRings-1)*numRingVertices;
		for(UINT i = 0; i < mNumSlices; ++i)
		{
			indices.push_back(southPoleIndex);
			indices.push_back(baseIndex+i);
			indices.push_back(baseIndex+i+1);
		}
	}

bool DiffSpecularSphere::CreateMesh(const D3D10_INPUT_ELEMENT_DESC* layout) {
		if (D3DX10CreateMesh(md3dDevice, 
			layout, 4, 
			"POSITION", 
			mNumVertices, 
			mNumFaces, 
			D3DX10_MESH_32_BIT, 
			&mMesh) == D3D_OK ) {
				/// TO ADD MESH's SET  VERTEX AND INDEX DATA, OTHERWISE INSERT WILL NOT WORK
				mMesh->SetVertexData(0, &(vertices[0]));
				mMesh->SetIndexData(&(indices[0]), indices.size());
				mMesh->CommitToDevice();
				return true;
		}
		return false;
	}

ID3DX10Mesh* DiffSpecularSphere::GetD3D10Mesh() {
		if (mMesh == NULL) {
			D3D10_INPUT_ELEMENT_DESC layout[] = {

				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0,  0,	D3D10_INPUT_PER_VERTEX_DATA, 0}, 
				{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	12, D3D10_INPUT_PER_VERTEX_DATA, 0},
				{ "DIFFUSE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,		0,	24, D3D10_INPUT_PER_VERTEX_DATA, 0},
				{ "SPECULAR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	40, D3D10_INPUT_PER_VERTEX_DATA, 0},
			};
			CreateMesh(layout);
		}
		return mMesh;
	}
