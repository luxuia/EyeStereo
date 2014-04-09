//--------------------------------------------------------------------------------
// This file is a portion of the Eye Stereo Project.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Luxuia
//---------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
// Vector3f
//----------------------------------------------------------------------------------------------------
#ifndef Vector3f_h
#define Vector3f_h

#include "Vector2f.h"
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
namespace EyeStereo
{
	class Vector3f
	{
	public:
		Vector3f( );
		Vector3f( float x, float y, float z );
		Vector3f( const Vector3f& Vector );

		// vector operations
		void Clamp( );
		Vector3f Cross( const Vector3f& A ) const;
		float Dot( const Vector3f& A ) const;
		void MakeZero( );
		float Magnitude( );
		void Normalize( );
        Vector3f Perpendicular( );

		Vector2f xy();


        // static vector ops
        static Vector3f Clamp( const Vector3f& A );
        static Vector3f Cross( const Vector3f& A, const Vector3f& B );
        static float Dot( const Vector3f& A, const Vector3f& B );
        static float LengthSq( const Vector3f& A );
		static Vector3f Normalize( const Vector3f& A );
        static Vector3f Perpendicular( const Vector3f& A );
		static Vector3f Random( );


		// assignment
		Vector3f& operator= (const Vector3f& Vector);

		// member access
		float operator[] (int iPos) const;
		float& operator[] (int iPos);

		// comparison
		bool operator== ( const Vector3f& Vector ) const;
		bool operator!= ( const Vector3f& Vector ) const;

		// arithmetic operations
		Vector3f operator+ ( const Vector3f& Vector ) const;
		Vector3f operator- ( const Vector3f& Vector ) const;
        Vector3f operator* ( const Vector3f& Vector ) const;
		Vector3f operator* ( float fScalar ) const;
        Vector3f operator/ ( const Vector3f& Vector ) const;
		Vector3f operator/ ( float fScalar ) const;
		Vector3f operator- ( ) const;

		// arithmetic updates
		Vector3f& operator+= ( const Vector3f& Vector );
		Vector3f& operator-= ( const Vector3f& Vector );
        Vector3f& operator*= ( const Vector3f& Vector );
		Vector3f& operator*= ( float fScalar );
        Vector3f& operator/= ( const Vector3f& Vector );
		Vector3f& operator/= ( float fScalar );

	public:
		float x;
		float y;
		float z;
	};
};
//----------------------------------------------------------------------------------------------------
#endif // Vector3f_h
