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
#include "PCH.h"
#include "Log.h"

//--------------------------------------------------------------------------------
using namespace EyeStereo;
//--------------------------------------------------------------------------------
Log::Log()
{
}
//--------------------------------------------------------------------------------
Log& Log::Get()
{
	static Log log;
	return( log );
}
//--------------------------------------------------------------------------------
bool Log::Open()
{
	std::wstring filename = L"Log.txt";
	AppLog.open( filename.c_str(), std::fstream::app);

	Write( L"Log file opened." );

	return( true );
}
//--------------------------------------------------------------------------------
bool Log::Write( const wchar_t *cTextString )
{
	AppLog << cTextString << "\n";
#if _DEBUG
	::OutputDebugStringW( cTextString );
	::OutputDebugStringW( L"\n" );
#endif
	return( true );
}
//--------------------------------------------------------------------------------
bool Log::Write( std::wstring& TextString )
{
	Log::Write( TextString.c_str() );
	AppLog.flush();
	return( true );
}
//--------------------------------------------------------------------------------
bool Log::Close( )
{
	Write( L"Log file closed." );
	AppLog.close();
	return( true );
}
//--------------------------------------------------------------------------------
bool Log::WriteSeparater( )
{
	Write( L"------------------------------------------------------------" );

	return( true );
}
//--------------------------------------------------------------------------------