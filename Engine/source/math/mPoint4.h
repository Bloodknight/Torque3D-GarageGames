//-----------------------------------------------------------------------------
// Copyright (c) 2012 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef _MPOINT4_H_
#define _MPOINT4_H_

#ifndef _MMATHFN_H_
#include "math/mMathFn.h"
#endif

#ifndef _MPOINT3_H_
#include "math/mPoint3.h"
#endif

/*	---------------------------------------------------
*		GLM Library Implementation
*
*	TODO
*	Reduce #Pragama Warnings, to 0 if possible
*	Reduce bloat by using only necessary headers from glm and ext
*
*/

#pragma warning( disable:4201)
#pragma warning( disable:4310)
#pragma warning( disable:4324)
#include "glm/glm.hpp"
#include "glm/ext.hpp"



//------------------------------------------------------------------------------
/// 4D integer point
///
/// Uses S32 internally. Currently storage only.
class Point4I
{
public:
	Point4I() {}
	Point4I(glm::ivec4 s);
	Point4I(S32 _x, S32 _y, S32 _z, S32 _w);

	void zero();   ///< Zero all values

	glm::ivec4 v;



	//-------------------------------------- Public static constants
public:
	const static Point4I One;
	const static Point4I Zero;
};

//------------------------------------------------------------------------------
// -------------------------	End Point4I

inline void Point4I::zero()
{
	v.x = v.y = v.z = v.w = 0;
}

inline Point4I::Point4I(glm::ivec4 vec)
{
	v = vec;
}

inline Point4I::Point4I(S32 _x, S32 _y, S32 _z, S32 _w)
{
	v.x = _x;
	v.y = _y;
	v.z = _z;
	v.w = _w;
}
// -------------------------	End Point4I


//------------------------------------------------------------------------------
/// 4D floating-point point.
///
/// Uses F32 internally.
///
/// Useful for representing quaternions and other 4d beasties.
class Point4F
{
   //-------------------------------------- Public data
  public:
/*
   F32 x;   ///< X co-ordinate.
   F32 y;   ///< Y co-ordinate.
   F32 z;   ///< Z co-ordinate.
   F32 w;   ///< W co-ordinate.
*/
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		glm::vec4 v;
	};

  public:
   Point4F();               ///< Create an uninitialized point.
   Point4F(const Point4F&); ///< Copy constructor.

   /// Create point from coordinates.
   Point4F(F32 _x, F32 _y, F32 _z, F32 _w);

   Point4F(glm::vec4 v);

   /// Set point's coordinates.
   void set(F32 _x, F32 _y, F32 _z, F32 _w);

   /// Interpolate from _pt1 to _pt2, based on _factor.
   ///
   /// @param   _pt1    Starting point.
   /// @param   _pt2    Ending point.
   /// @param   _factor Interpolation factor (0.0 .. 1.0).
   void interpolate(const Point4F& _pt1, const Point4F& _pt2, F32 _factor);

   void zero();

   operator F32*() { return (&v.x); }
   operator const F32*() const { return &v.x; }
   
   F32 len() const;

   Point4F operator/(F32) const;

   Point4F  operator*(F32) const;
   Point4F  operator+(const Point4F&) const;
   Point4F& operator+=(const Point4F&);
   Point4F  operator-(const Point4F&) const;      
   Point4F  operator*(const Point4F&) const;
   Point4F& operator*=(const Point4F&);
   Point4F& operator=(const Point3F&);
   Point4F& operator=(const Point4F&);

	Point3F asPoint3F() const
	{
		return Point3F(v.x, v.y, v.z);
	}

	//-------------------------------------- Public static constants
  public:
	const static Point4F One;
	const static Point4F Zero;
};

typedef Point4F Vector4F;   ///< Points can be vectors!

//------------------------------------------------------------------------------
// -------------------------	Start Point4F

inline Point4F::Point4F()
{
	v = glm::vec4(0);
}

inline Point4F::Point4F(const Point4F& _copy)
{
	v.x = _copy.v.x;
	v.y = _copy.v.y;
	v.z = _copy.v.z;
	v.w = _copy.v.w;
}

inline Point4F::Point4F(glm::vec4 s)
{
	v = s;
}

inline Point4F::Point4F(F32 _x, F32 _y, F32 _z, F32 _w)
{
	v.x = _x;
	v.y = _y;
	v.z = _z;
	v.w = _w;

	}
inline void Point4F::set(F32 _x, F32 _y, F32 _z, F32 _w)
{
   v.x = _x;
   v.y = _y;
   v.z = _z;
   v.w = _w;
}

inline F32 Point4F::len() const
{
	//glm::sqrt();
   return mSqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
}

inline void Point4F::interpolate(const Point4F& _from, const Point4F& _to, F32 _factor)
{
	v.x = (_from.v.x * (1.0f - _factor)) + (_to.v.x * _factor);
	v.y = (_from.v.y * (1.0f - _factor)) + (_to.v.y * _factor);
	v.z = (_from.v.z * (1.0f - _factor)) + (_to.v.z * _factor);
	v.w = (_from.v.w * (1.0f - _factor)) + (_to.v.w * _factor);
}

/*inline void Point4F::interpolate(const Point4F& _from, const Point4F& _to, F32 _factor)
{
	glm::mix(_from, _to, _factor);
}*/

inline void Point4F::zero()
{
	v = glm::vec4(0);
}

inline Point4F& Point4F::operator=(const Point3F& _vec)
{
	v = glm::vec4(glm::vec3(_vec), 1.0f);
	return *this;
}

inline Point4F& Point4F::operator=(const Point4F& _vec)
{
	v = glm::vec4(_vec);
	return *this;
}

inline Point4F Point4F::operator+(const Point4F& _add) const
{
	return v + glm::vec4(_add);
}

inline Point4F& Point4F::operator+=(const Point4F& _add)
{
	v += glm::vec4(_add);
	return *this;
}

inline Point4F Point4F::operator-(const Point4F& _rSub) const
{
	return v - glm::vec4(_rSub);
}

inline Point4F Point4F::operator*(const Point4F& _vec) const
{
	return v * glm::vec4(_vec);
}

inline Point4F Point4F::operator*(F32 _mul) const
{
	return glm::mul(v, _mul);
}

inline Point4F Point4F::operator /(F32 t) const
{
   F32 f = 1.0f / t;
   return Point4F( v.x * f, v.y * f, v.z * f, v.w * f );   
}

// -------------------------	End Point4F

//-------------------------------------------------------------------
// Non-Member Operators
//-------------------------------------------------------------------

inline Point4F operator*(F32 mul, const Point4F& multiplicand)
{
   return multiplicand * mul;
}

inline bool mIsNaN( const Point4F &p )
{
	return glm::isnan(p);
   //return mIsNaN_F( p.x ) || mIsNaN_F( p.y ) || mIsNaN_F( p.z ) || mIsNaN_F( p.w );
}

#endif // _MPOINT4_H_
