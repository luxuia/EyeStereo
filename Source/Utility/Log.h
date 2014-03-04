//--------------------------------------------------------------------------------
// This file is a portion of the Eye Stereo Project.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Luxuia
//---------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// Log
//
// The log class is a singleton that allows the application to write messages to 
// a file.
//--------------------------------------------------------------------------------
#ifndef Log_h
#define Log_h
//--------------------------------------------------------------------------------
#include "PCH.h"
//--------------------------------------------------------------------------------
namespace EyeStereo
{
	class Log 
	{
	protected:
		Log();

		std::wofstream	AppLog;

	public:

		static Log& Get( );

		bool Open( );
		bool Close( );

		bool Write( const wchar_t *TextString );
		bool Write( std::wstring& TextString );
		bool WriteSeparater( );
	};
};
//--------------------------------------------------------------------------------
#endif // Log_h
//--------------------------------------------------------------------------------