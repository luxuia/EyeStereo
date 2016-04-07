
#include "FillObject.h"

namespace EyeStereo{

	/**
		@param[in] othermesh 另一个元素的模型
		@param[in] otherpos 另一个元素的位置
		@param[in] dir 判断相交的方向
		@param[in] space 相交的判断精度
		@return 是否相交
	*/
	bool ModelObject::CollisionOne(ID3DX10Mesh* othermesh, D3DXVECTOR3 &otherpos, Status dir = DOWN, float space = 0.01) {
		D3DXVECTOR3 dir3;
		D3DXVECTOR3 origin = WordPos;
		if (dir & DOWN) {
			dir3 = D3DXVECTOR3(0, -1, 0);
			/*origin.y -= 0.5;*/
		} else if (dir == LEFT) {
			dir3 = D3DXVECTOR3(-1, 0, 0);
			/*origin.x -= 0.5;*/
		} else if (dir == RIGHT) {
			dir3 = D3DXVECTOR3(+1, 0, 0);
			/*origin.x += 0.5;*/
		}
		float dist;
		D3DXMATRIX msword;
		D3DXMATRIX inverseW;

		D3DXMatrixTranslation(&msword, otherpos.x, otherpos.y, otherpos.z);
		D3DXMatrixInverse(&inverseW, 0, &msword);
		D3DXVECTOR3 torigin, tdir;

		D3DXVec3TransformCoord(&torigin, &origin, &inverseW);
		D3DXVec3TransformNormal(&tdir, &dir3, &inverseW);

		UINT hitCount;

		othermesh->Intersect(&torigin, &tdir, &hitCount, NULL, NULL, NULL, &dist, NULL);

		if (abs(dist-0.5) < space + 0.001) {

			return true;
		};

		fprintf(stdout, "space %f\n", dist);
		return false;
	}


	/**
		@param[in] step 移动的距离
		@param[in] dir 移动的方向
		@return 测试情况下移动到的位置
	*/
	D3DXVECTOR3 ModelObject::TestMove(float step, Status dir = NONE) {
		D3DXVECTOR3 pos(WordPos);
		if (dir == RIGHT) {
			pos.x += step;
		} else if (dir == LEFT) {
			pos.x -= step;
		} else if (dir == DOWN) {
			pos.y -= step;
		}
		return pos;
	}

	void ModelObject::DirectMove(float step, Status dir = NONE) {

	}

	void ModelObject::Move(float step, Status dir) {
		if (dir == RIGHT)
			WordPos.x += step;
		else if (dir == LEFT) {
			WordPos.x -= step;
		} else if (dir == DOWN) {
			WordPos.y -= step;
		} 
	}

	void ModelObject::Move(float step) {
		if (step > Elapse) 
			Move(step, RIGHT);
		else if (step < -Elapse ){
			Move(-step, LEFT);
		}
	}
}