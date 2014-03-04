#ifndef  __BOX_H__
#define __BOX_H__


#include "Common.h"
#include "Vertex.h"




#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }


struct  Box {
	DWORD mNumVertices;
	DWORD mNumFaces;

	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	

	void init(ID3D10Device* md3dDevice, float scale)
	{
		
		//SAFE_RELEASE( mVB );
		//SAFE_RELEASE( mIB );


		mNumVertices = 24;
		mNumFaces    = 12; // 2 per quad
		
		// Create vertex buffer
		Vertex8 v[24];

		// Fill in the front face vertex data.
 		v[0] = Vertex8(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		v[1] = Vertex8(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		v[2] = Vertex8( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		v[3] = Vertex8( 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

		// Fill in the back face vertex data.
		v[4] = Vertex8(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
		v[5] = Vertex8( 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
		v[6] = Vertex8( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		v[7] = Vertex8(-1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

		// Fill in the top face vertex data.
		v[8]  = Vertex8(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[9]  = Vertex8(-1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		v[10] = Vertex8( 1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		v[11] = Vertex8( 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

		// Fill in the bottom face vertex data.
		v[12] = Vertex8(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
		v[13] = Vertex8( 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
		v[14] = Vertex8( 1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
		v[15] = Vertex8(-1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);

		// Fill in the left face vertex data.
		v[16] = Vertex8(-1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[17] = Vertex8(-1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[18] = Vertex8(-1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		v[19] = Vertex8(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		// Fill in the right face vertex data.
		v[20] = Vertex8( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[21] = Vertex8( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[22] = Vertex8( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		v[23] = Vertex8( 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		// Scale the box.
		for(DWORD i = 0; i < mNumVertices; ++i)
			v[i].pos *= scale;


		D3D10_BUFFER_DESC vbd;
		vbd.Usage = D3D10_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
		vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		D3D10_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = v;
		md3dDevice->CreateBuffer( &vbd, &vinitData, &mVB );


		// Create the index buffer

		DWORD i[36];

		// Fill in the front face index data
		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 0; i[4] = 2; i[5] = 3;

		// Fill in the back face index data
		i[6] = 4; i[7]  = 5; i[8]  = 6;
		i[9] = 4; i[10] = 6; i[11] = 7;

		// Fill in the top face index data
		i[12] = 8; i[13] =  9; i[14] = 10;
		i[15] = 8; i[16] = 10; i[17] = 11;

		// Fill in the bottom face index data
		i[18] = 12; i[19] = 13; i[20] = 14;
		i[21] = 12; i[22] = 14; i[23] = 15;

		// Fill in the left face index data
		i[24] = 16; i[25] = 17; i[26] = 18;
		i[27] = 16; i[28] = 18; i[29] = 19;

		// Fill in the right face index data
		i[30] = 20; i[31] = 21; i[32] = 22;
		i[33] = 20; i[34] = 22; i[35] = 23;

		D3D10_BUFFER_DESC ibd;
		ibd.Usage = D3D10_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
		ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		D3D10_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = i;
		md3dDevice->CreateBuffer( &ibd, &iinitData, &mIB );

	}

	void draw(ID3D10Device * md3dDevice)
	{
		UINT stride = sizeof( Vertex8 );
		UINT offset = 0;
		md3dDevice->IASetVertexBuffers( 0, 1, &mVB, &stride, &offset );
		md3dDevice->IASetIndexBuffer( mIB, DXGI_FORMAT_R32_UINT, 0 );
		md3dDevice->DrawIndexed( mNumFaces*3, 0, 0 );
	}

	void destory()
	{
		SAFE_RELEASE( mIB );
		SAFE_RELEASE( mVB );
		
	}

};




#endif //__BOX_H__