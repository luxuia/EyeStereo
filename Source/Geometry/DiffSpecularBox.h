#ifndef __DiffSpecularBox_H__
#define __DiffSpecularBox_H__

#include "Utility/PCH.h"

#include "Geometry/Vertex.h"

namespace EyeStereo {
	enum {FOREVER, LIVE, DIE};

	class  DiffSpecularBox {
	public:
		DWORD mNumVertices;
		DWORD mNumFaces;

		ID3D10Buffer* mVB = NULL;
		ID3D10Buffer* mIB = NULL;

		D3DXCOLOR mDiffColor;
		D3DXCOLOR mSpecColor;
		Vertex14 v[24];
		DWORD    index[36];
		D3DXVECTOR3 world[24];

		D3DXPLANE	 pPlane[6];

		ID3D10Device*	pd3dDevice = NULL;
		ID3DX10Mesh*	pmesh = NULL;

		int live;
		float w, h, l;
		float x, y, z;

		DiffSpecularBox() {}

		DiffSpecularBox(float ll, float hh, float ww, D3DXCOLOR diff, D3DXCOLOR spec = YELLOW) {
			mDiffColor = diff;
			mSpecColor = spec;
			w = ww, h = hh, l = ll;
		}

		~DiffSpecularBox() {

			SAFE_RELEASE(mIB);
			SAFE_RELEASE(mVB);
			SAFE_RELEASE(pmesh);
		}

		void updatePlane(D3DXMATRIX &matWorld) {

			for (int i = 0; i < 6; i++) {
				D3DXPlaneFromPointNormal(&pPlane[i], &(world[i * 4 + 0]), &(v[i * 4].normal));
				//D3DXPlaneNormalize( pPlane[i], pPlane[i]);
			}

			/*
			D3DXMatrixInverse(&matWorld, NULL, &matWorld);
			D3DXMatrixTranspose(&matWorld, &matWorld);
			for (int i = 0; i < 6; i++) {

			D3DXPlaneNormalize(&pPlane[i], &pPlane[i]);
			D3DXPlaneTransform(&pPlane[i], &pPlane[i], &matWorld);
			D3DXPlaneNormalize(&pPlane[i], &pPlane[i]);
			}
			*/
			//CaculateWorldAxis(matWorld);
		}


		bool contain(int i, D3DXVECTOR3 &p) {
			if (p.x < min(world[i * 4].x, min(world[i * 4 + 1].x, min(world[i * 4 + 2].x, world[i * 4 + 3].x))) - Elapse)
				return false;
			if (p.x > max(world[i * 4].x, max(world[i * 4 + 1].x, max(world[i * 4 + 2].x, world[i * 4 + 3].x))) + Elapse)
				return false;
			/*
			if (p.y < min(world[i].y, min(world[i+1].y, min(world[i+2].y, world[i+3].y))))
			return false;
			if (p.y > max(world[i].y, max(world[i+1].y, max(world[i*4+2].y, world[i*4+3].y))))
			return false;
			*/
			if (p.z < min(world[i * 4].z, min(world[i * 4 + 1].z, min(world[i * 4 + 2].z, world[i * 4 + 3].z))) - Elapse)
				return false;
			if (p.z > max(world[i * 4].z, max(world[i * 4 + 1].z, max(world[i * 4 + 2].z, world[i * 4 + 3].z))) + Elapse)
				return false;


			return true;

		}

		void CaculateWorldAxis(D3DXMATRIX &mat) {

			for (int i = 0; i < 24; i++) {
				world[i] = v[i].pos + D3DXVECTOR3(x, y, z);
			}
		}
		float getDist(int i, D3DXVECTOR3 &p) {

			return D3DXPlaneDotCoord(&pPlane[i], &p);
		}

		void init(ID3D10Device* md3dDevice)
		{

			//SAFE_RELEASE( mVB );
			//SAFE_RELEASE( mIB );

			pd3dDevice = md3dDevice;
			mNumVertices = 24;
			mNumFaces = 12; // 2 per quad

			// Create vertex buffer
			// Fill in the front face vertex data.
			v[0] = Vertex14(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f);
			v[1] = Vertex14(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f);
			v[2] = Vertex14(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f);
			v[3] = Vertex14(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f);


			// Fill in the back face vertex data.
			v[4] = Vertex14(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			v[5] = Vertex14(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			v[6] = Vertex14(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			v[7] = Vertex14(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

			// Fill in the top face vertex data.
			v[8] = Vertex14(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f);
			v[9] = Vertex14(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f);
			v[10] = Vertex14(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f);
			v[11] = Vertex14(1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f);

			// Fill in the bottom face vertex data.
			v[12] = Vertex14(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f);
			v[13] = Vertex14(1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f);
			v[14] = Vertex14(1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f);
			v[15] = Vertex14(-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f);

			// Fill in the left face vertex data.
			v[16] = Vertex14(-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f);
			v[17] = Vertex14(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f);
			v[18] = Vertex14(-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f);
			v[19] = Vertex14(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f);

			// Fill in the right face vertex data.
			v[20] = Vertex14(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f);
			v[21] = Vertex14(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f);
			v[22] = Vertex14(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
			v[23] = Vertex14(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f);

			// Scale the box.

			for (UINT i = 0; i < mNumVertices; ++i) {
				v[i].diffuse = mDiffColor;
				v[i].spec = mSpecColor;
			}

			for (UINT i = 0; i < mNumVertices; ++i) {
				v[i].pos.x *= l;
				v[i].pos.y *= h;
				v[i].pos.z *= w;
			}

			D3D10_BUFFER_DESC vbd;
			vbd.Usage = D3D10_USAGE_IMMUTABLE;
			vbd.ByteWidth = sizeof(Vertex14)* mNumVertices;
			vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
			vbd.CPUAccessFlags = 0;
			vbd.MiscFlags = 0;
			D3D10_SUBRESOURCE_DATA vinitData;
			vinitData.pSysMem = v;
			md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB);


			// Create the index buffer


			// Fill in the front face index data
			index[0] = 0; index[1] = 1; index[2] = 2;
			index[3] = 0; index[4] = 2; index[5] = 3;

			// Fill in the back face index data
			index[6] = 4; index[7] = 5; index[8] = 6;
			index[9] = 4; index[10] = 6; index[11] = 7;

			// Fill in the top face index data
			index[12] = 8; index[13] = 9; index[14] = 10;
			index[15] = 8; index[16] = 10; index[17] = 11;

			// Fill in the bottom face index data
			index[18] = 12; index[19] = 13; index[20] = 14;
			index[21] = 12; index[22] = 14; index[23] = 15;

			// Fill in the left face index data
			index[24] = 16; index[25] = 17; index[26] = 18;
			index[27] = 16; index[28] = 18; index[29] = 19;

			// Fill in the right face index data
			index[30] = 20; index[31] = 21; index[32] = 22;
			index[33] = 20; index[34] = 22; index[35] = 23;

			D3D10_BUFFER_DESC ibd;
			ibd.Usage = D3D10_USAGE_IMMUTABLE;
			ibd.ByteWidth = sizeof(DWORD)* mNumFaces * 3;
			ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
			ibd.CPUAccessFlags = 0;
			ibd.MiscFlags = 0;
			D3D10_SUBRESOURCE_DATA iinitData;
			iinitData.pSysMem = index;
			md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB);

		}

		void draw()
		{
			UINT stride = sizeof(Vertex14);
			UINT offset = 0;
			pd3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
			pd3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
			pd3dDevice->DrawIndexed(mNumFaces * 3, 0, 0);
		}

		

		bool CreateMesh(const D3D10_INPUT_ELEMENT_DESC* layout) {
			if (D3DX10CreateMesh(pd3dDevice,
				layout, 4,
				"POSITION",
				mNumVertices,
				mNumFaces,
				D3DX10_MESH_32_BIT,
				&pmesh) == D3D_OK) {
				/// TO ADD MESH's SET  VERTEX AND INDEX DATA, OTHERWISE INTERSET WILL NOT WORK
				pmesh->SetVertexData(0, &(v[0]));
				pmesh->SetIndexData(&(index[0]), 36);
				pmesh->CommitToDevice();
				return true;
			}
			return false;
		}

		ID3DX10Mesh* GetD3D10Mesh() {
			if (pmesh == NULL) {
				D3D10_INPUT_ELEMENT_DESC layout[] = {

					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
					{ "DIFFUSE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
					{ "SPECULAR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 40, D3D10_INPUT_PER_VERTEX_DATA, 0 },
				};
				CreateMesh(layout);
			}
			return pmesh;
		}
	};

}

#endif //__DiffSpecularBox_H__