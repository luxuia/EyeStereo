//--------------------------------------------------------------------------------
// This file is a portion of the Eye Stereo Project.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Luxuia
//---------------------------------------------------------------------------------

#ifndef __KEYBOARDSTATE_H__
#define __KEYBOARDSTATE_H__


namespace EyeStereo {
	struct KeyBoard {


		bool _upDownRightLeft[4]; ///< 上下右左键


		bool _qwert[5]; ///< QWERT键


		bool _return[1]; ///< Enter键

		/**
			@param[in] a 索引
			索引在0-3就是上下右左
			4-8 就是QWERT
			9 就是ENTER
		*/
		bool& operator[](int a) {
			if (a < 4)
				return _upDownRightLeft[a];
			else if (a < 9)
				return _qwert[a-4];
			//if (a == 9)
				return _return[0];
		}
	};

}
#endif //__KEYBOARDSTATE_H__

