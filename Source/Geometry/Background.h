#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "Utility/PCH.h"
#include "Geometry/Vertex.h"

namespace EyeStereo {
	struct Background {

		DWORD	mNumVertices;
		DWORD	mNumFaces;
		UINT		mUnit;
		float	mWidth, mHeight;


		ID3D10Device* md3dDevice;
		ID3D10Buffer* mVB;
		ID3D10Buffer* mIB;

		Background() {
			md3dDevice = NULL;

			mVB = NULL;
			mIB = NULL;

		}

		~Background() {

			if (md3dDevice != NULL) {
				md3dDevice->Release();
			}

			if (mVB != NULL) {
				SAFE_RELEASE(mVB);
			}

			if (mIB != NULL) {
				SAFE_RELEASE(mIB);
			}
		}

		void init(ID3D10Device* device, int unit, float width, float height)
		{
			md3dDevice = device;
			md3dDevice->AddRef();


			std::vector<Vertex14> vertices;
			std::vector<DWORD> indices;

			mUnit = unit;
			mWidth = width;
			mHeight = height;


			assert(mUnit > 0, "Unit number must large than zero");
			assert(mWidth > 0 && mHeight > 0, "Width && Height Should be large than Zero");

			buildStacks(vertices, indices);



			mNumVertices = (UINT)vertices.size();
			mNumFaces = (UINT)indices.size() / 3;

			D3D10_BUFFER_DESC vbd;
			vbd.Usage = D3D10_USAGE_IMMUTABLE;
			vbd.ByteWidth = sizeof(Vertex14)* mNumVertices;
			vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
			vbd.CPUAccessFlags = 0;
			vbd.MiscFlags = 0;
			D3D10_SUBRESOURCE_DATA vinitData;
			vinitData.pSysMem = &vertices[0];
			md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB);

			D3D10_BUFFER_DESC ibd;
			ibd.Usage = D3D10_USAGE_IMMUTABLE;
			ibd.ByteWidth = sizeof(DWORD)* mNumFaces * 3;
			ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
			ibd.CPUAccessFlags = 0;
			ibd.MiscFlags = 0;
			D3D10_SUBRESOURCE_DATA iinitData;
			iinitData.pSysMem = &indices[0];
			md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB);
		}

		void draw() {
			UINT stride = sizeof(Vertex14);
			UINT offset = 0;
			md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
			md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
			md3dDevice->DrawIndexed(mNumFaces * 3, 0, 0);
		}

		void buildStacks(std::vector<Vertex14> & vertices, std::vector<DWORD> & indices){
			printf("Building Stacks\n");
			BuildBottom(vertices, indices);

			BuildLeft(vertices, indices);
			BuildRight(vertices, indices);
			BuildFront(vertices, indices);

		}

		void BuildBottom(std::vector<Vertex14> & vertices, std::vector<DWORD> & indices) {

			printf("Building Bottom\n");

			float unit = 2 * mWidth / mUnit;

			for (UINT i = 0; i < mUnit; i++) {
				for (UINT j = 0; j < mUnit; j++) {
					//printf("Caculating %d's Stack\n", i*mUnit+j);
					Vertex14 v;
					v.pos.x = -mWidth + j*unit;
					v.pos.y = 0;
					v.pos.z = mWidth - i*unit;
					v.normal = D3DXVECTOR3(0, 1, 0);
					v.diffuse = BLACK;
					v.spec = WHITE;
					vertices.push_back(v);
				}
			}

			for (UINT i = 0; i < mUnit - 1; i++) {
				for (UINT j = 0; j < mUnit - 1; j++) {
					//printf("Adding %d's Stack\n", i*mUnit+j);
					indices.push_back(i*mUnit + j);
					indices.push_back(i*mUnit + j + 1);
					indices.push_back((i + 1)*mUnit + j);

					indices.push_back(i*mUnit + j + 1);
					indices.push_back((i + 1)*mUnit + j + 1);
					indices.push_back((i + 1)*mUnit + j);
				}
			}
		}


		void BuildLeft(std::vector<Vertex14> & vertices, std::vector<DWORD> & indices) {


			float wunit = 2 * mWidth / mUnit;
			float hunit = mHeight / mUnit;
			UINT base = vertices.size();

			for (UINT i = 0; i < mUnit; i++) {
				for (UINT j = 0; j < mUnit; j++) {
					Vertex14 v;
					v.pos.x = -mWidth;
					v.pos.y = mHeight - i*hunit;
					v.pos.z = -mWidth + j*wunit;
					v.normal = D3DXVECTOR3(1, 0, 0);
					v.diffuse = BLACK;
					v.spec = WHITE;
					vertices.push_back(v);
				}
			}

			for (UINT i = 0; i < mUnit - 1; i++) {
				for (UINT j = 0; j < mUnit - 1; j++) {
					//printf("Adding %d's Stack\n", i*mUnit+j);
					indices.push_back(base + i*mUnit + j);
					indices.push_back(base + i*mUnit + j + 1);
					indices.push_back(base + (i + 1)*mUnit + j);

					indices.push_back(base + i*mUnit + j + 1);
					indices.push_back(base + (i + 1)*mUnit + j + 1);
					indices.push_back(base + (i + 1)*mUnit + j);
				}
			}

		}

		void BuildRight(std::vector<Vertex14> & vertices, std::vector<DWORD> & indices) {

			float wunit = 2 * mWidth / mUnit;
			float hunit = mHeight / mUnit;

			UINT base = vertices.size();
			for (UINT i = 0; i < mUnit; i++) {
				for (UINT j = 0; j < mUnit; j++) {

					Vertex14 v;
					v.pos.x = mWidth;
					v.pos.y = mHeight - i*hunit;
					v.pos.z = -mWidth + j*wunit;
					v.normal = D3DXVECTOR3(-1, 0, 0);
					v.diffuse = BLACK;
					v.spec = WHITE;
					vertices.push_back(v);
				}
			}

			for (UINT i = 0; i < mUnit - 1; i++) {
				for (UINT j = 0; j < mUnit - 1; j++) {
					//	printf("Adding %d's Stack\n", i*mUnit+j);
					indices.push_back(base + (i + 1)*mUnit + j);
					indices.push_back(base + i*mUnit + j + 1);
					indices.push_back(base + i*mUnit + j);


					indices.push_back(base + (i + 1)*mUnit + j);

					indices.push_back(base + (i + 1)*mUnit + j + 1);
					indices.push_back(base + i*mUnit + j + 1);
				}
			}

		}

		void BuildFront(std::vector<Vertex14> & vertices, std::vector<DWORD> & indices) {

			float wunit = 2 * mWidth / mUnit;
			float hunit = mHeight / mUnit;
			UINT base = vertices.size();

			for (UINT i = 0; i < mUnit; i++) {
				for (UINT j = 0; j < mUnit; j++) {

					Vertex14 v;
					v.pos.x = -mWidth + j*wunit;
					v.pos.y = mHeight - i*hunit;
					v.pos.z = mWidth;
					v.normal = D3DXVECTOR3(0, 0, -1);
					v.diffuse = BLACK;
					v.spec = WHITE;
					vertices.push_back(v);
				}
			}

			for (UINT i = 0; i < mUnit - 1; i++) {
				for (UINT j = 0; j < mUnit - 1; j++) {
					//	printf("Adding %d's Stack\n", i*mUnit+j);
					indices.push_back(base + i*mUnit + j);
					indices.push_back(base + i*mUnit + j + 1);
					indices.push_back(base + (i + 1)*mUnit + j);

					indices.push_back(base + i*mUnit + j + 1);
					indices.push_back(base + (i + 1)*mUnit + j + 1);
					indices.push_back(base + (i + 1)*mUnit + j);
				}
			}

		}



	};

}


#endif //__BACKGROUND_H__