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


		bool _upDownRightLeft[4];


		bool _qwert[5];


		bool _return[1];

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

