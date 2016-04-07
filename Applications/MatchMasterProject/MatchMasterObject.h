#ifndef __MATCH_MASTER_OBJ__
#define __MATCH_MASTER_OBJ__

#include "Utility/PCH.h"
#include "Stereo/StereoSetting.h"
#include "Geometry/Light.h"
#include "Helper/MeshLoader10.h"

namespace EyeStereo {

	class MatchMasterObject {

	public:
		ID3D10Device*					pd3dDevice;
		ID3D10Effect*					pEffect;


		const int MAX_OBJNUM = 50;
		UINT num, id;
		struct g_Obj
		{
//			bool				g_ObjectFlag;
			int					g_ObjectID;
			int					g_ObjectNum;
			CMeshLoader10*		g_ObjectsMesh;
			WCHAR*				g_ObjectsPath;
			WCHAR*				g_ObjectsName;
			D3DXVECTOR3			g_ObjectsPosition;
			D3DXVECTOR3			g_ObjectResize;
		};
		struct g_Obj tmp;
		std::vector<g_Obj> VecObject;


		MatchMasterObject();
		~MatchMasterObject();
		
		void MatchMasterObject::DeletObject(UINT id1, UINT id2);
		
		int MatchMasterObject::PlayGame();

		HRESULT AddObject(WCHAR* Name, WCHAR* Path, D3DXVECTOR3 Position, D3DXVECTOR3 Size, bool flag);
		HRESULT InitMesh(g_Obj &t);
	};
}

#endif