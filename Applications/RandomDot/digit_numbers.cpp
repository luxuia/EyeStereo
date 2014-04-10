#include "digit_numbers.h"
#include "Mathematic\Vector2f.h"

using namespace EyeStereo;

float digit_numbers::distanceShiftDot(float x, float y) {
	Vertex* number = ver[ChoosedNumber];
	Vector2f pos(x, y);
	for (UINT i = 0; i < digitnum[ChoosedNumber]; i+=4) {
		if (InGeometry(number[i].pos.xy(), number[i + 1].pos.xy(), number[i + 2].pos.xy(), pos)) {
			return 1;
		}
		if (InGeometry(number[i+1].pos.xy(), number[i + 2].pos.xy(), number[i + 3].pos.xy(), pos)) {
			return 1;
		}
	}
	return 0;
}

digit_numbers::digit_numbers() {
	//digit 0
	ver[0] = new Vertex[digitnum[0]];
	ver[0][0] = Vertex(0, 0, 0, WHITE);
	ver[0][1] = Vertex(5, 0, 0, WHITE);
	ver[0][2] = Vertex(0, 1, 0, WHITE);
	ver[0][3] = Vertex(5, 1, 0, WHITE);

	ver[0][4] = Vertex(0, 1, 0, WHITE);
	ver[0][5] = Vertex(1, 1, 0, WHITE);
	ver[0][6] = Vertex(0, 10, 0, WHITE);
	ver[0][7] = Vertex(1, 10, 0, WHITE);

	ver[0][8] = Vertex(1, 9, 0, WHITE);
	ver[0][9] = Vertex(1, 10, 0, WHITE);
	ver[0][10] = Vertex(4, 9, 0, WHITE);
	ver[0][11] = Vertex(4, 10, 0, WHITE);

	ver[0][12] = Vertex(4, 1, 0, WHITE);
	ver[0][13] = Vertex(5, 1, 0, WHITE);
	ver[0][14] = Vertex(4, 10, 0, WHITE);
	ver[0][15] = Vertex(5, 10, 0, WHITE);

	//digit 1
	ver[1] = new Vertex[digitnum[1]];
	ver[1][0] = Vertex(0, 0, 0, WHITE);
	ver[1][1] = Vertex(1, 0, 0, WHITE);
	ver[1][2] = Vertex(0, 10, 0, WHITE);
	ver[1][3] = Vertex(1, 10, 0, WHITE);

	//digit 2
	ver[2] = new Vertex[digitnum[2]];
	ver[2][0] = Vertex(0, 0, 0, WHITE);
	ver[2][1] = Vertex(5, 0, 0, WHITE);
	ver[2][2] = Vertex(0, 1, 0, WHITE);
	ver[2][3] = Vertex(5, 1, 0, WHITE);

	ver[2][4] = Vertex(4, 1, 0, WHITE);
	ver[2][5] = Vertex(5, 1, 0, WHITE);
	ver[2][6] = Vertex(4, 5, 0, WHITE);
	ver[2][7] = Vertex(5, 5, 0, WHITE);

	ver[2][8] = Vertex(0, 4, 0, WHITE);
	ver[2][9] = Vertex(4, 4, 0, WHITE);
	ver[2][10] = Vertex(0, 5, 0, WHITE);
	ver[2][11] = Vertex(4, 5, 0, WHITE);

	ver[2][12] = Vertex(0, 5, 0, WHITE);
	ver[2][13] = Vertex(1, 5, 0, WHITE);
	ver[2][14] = Vertex(0, 10, 0, WHITE);
	ver[2][15] = Vertex(1, 10, 0, WHITE);

	ver[2][16] = Vertex(0, 5, 0, WHITE);
	ver[2][17] = Vertex(1, 5, 0, WHITE);
	ver[2][18] = Vertex(0, 10, 0, WHITE);
	ver[2][19] = Vertex(1, 10, 0, WHITE);

	//digit 3
	ver[3] = new Vertex[digitnum[3]];
	ver[3][0] = Vertex(0, 0, 0, WHITE);
	ver[3][1] = Vertex(5, 0, 0, WHITE);
	ver[3][2] = Vertex(0, 1, 0, WHITE);
	ver[3][3] = Vertex(5, 1, 0, WHITE);

	ver[3][4] = Vertex(4, 1, 0, WHITE);
	ver[3][5] = Vertex(5, 1, 0, WHITE);
	ver[3][6] = Vertex(4, 10, 0, WHITE);
	ver[3][7] = Vertex(5, 10, 0, WHITE);

	ver[3][8] = Vertex(0, 4, 0, WHITE);
	ver[3][9] = Vertex(0, 5, 0, WHITE);
	ver[3][10] = Vertex(4, 4, 0, WHITE);
	ver[3][11] = Vertex(4, 5, 0, WHITE);

	ver[3][12] = Vertex(0, 9, 0, WHITE);
	ver[3][13] = Vertex(4, 9, 0, WHITE);
	ver[3][14] = Vertex(0, 10, 0, WHITE);
	ver[3][15] = Vertex(4, 10, 0, WHITE);

	//digit 4
	ver[4] = new Vertex[digitnum[4]];
	ver[4][0] = Vertex(0, 0, 0, WHITE);
	ver[4][1] = Vertex(1, 0, 0, WHITE);
	ver[4][2] = Vertex(0, 5, 0, WHITE);
	ver[4][3] = Vertex(1, 5, 0, WHITE);

	ver[4][4] = Vertex(1, 4, 0, WHITE);
	ver[4][5] = Vertex(1, 5, 0, WHITE);
	ver[4][6] = Vertex(4, 4, 0, WHITE);
	ver[4][7] = Vertex(4, 5, 0, WHITE);

	ver[4][8] = Vertex(4, 0, 0, WHITE);
	ver[4][9] = Vertex(5, 0, 0, WHITE);
	ver[4][10] = Vertex(4, 10, 0, WHITE);
	ver[4][11] = Vertex(5, 10, 0, WHITE);

	//digit 5
	ver[5] = new Vertex[digitnum[5]];
	ver[5][0] = Vertex(0, 0, 0, WHITE);
	ver[5][1] = Vertex(5, 0, 0, WHITE);
	ver[5][2] = Vertex(0, 1, 0, WHITE);
	ver[5][3] = Vertex(5, 1, 0, WHITE);

	ver[5][4] = Vertex(0, 1, 0, WHITE);
	ver[5][5] = Vertex(1, 1, 0, WHITE);
	ver[5][6] = Vertex(0, 5, 0, WHITE);
	ver[5][7] = Vertex(1, 5, 0, WHITE);

	ver[5][8] = Vertex(1, 4, 0, WHITE);
	ver[5][9] = Vertex(5, 4, 0, WHITE);
	ver[5][10] = Vertex(1, 5, 0, WHITE);
	ver[5][11] = Vertex(5, 5, 0, WHITE);

	ver[5][12] = Vertex(4, 5, 0, WHITE);
	ver[5][13] = Vertex(5, 5, 0, WHITE);
	ver[5][14] = Vertex(4, 10, 0, WHITE);
	ver[5][15] = Vertex(5, 10, 0, WHITE);

	ver[5][16] = Vertex(0, 9, 0, WHITE);
	ver[5][17] = Vertex(4, 9, 0, WHITE);
	ver[5][18] = Vertex(0, 10, 0, WHITE);
	ver[5][19] = Vertex(5, 9, 0, WHITE);

	//digit 6
	ver[6] = new Vertex[digitnum[6]];
	ver[6][0] = Vertex(0, 0, 0, WHITE);
	ver[6][1] = Vertex(5, 0, 0, WHITE);
	ver[6][2] = Vertex(0, 1, 0, WHITE);
	ver[6][3] = Vertex(5, 1, 0, WHITE);

	ver[6][4] = Vertex(0, 1, 0, WHITE);
	ver[6][5] = Vertex(1, 1, 0, WHITE);
	ver[6][6] = Vertex(0, 10, 0, WHITE);
	ver[6][7] = Vertex(1, 10, 0, WHITE);

	ver[6][8] = Vertex(1, 4, 0, WHITE);
	ver[6][9] = Vertex(5, 4, 0, WHITE);
	ver[6][10] = Vertex(1, 5, 0, WHITE);
	ver[6][11] = Vertex(5, 5, 0, WHITE);

	ver[6][12] = Vertex(1, 9, 0, WHITE);
	ver[6][13] = Vertex(5, 9, 0, WHITE);
	ver[6][14] = Vertex(1, 10, 0, WHITE);
	ver[6][15] = Vertex(5, 10, 0, WHITE);

	ver[6][16] = Vertex(4, 5, 0, WHITE);
	ver[6][17] = Vertex(5, 5, 0, WHITE);
	ver[6][18] = Vertex(4, 9, 0, WHITE);
	ver[6][19] = Vertex(5, 9, 0, WHITE);


	//digit 7
	ver[7] = new Vertex[digitnum[7]];
	ver[7][0] = Vertex(0, 0, 0, WHITE);
	ver[7][1] = Vertex(5, 0, 0, WHITE);
	ver[7][2] = Vertex(0, 1, 0, WHITE);
	ver[7][3] = Vertex(5, 1, 0, WHITE);

	ver[7][4] = Vertex(4, 1, 0, WHITE);
	ver[7][5] = Vertex(5, 1, 0, WHITE);
	ver[7][6] = Vertex(4, 10, 0, WHITE);
	ver[7][7] = Vertex(5, 10, 0, WHITE);


	//digit 8
	ver[8] = new Vertex[digitnum[8]];
	ver[8][0] = Vertex(0, 0, 0, WHITE);
	ver[8][1] = Vertex(5, 0, 0, WHITE);
	ver[8][2] = Vertex(0, 1, 0, WHITE);
	ver[8][3] = Vertex(5, 1, 0, WHITE);

	ver[8][4] = Vertex(0, 1, 0, WHITE);
	ver[8][5] = Vertex(1, 1, 0, WHITE);
	ver[8][6] = Vertex(0, 10, 0, WHITE);
	ver[8][7] = Vertex(1, 10, 0, WHITE);

	ver[8][8] = Vertex(1, 4, 0, WHITE);
	ver[8][9] = Vertex(4, 4, 0, WHITE);
	ver[8][10] = Vertex(1, 5, 0, WHITE);
	ver[8][11] = Vertex(4, 5, 0, WHITE);

	ver[8][12] = Vertex(1, 9, 0, WHITE);
	ver[8][13] = Vertex(4, 9, 0, WHITE);
	ver[8][14] = Vertex(1, 10, 0, WHITE);
	ver[8][15] = Vertex(4, 10, 0, WHITE);

	ver[8][16] = Vertex(4, 1, 0, WHITE);
	ver[8][17] = Vertex(5, 1, 0, WHITE);
	ver[8][18] = Vertex(4, 10, 0, WHITE);
	ver[8][19] = Vertex(5, 10, 0, WHITE);

	//digit 9
	ver[9] = new Vertex[digitnum[9]];
	ver[9][0] = Vertex(0, 0, 0, WHITE);
	ver[9][1] = Vertex(5, 0, 0, WHITE);
	ver[9][2] = Vertex(0, 1, 0, WHITE);
	ver[9][3] = Vertex(5, 1, 0, WHITE);

	ver[9][4] = Vertex(0, 1, 0, WHITE);
	ver[9][5] = Vertex(1, 1, 0, WHITE);
	ver[9][6] = Vertex(0, 5, 0, WHITE);
	ver[9][7] = Vertex(1, 5, 0, WHITE);

	ver[9][8] = Vertex(1, 4, 0, WHITE);
	ver[9][9] = Vertex(4, 4, 0, WHITE);
	ver[9][10] = Vertex(1, 5, 0, WHITE);
	ver[9][11] = Vertex(4, 5, 0, WHITE);

	ver[9][12] = Vertex(0, 9, 0, WHITE);
	ver[9][13] = Vertex(4, 9, 0, WHITE);
	ver[9][14] = Vertex(0, 10, 0, WHITE);
	ver[9][15] = Vertex(4, 10, 0, WHITE);

	ver[9][16] = Vertex(4, 1, 0, WHITE);
	ver[9][17] = Vertex(5, 1, 0, WHITE);
	ver[9][18] = Vertex(4, 10, 0, WHITE);
	ver[9][19] = Vertex(5, 10, 0, WHITE);
}



digit_numbers::~digit_numbers() {
	for (int i = 0; i < 10; ++i) {
		SAFE_DELETE(ver[i]);
	}
}

void digit_numbers::Scale(float scale) {
	for (UINT i = 0; i < 10; ++i) {
		for (UINT j = 0; j < digitnum[i]; ++j) {
			ver[i][j].pos *= scale;
		}
	}
}

void digit_numbers::Pos(float x, float y) {
	for (UINT i = 0; i < 10; ++i) {
		for (UINT j = 0; j < digitnum[i]; ++j) {
			ver[i][j].pos += Vector3f(x, y, 0);
		}
	}
}

void digit_numbers::random() {
	srand(time(0));
	ChoosedNumber = rand() % 10;
}

