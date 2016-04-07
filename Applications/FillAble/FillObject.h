#ifndef __TETRIS_CUBES__
#define __TETRIS_CUBES__


#include "Utility/PCH.h"
#include "Helper/MeshLoader10.h"
#include "Helper/MeshHelper.h"
#include "Utility/Log.h"

namespace EyeStereo {
	class ModelObject {
	public:

/// const value define
		/**
			方块，与俄罗斯公用，待解决
		*/
		enum CubeTypes { 
			LINE, ///< 条块
			AO,   ///< 凹块
			TU,   ///< 凸块
			FANG, ///< 方块
		};

		/**
			方块的状态
		*/
		enum Status{ 
			ACTIVE = 7,   	///< 活动
			DOWN = 1,   	///< 正在下降
			LEFT = 2, 		///< 左
			RIGHT = 4, 
			NONE = 0, 		///< 不正常
			INVISIABLE = 8, ///< 不可见
		};

		/**
			方块Z轴深度纬度
		*/
		#define CONST_Z_ 5   

		D3DXVECTOR3	WordPos;
		Status status;
		CMeshLoader10*	mesh;
		CubeTypes type;

		ModelObject() {
			mesh = NULL;
		}

		bool CollisionOne(ID3DX10Mesh* othermesh, D3DXVECTOR3 &otherpos, Status dir, float space);
		void Move(float step, Status dir);
		void DirectMove(float step, Status dir);
		D3DXVECTOR3 TestMove(float step, Status dir);
		void Move(float);

		bool Collision(ID3DX10Mesh* othermesh, D3DXVECTOR3 &otherpos, Status dir = DOWN, float space = 0.01) {
			//for (UINT i = 0; i < WordPos.size(); ++i){
				if (CollisionOne(othermesh, otherpos, dir, space))
					return true;
			//}
			return false;
		}

		bool Collision(ModelObject* otherCubes, Status dir = DOWN, float space = 0.01) {
			//for (UINT i = 0; i < otherCubes->WordPos.size(); ++i) {
				if (Collision(otherCubes->mesh->GetMesh(), otherCubes->WordPos, dir, space))
					return true;
			//}
			return false;
		}

		void Rotate90();
		D3DXVECTOR3* getCentrePos();

	};


	class ModelObjectCreator {
	public:
		ModelObjectCreator(MeshHelper& meshhelp, const std::wstring* nutpath, const std::wstring* screwpath, int size){

			NutObject = new ModelObject();
			ScrewObject = new ModelObject();
			NutObject->WordPos = D3DXVECTOR3(0, 0, 0);

			listsize = size;
			NutMeshList = (CMeshLoader10**)malloc(sizeof(CMeshLoader10*) * size);
			ScrewMeshList = (CMeshLoader10**)malloc(sizeof(CMeshLoader10*)* size);
			for (UINT32 i = 0; i < size; ++i) {
				NutMeshList[i] = new CMeshLoader10();
				meshhelp.CreateMesh(NutMeshList[i], nutpath[i].c_str());

				ScrewMeshList[i] = new CMeshLoader10();
				meshhelp.CreateMesh(ScrewMeshList[i], screwpath[i].c_str());
			}
		}

		~ModelObjectCreator() {
			for (UINT i = 0; i < listsize; ++i) {
				NutMeshList[i]->Destroy();
				ScrewMeshList[i]->Destroy();
			}
		}

		bool CreateRandomCube(D3DXVECTOR3 &wPos, ModelObject::Status status = ModelObject::DOWN) {
			ModelObject::CubeTypes type = RandomType();

			NutObject->mesh = NutMeshList[type];
			ScrewObject->WordPos = wPos;
			ScrewObject->mesh = ScrewMeshList[type];
			ScrewObject->status = ModelObject::ACTIVE;
			return true;
		}

		bool CreateRandomCube(ModelObject::Status status = ModelObject::DOWN) {
			float x = floor(RandF(-30, 30));
			float y = floor(RandF(50, 70));
			float z = 0;
			return CreateRandomCube(D3DXVECTOR3(x, y, z), status);
		}

		ModelObject::CubeTypes RandomType() {
			return (ModelObject::CubeTypes)(int(RandF() * listsize));
		}

		ModelObject* NutObject;
		ModelObject* ScrewObject;

		CMeshLoader10** ScrewMeshList;
		CMeshLoader10** NutMeshList;
		int listsize;
	};


}



#endif //__TETRIS_CUBES__