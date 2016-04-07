#include "MazeObject.h"

using namespace EyeStereo;

MazeObject::MazeObject()
{
	num = 0;
	id = 0;
}


MazeObject::~MazeObject()
{
	VecObject.clear();
	SAFE_RELEASE(pd3dDevice);
	SAFE_RELEASE(pEffect);

}

HRESULT MazeObject::InitMesh(g_Obj &t)
{
	HRESULT hr;
	CMeshLoader10* mesh = new CMeshLoader10();
	V_RETURN(mesh->Create(pd3dDevice, t.g_ObjectsPath));

	wprintf(L"Loading Mesh: %s\n", t.g_ObjectsName);

	for (UINT i = 0; i < mesh->GetNumMaterials(); ++i) {
		Material* pMaterial = mesh->GetMaterial(i);

		const char* strTechnique = "";

		if (pMaterial->pTextureRV10 && pMaterial->bSpecular)
			strTechnique = "TexturedSpecular";
		else if (pMaterial->pTextureRV10 && !pMaterial->bSpecular)
			strTechnique = "TexturedNoSpecular";
		else if (!pMaterial->pTextureRV10 && pMaterial->bSpecular)
			strTechnique = "Specular";
		else if (!pMaterial->pTextureRV10 && !pMaterial->bSpecular)
			strTechnique = "NoSpecular";

		pMaterial->pTechnique = pEffect->GetTechniqueByName(strTechnique);
	}
	t.g_ObjectsMesh = mesh;
}

HRESULT MazeObject::AddObject(WCHAR* Name, WCHAR* Path, D3DXVECTOR3 Position, D3DXVECTOR3 Size, bool flag)
{
	//tmp.g_ObjectsMesh = Mesh;
	HRESULT hr;
	if (flag) tmp.g_ObjectID = id++;
	num++;
	tmp.g_ObjectFlag = true;
	tmp.g_ObjectsName = Name;
	tmp.g_ObjectsPath = Path;
	tmp.g_ObjectsPosition = Position;
	tmp.g_ObjectResize = Size;
	V_RETURN(MazeObject::InitMesh(tmp));
	VecObject.push_back(tmp);
}

bool MazeObject::DeletObject(UINT id)
{
	bool suc = false;
	std::vector<g_Obj>::iterator it;
	for (it = VecObject.begin(); it != VecObject.end(); ++it)
	{
		int cal = it - VecObject.begin();
		if (VecObject[cal].g_ObjectID == id)
		{
			suc = true;
			it = VecObject.erase(it) - 1;
		}
	}
	return suc;
}

int MazeObject::PlayGame()
{

}