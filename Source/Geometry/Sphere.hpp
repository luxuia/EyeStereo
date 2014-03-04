#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Utility\PCH.h"
struct Vertex14;

namespace EyeStereo {
	class Sphere {
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

		Sphere() :mDiffColor(GREEN), md3dDevice(NULL), mVB(NULL), mIB(NULL), mMesh(NULL) {
		}

		~Sphere() {
			SAFE_RELEASE(md3dDevice);
			if (mVB != NULL) {
				SAFE_RELEASE(mVB);
			}

			if (mIB != NULL) {
				SAFE_RELEASE(mIB);
			}

			if (mMesh != NULL) {

				SAFE_RELEASE(mMesh);
			}
			indices.clear();
			vertices.clear();
		}


		void setColor(D3DXCOLOR color);

		void init(ID3D10Device* device, float radius, UINT numSlices, UINT numStacks);

		void draw();

		void buildStacks(std::vector<Vertex14> & vertices, std::vector<DWORD> & indices);

		bool CreateMesh(const D3D10_INPUT_ELEMENT_DESC* layout);

		ID3DX10Mesh* GetD3D10Mesh();

	};

}

#endif// __SPHERE_H__