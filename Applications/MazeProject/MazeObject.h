#ifndef __MAZE_OBJ__
#define __MAZE_OBJ__

#include "Utility/PCH.h"
#include "Stereo/StereoSetting.h"
#include "Geometry/Light.h"
#include "Helper/MeshLoader10.h"

namespace EyeStereo {

	class MazeObject {

	public:
		ID3D10Device*					pd3dDevice;
		ID3D10Effect*					pEffect;


		const int MAX_OBJNUM = 50;
		UINT num, id;
		struct g_Obj
		{
			bool				g_ObjectFlag;
			int					g_ObjectID;
			CMeshLoader10*		g_ObjectsMesh;
			WCHAR*				g_ObjectsPath;
			WCHAR*				g_ObjectsName;
			D3DXVECTOR3			g_ObjectsPosition;
			D3DXVECTOR3			g_ObjectResize;
		};
		struct g_Obj tmp;
		std::vector<g_Obj> VecObject;


		MazeObject();
		~MazeObject();
		
		bool MazeObject::DeletObject(UINT id);
		
		int MazeObject::PlayGame();

		HRESULT AddObject(WCHAR* Name, WCHAR* Path, D3DXVECTOR3 Position, D3DXVECTOR3 Size, bool flag);
		HRESULT InitMesh(g_Obj &t);
	};
}

#endif