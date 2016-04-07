#ifndef __DIGITAL_NUMBER__
#define __DIGITAL_NUMBER__


#include "Utility/PCH.h"
#include "Geometry/vertex.h"
#include "Geometry/Geometry.h"

#pragma once

namespace EyeStereo {

	/** 0...9这10个数字所需要的坐标点数 */
	static const int digitnum[] = {16, 4, 20, 16, 12, 20, 20, 8, 20, 20};

	/**
		@file 
		@class digit_numbers
		用于随机点融合测试的数字
	*/
	class digit_numbers: public Geometry{

	public:
		/** 初始化数字坐标数组 */
		digit_numbers();
		~digit_numbers();
		void random_number();

		/** 
			@param[in] scale 伸缩量
			伸缩数字坐标
		*/
		void Scale(float scale);

		/** 
			@param[in] x x
			@param[in] y y
			数字的位置
		*/
		void Pos(float x, float y);
		

		virtual float distanceShiftDot(float x, float y);
		
		/**
			从一个随机序列中得到下一个数
		*/
		virtual void Select();
		

		/**
			返回当前的随机数
		*/
		virtual int getType();
		

		/**
			重新生成随机数序列
		*/
		void generate();

		int ChoosedNumber;
		Vertex*		ver[10];
		int randomList[10]; 
	
		
	};
}
#endif // __DIGITAL_NUMBER__