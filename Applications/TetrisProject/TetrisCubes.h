#ifndef __TETRIS_CUBES__
#define __TETRIS_CUBES__


#include "Utility/PCH.h"
#include "Helper/MeshLoader10.h"


namespace EyeStereo {
	class TetrisCubes {
	public:

/// const value define
		enum CubeTypes { LINE, AO, TU, FANG, };
		enum Status{ ACTIVE = 7, DOWN = 1, LEFT = 2, RIGHT = 4, NONE = 0, INVISIABLE = 8 };
		#define CONST_Z_ 5



		std::vector<D3DXVECTOR3>	WordPos;
		Status status;
		CMeshLoader10*	mesh;
		D3DXVECTOR3*					CentrePos;
		CubeTypes type;

		TetrisCubes() {
			CentrePos = NULL;
		}

		bool CollisionOne(UINT i, ID3DX10Mesh* othermesh, D3DXVECTOR3 &otherpos, Status dir, float space);
		void Move(float step, Status dir);
		void Move(float);

		bool Collision(ID3DX10Mesh* othermesh, D3DXVECTOR3 &otherpos, Status dir = DOWN, float space = 0.01) {
			for (UINT i = 0; i < WordPos.size(); ++i){
				if (CollisionOne(i, othermesh, otherpos, dir, space))
					return true;
			}
			return false;
		}

		bool Collision(TetrisCubes* otherCubes, Status dir = DOWN, float space = 0.01) {
			for (UINT i = 0; i < otherCubes->WordPos.size(); ++i) {
				if (Collision(otherCubes->mesh->GetMesh(), otherCubes->WordPos[i], dir, space))
					return true;
			}
			return false;
		}

		void Rotate90();
		D3DXVECTOR3* getCentrePos();

	};


	class TetrisCubesCreator {
	public:
		TetrisCubesCreator(CMeshLoader10 *cubeMesh) :CubeMesh(cubeMesh){}

		bool CreateRandomCube(D3DXVECTOR3 &wPos, TetrisCubes::Status status = TetrisCubes::DOWN) {
			TetrisCubes::CubeTypes type = RandomType();

			switch (type) {
			case TetrisCubes::LINE: { // -------
						   TetrisCubes cube;
						   cube.WordPos.push_back(wPos);
						   for (UINT i = 1; i < 4; i++) {
							   cube.WordPos.push_back(D3DXVECTOR3(++wPos.x, wPos.y, wPos.z));
						   }
						   cube.status = status;
						   cube.mesh = CubeMesh;
						   cube.type = TetrisCubes::LINE;
						   Cubes.push_back(cube);
						   return true;
			}
			case TetrisCubes::AO: { //����������
						 TetrisCubes cube;
						 cube.WordPos.push_back(wPos);
						 cube.WordPos.push_back(D3DXVECTOR3(wPos.x + 1, wPos.y, wPos.z));
						 cube.WordPos.push_back(D3DXVECTOR3(wPos.x + 2, wPos.y, wPos.z));
						 cube.WordPos.push_back(D3DXVECTOR3(wPos.x, wPos.y + 1, wPos.z));
						 cube.WordPos.push_back(D3DXVECTOR3(wPos.x + 2, wPos.y + 1, wPos.z));
						 cube.status = status;
						 cube.mesh = CubeMesh;
						 cube.type = TetrisCubes::AO;
						 Cubes.push_back(cube);
						 return true;
			}
			case TetrisCubes::TU: { //͹������
						 TetrisCubes cube;
						 cube.WordPos.push_back(wPos);
						 cube.WordPos.push_back(D3DXVECTOR3(wPos.x + 1, wPos.y, wPos.z));
						 cube.WordPos.push_back(D3DXVECTOR3(wPos.x + 2, wPos.y, wPos.z));
						 cube.WordPos.push_back(D3DXVECTOR3(wPos.x + 1, wPos.y + 1, wPos.z));
						 cube.status = status;
						 cube.mesh = CubeMesh;
						 cube.type = TetrisCubes::TU;
						 Cubes.push_back(cube);
						 return true;
			}
			case TetrisCubes::FANG: { //��������
						   TetrisCubes cube;
						   cube.WordPos.push_back(wPos);
						   cube.WordPos.push_back(D3DXVECTOR3(wPos.x + 1, wPos.y, wPos.z));
						   cube.WordPos.push_back(D3DXVECTOR3(wPos.x, wPos.y + 1, wPos.z));
						   cube.WordPos.push_back(D3DXVECTOR3(wPos.x + 1, wPos.y + 1, wPos.z));
						   cube.status = status;
						   cube.mesh = CubeMesh;
						   cube.type = TetrisCubes::FANG;
						   Cubes.push_back(cube);
						   return true;
			}
			default:
				return CreateRandomCube(wPos, status);
			}
		}

		bool CreateRandomCube(TetrisCubes::Status status = TetrisCubes::DOWN) {
			float x = floor(RandF(-3, 3));
			float y = floor(RandF(4, 6));
			float z = CONST_Z_;
			return CreateRandomCube(D3DXVECTOR3(x, y, z), status);
		}

	public:

		TetrisCubes::CubeTypes RandomType() {
			return (TetrisCubes::CubeTypes)(int(RandF() * 5));
		}

		std::vector<TetrisCubes> Cubes;
		CMeshLoader10*	CubeMesh;
	};


}



#endif //__TETRIS_CUBES__