#ifndef __MESH_HELPER__
#define __MESH_HELPER__

#include "Utility/Log.h"

class MeshHelper {
public :
	MeshHelper(ID3D10Device* &pDevice, ID3D10Effect*& pEffect,const WCHAR*	fxFile):pd3dDevice(pDevice), pEffect(pEffect) {
		HRESULT hr;
		V (D3DX10CreateEffectFromFile( fxFile, NULL, NULL, "fx_4_0", 
			D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG, 0,
			pDevice, NULL, NULL, &pEffect, NULL, NULL ) );
		
		pTech = pEffect->GetTechniqueByName("NoSpecular");
		g_ptxDiffuseVariable = pEffect->GetVariableByName( "g_MeshTexture" )->AsShaderResource();    

		g_pAmbient = pEffect->GetVariableByName( "g_vMaterialAmbient" )->AsVector();
		g_pDiffuse = pEffect->GetVariableByName( "g_vMaterialDiffuse" )->AsVector();
		g_pSpecular = pEffect->GetVariableByName( "g_vMaterialSpecular" )->AsVector();
		g_pOpacity = pEffect->GetVariableByName( "g_fMaterialAlpha" )->AsScalar();
		g_pSpecularPower = pEffect->GetVariableByName( "g_nMaterialShininess" )->AsScalar();
		g_pLightColor = pEffect->GetVariableByName( "g_vLightColor" )->AsVector();
		g_pLightPosition = pEffect->GetVariableByName( "g_vLightPosition" )->AsVector();
		g_pCameraPosition = pEffect->GetVariableByName( "g_vCameraPosition" )->AsVector();
		g_pTime = pEffect->GetVariableByName( "g_fTime" )->AsScalar();
		g_pWorld = pEffect->GetVariableByName( "g_mWorld" )->AsMatrix();
		g_pWorldViewProjection = pEffect->GetVariableByName( "g_mWorldViewProjection" )->AsMatrix();
	}

	void CreateMesh(CMeshLoader10* mesh, const WCHAR*	meshFile) {
		mesh->Create(pd3dDevice, meshFile);
		//mesh->GetMesh()->GenerateGSAdjacency();
		wprintf(L"Loading Mesh: %s\n", meshFile);
		for (UINT i = 0; i < mesh->GetNumMaterials(); ++i) {
			Material* pMaterial = mesh->GetMaterial( i );

			const char* strTechnique = "";

			if( pMaterial->pTextureRV10 && pMaterial->bSpecular )
				strTechnique = "TexturedSpecular";
			else if( pMaterial->pTextureRV10 && !pMaterial->bSpecular )
				strTechnique = "TexturedNoSpecular";
			else if( !pMaterial->pTextureRV10 && pMaterial->bSpecular )
				strTechnique = "Specular";
			else if( !pMaterial->pTextureRV10 && !pMaterial->bSpecular )
				strTechnique = "NoSpecular";

			pMaterial->pTechnique = pEffect->GetTechniqueByName( strTechnique );
		}
	}

	void RenderMesh(CMeshLoader10*	mesh, D3DXMATRIX	&world, D3DXMATRIX &WVP, float fTime) {
		HRESULT hr;

		V( g_pWorldViewProjection->SetMatrix( (float*)&WVP ) );
		V( g_pWorld->SetMatrix( (float*)&world ) );
		V( g_pTime->SetFloat( (float)fTime ) );

		for (UINT iSubSet = 0; iSubSet < mesh->GetNumSubsets(); ++iSubSet) {
			Material *pMaterial = mesh->GetSubsetMaterial(iSubSet);
			V( g_pAmbient->SetFloatVector( pMaterial->vAmbient ) );
			V( g_pDiffuse->SetFloatVector( pMaterial->vDiffuse ) );
			V( g_pSpecular->SetFloatVector( pMaterial->vSpecular ) );
			V( g_pOpacity->SetFloat( pMaterial->fAlpha ) );
			V( g_pSpecularPower->SetInt( pMaterial->nShininess ) );

			if ( !IsErrorResource( pMaterial->pTextureRV10 ) )
				g_ptxDiffuseVariable->SetResource( pMaterial->pTextureRV10 );

			D3D10_TECHNIQUE_DESC techDesc;
			pMaterial->pTechnique->GetDesc(&techDesc);
			for (UINT p = 0; p < techDesc.Passes; ++p) {
				pMaterial->pTechnique->GetPassByIndex(p)->Apply(0);
				mesh->GetMesh()->DrawSubset(iSubSet);
			}
		}
	}




	ID3D10EffectTechnique*				pTech;
	ID3D10Device*						&pd3dDevice;
	ID3D10Effect*						&pEffect;
	ID3D10EffectShaderResourceVariable* g_ptxDiffuseVariable;
	ID3D10EffectVectorVariable*         g_pAmbient;
	ID3D10EffectVectorVariable*         g_pDiffuse;
	ID3D10EffectVectorVariable*         g_pSpecular;
	ID3D10EffectScalarVariable*         g_pOpacity;
	ID3D10EffectScalarVariable*         g_pSpecularPower;
	ID3D10EffectVectorVariable*         g_pLightColor;
	ID3D10EffectVectorVariable*         g_pLightPosition;
	ID3D10EffectVectorVariable*         g_pCameraPosition;
	ID3D10EffectScalarVariable*         g_pTime;
	ID3D10EffectMatrixVariable*         g_pWorld;
	ID3D10EffectMatrixVariable*         g_pWorldViewProjection;

};




#endif //__MESH_HELPER__