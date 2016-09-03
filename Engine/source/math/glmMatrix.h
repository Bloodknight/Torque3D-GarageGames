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

#ifndef _GLMMATRIX_H_
#define _GLMMATRIX_H_

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#ifndef _MPLANE_H_
#include "math/mPlane.h"
#endif

#ifndef _MBOX_H_
#include "math/mBox.h"
#endif

#ifndef _MPOINT4_H_
#include "math/mPoint4.h"
#endif

/// 4x4 Matrix Class
///
/// This runs at F32 precision.

inline glm::vec3 Point3FtoVec3(Point3F val)
{
	glm::vec3 newval;
	newval.x = val.x;
	newval.y = val.y;
	newval.z = val.z;
	return newval;
}

inline Point3F Vec3toPoint3F(glm::vec3 val)
{
	Point3F newval;
	newval.x = val.x;
	newval.y = val.y;
	newval.z = val.z;
	return newval;
}

class glmMatrixF
{
private:
	
	F32 _m[16];
	glm::mat4 m;		///< Note: glm uses column-major matrices
						///  Previously used row major; likely a major source of error if not careful

public:
   /// Create an uninitialized matrix.
   ///
   /// @param   identity    If true, initialize to the identity matrix.
   explicit glmMatrixF(bool identity=false);

   /// Create a matrix to rotate about origin by e.
   /// @see set
   explicit glmMatrixF( const EulerF &e);

   /// Create a matrix to rotate about p by e.
   /// @see set
   glmMatrixF( const EulerF &e, const Point3F& p);

   /// Get the index in m to element in column i, row j
   ///
   /// This is necessary as we have m as a one dimensional array.
   ///
   /// @param   i   Column desired.
   /// @param   j   Row desired.
   static U32 idx(U32 i, U32 j) { return (i + j*4); }

   /// Initialize matrix to rotate about origin by e.
   glmMatrixF& set( const EulerF &e);

   /// Initialize matrix to rotate about p by e.
   glmMatrixF& set( const EulerF &e, const Point3F& p);

   /// Initialize matrix with a cross product of p.
   glmMatrixF& setCrossProduct( const Point3F &p);

   /// Initialize matrix with a tensor product of p.
   glmMatrixF& setTensorProduct( const Point3F &p, const Point3F& q);

   //operator glm::mat4*() { return ((glm::mat4)m); }              ///< Allow people to get at m.
   //operator const glm::mat4*() const { return (m); }  ///< Allow people to get at m.

   bool isAffine() const;                       ///< Check to see if this is an affine matrix.
   bool isIdentity() const;                     ///< Checks for identity matrix.

   /// Make this an identity matrix.
   glmMatrixF& identity();

   /// Invert m.
   glmMatrixF& inverse();
   /// Copy the inversion of this into out matrix.
   void invertTo(glmMatrixF *out );

   /// Take inverse of matrix assuming it is affine (rotation,
   /// scale, sheer, translation only).
   glmMatrixF& affineInverse();

   /// Swap rows and columns.
   glmMatrixF& transpose();

   /// M * Matrix(p) -> M
   glmMatrixF& scale( const Point3F &s );
   glmMatrixF& scale( F32 s ) { return scale( Point3F( s, s, s ) ); }

   /// Return scale assuming scale was applied via mat.scale(s).
   Point3F getScale() const;

   EulerF toEuler() const;

   /// Compute the inverse of the matrix.
   ///
   /// Computes inverse of full 4x4 matrix. Returns false and performs no inverse if
   /// the determinant is 0.
   ///
   /// Note: In most cases you want to use the normal inverse function.  This method should
   ///       be used if the matrix has something other than (0,0,0,1) in the bottom row.
   bool fullInverse();

   /// Swaps rows and columns into matrix.
   //void transposeTo(F32 *matrix) const;
   //void transposeTo(glm::mat4 * matrix) const;
   void transposeTo(glm::mat4 matrix) const;

   /// Normalize the matrix.
   void normalize();

   /// Copy the requested column into a Point4F.
   void getColumn(S32 col, Point4F *cptr) const;
   Point4F getColumn4F(S32 col) const { Point4F ret; getColumn(col,&ret); return ret; }

   /// Copy the requested column into a Point3F.
   ///
   /// This drops the bottom-most row.
   void getColumn(S32 col, Point3F *cptr) const;
   Point3F getColumn3F(S32 col) const { Point3F ret; getColumn(col,&ret); return ret; }

   /// Set the specified column from a Point4F.
   void setColumn(S32 col, const Point4F& cptr);

   /// Set the specified column from a Point3F.
   ///
   /// The bottom-most row is not set.
   void setColumn(S32 col, const Point3F& cptr);

   /// Copy the specified row into a Point4F.
   void getRow(S32 row, Point4F *cptr) const;
   Point4F getRow4F(S32 row) const { Point4F ret; getRow(row,&ret); return ret; }

   /// Copy the specified row into a Point3F.
   ///
   /// Right-most item is dropped.
   void getRow(S32 row, Point3F *cptr) const;
   Point3F getRow3F(S32 row) const { Point3F ret; getRow(row,&ret); return ret; }

   /// Set the specified row from a Point4F.
   void setRow(S32 row, const Point4F& cptr);

   /// Set the specified row from a Point3F.
   ///
   /// The right-most item is not set.
   void setRow(S32 row, const Point3F& cptr);

   /// Get the position of the matrix.
   ///
   /// This is the 4th column of the matrix.
   Point3F getPosition() const;

   /// Set the position of the matrix.
   ///
   /// This is the 4th column of the matrix.
   void setPosition( const Point3F &pos ) { setColumn( 3, pos ); }

   /// Add the passed delta to the matrix position.
   void displace( const Point3F &delta );

   /// Get the x axis of the matrix.
   ///
   /// This is the 1st column of the matrix and is
   /// normally considered the right vector.
   VectorF getRightVector() const;

   /// Get the y axis of the matrix.
   ///
   /// This is the 2nd column of the matrix and is
   /// normally considered the forward vector.   
   VectorF getForwardVector() const;   

   /// Get the z axis of the matrix.
   ///
   /// This is the 3rd column of the matrix and is
   /// normally considered the up vector.   
   VectorF getUpVector() const;

   glmMatrixF&  mul(const glmMatrixF &a);                    ///< M * a -> M
   glmMatrixF&  mulL(const glmMatrixF &a);                   ///< a * M -> M
   glmMatrixF&  mul(const glmMatrixF &a, const glmMatrixF &b);  ///< a * b -> M

   // Scalar multiplies
   glmMatrixF&  mul(const F32 a);                         ///< M * a -> M
   glmMatrixF&  mul(const glmMatrixF &a, const F32 b);       ///< a * b -> M


   void mul( Point4F& p ) const;                       ///< M * p -> p (full [4x4] * [1x4])
   void mulP( Point3F& p ) const;                      ///< M * p -> p (assume w = 1.0f)
   void mulP( const Point3F &p, Point3F *d) const;     ///< M * p -> d (assume w = 1.0f)
   void mulV( VectorF& p ) const;                      ///< M * v -> v (assume w = 0.0f)
   void mulV( const VectorF &p, Point3F *d) const;     ///< M * v -> d (assume w = 0.0f)

   void mul(Box3F& b) const;                           ///< Axial box -> Axial Box
   
   glmMatrixF& add( const glmMatrixF& m );

   void dumpMatrix(const char *caption=nullptr) const;

   // Math operator overloads
   //------------------------------------
   friend glmMatrixF operator * ( const glmMatrixF &m1, const glmMatrixF &m2 );
   glmMatrixF& operator *= ( const glmMatrixF &m );

   // Static identity matrix
   const static glmMatrixF Identity;
};


//--------------------------------------
// Inline Functions

inline glmMatrixF::glmMatrixF(bool _identity)
{
   if (_identity)
      identity();
}

inline glmMatrixF::glmMatrixF( const EulerF &e )
{
   set(e);
}

//inline glmMatrixF::glmMatrixF(const glm::vec3& e, const glm::vec3& p)

inline glmMatrixF::glmMatrixF(const EulerF& e, const Point3F& p)
{
	set(e, p);
}

inline glmMatrixF& glmMatrixF::set(const EulerF& e)
{
	
	glm::vec3 p;
	p = Point3FtoVec3(e);

	glm::eulerAngleXYZ(p.x, p.y, p.z);

	m_matF_set_euler(e, *this);
	return (*this);
}


inline glmMatrixF& glmMatrixF::set(const EulerF& e, const Point3F& p)
{
	m_matF_set_euler_point(e, p, *this);
	return (*this);
}

inline glmMatrixF& glmMatrixF::setCrossProduct( const Point3F& p)
{
	_m[1] = -(_m[4] = p.z);
	_m[8] = -(_m[2] = p.y);
	_m[6] = -(_m[9] = p.x);
	_m[0] = _m[3] = _m[5] = _m[7] = _m[10] = _m[11] = _m[12] = _m[13] = _m[14] = 0.0f;
	_m[15] = 1;
	m = glm::make_mat4(_m);
   return (*this);
}

inline glmMatrixF& glmMatrixF::setTensorProduct(const Point3F& p, const Point3F& q)
{
   m[0][0] = p.x * q.x;
   m[1] = p.x * q.y;
   m[2] = p.x * q.z;
   m[4] = p.y * q.x;
   m[5] = p.y * q.y;
   m[6] = p.y * q.z;
   m[8] = p.z * q.x;
   m[9] = p.z * q.y;
   m[10] = p.z * q.z;
   m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0.0f;
   m[15] = 1.0f;
   return (*this);
}

inline bool glmMatrixF::isIdentity() const
{
	return glm::isIdentity(m, 0.0f);
}

inline glmMatrixF& glmMatrixF::identity()
{
	m = glm::mat4();
   return (*this);
}

inline glmMatrixF& glmMatrixF::inverse()
{
   glm::inverse(m);
   return (*this);
}

inline void glmMatrixF::invertTo( glmMatrixF *out )
{
   //m_matF_invert_to(m, *out);
}

inline glmMatrixF& glmMatrixF::affineInverse()
{
   glm::affineInverse(m);
   return (*this);
}

inline glmMatrixF& glmMatrixF::transpose()
{
	m = glm::transpose(m);
   return (*this);
}

inline glmMatrixF& glmMatrixF::scale(const Point3F& p)
{
   m_matF_scale(m,p);
   return *this;
}

inline Point3F glmMatrixF::getScale() const
{
   Point3F scale;
   scale.x = mSqrt(m[0]*m[0] + m[4] * m[4] + m[8] * m[8]);
   scale.y = mSqrt(m[1]*m[1] + m[5] * m[5] + m[9] * m[9]);
   scale.z = mSqrt(m[2]*m[2] + m[6] * m[6] + m[10] * m[10]);
   return scale;
}

inline void glmMatrixF::normalize()
{
   m_matF_normalize(m);
}

inline glmMatrixF& glmMatrixF::mul( const glmMatrixF &a )
{  // M * a -> M
   AssertFatal(&a != this, "glmMatrixF::mul - a.mul(a) is invalid!");

   glmMatrixF tempThis(*this);
   m_matF_x_matF(tempThis, a, *this);
   return (*this);
}

inline glmMatrixF& glmMatrixF::mulL( const glmMatrixF &a )
{  // a * M -> M
   AssertFatal(&a != this, "glmMatrixF::mulL - a.mul(a) is invalid!");

   glmMatrixF tempThis(*this);
   m_matF_x_matF(a, tempThis, *this);
   return (*this);
}

inline glmMatrixF& glmMatrixF::mul( const glmMatrixF &a, const glmMatrixF &b )
{  // a * b -> M
   AssertFatal((&a != this) && (&b != this), "glmMatrixF::mul - a.mul(a, b) a.mul(b, a) a.mul(a, a) is invalid!");

   m_matF_x_matF(a, b, *this);
   return (*this);
}


inline glmMatrixF& glmMatrixF::mul(const F32 a)
{
   for (U32 i = 0; i < 16; i++)
      m[i] *= a;

   return *this;
}


inline glmMatrixF& glmMatrixF::mul(const glmMatrixF &a, const F32 b)
{
   *this = a;
   mul(b);

   return *this;
}

inline void glmMatrixF::mul( Point4F& p ) const
{
   Point4F temp;
   m_matF_x_point4F(*this, &p.x, &temp.x);
   p = temp;
}

inline void glmMatrixF::mulP( Point3F& p) const
{
   // M * p -> d
   Point3F d;
   m_matF_x_point3F(*this, &p.x, &d.x);
   p = d;
}

inline void glmMatrixF::mulP( const Point3F &p, Point3F *d) const
{
   // M * p -> d
   m_matF_x_point3F(*this, &p.x, &d->x);
}

inline void glmMatrixF::mulV( VectorF& v) const
{
   // M * v -> v
   VectorF temp;
   m_matF_x_vectorF(*this, &v.x, &temp.x);
   v = temp;
}

inline void glmMatrixF::mulV( const VectorF &v, Point3F *d) const
{
   // M * v -> d
   m_matF_x_vectorF(*this, &v.x, &d->x);
}

inline void glmMatrixF::mul(Box3F& b) const
{
   m_matF_x_box3F(*this, &b.minExtents.x, &b.maxExtents.x);
}

inline glmMatrixF& glmMatrixF::add( const glmMatrixF& a )
{
   for( U32 i = 0; i < 16; ++ i )
      m[ i ] += a.m[ i ];
      
   return *this;
}

inline void glmMatrixF::getColumn(S32 col, Point4F *cptr) const
{
   cptr->x = m[col];
   cptr->y = m[col+4];
   cptr->z = m[col+8];
   cptr->w = m[col+12];
}

inline void glmMatrixF::getColumn(S32 col, Point3F *cptr) const
{
   cptr->x = m[col];
   cptr->y = m[col+4];
   cptr->z = m[col+8];
}

inline void glmMatrixF::setColumn(S32 col, const Point4F &cptr)
{
   m[col]   = cptr.x;
   m[col+4] = cptr.y;
   m[col+8] = cptr.z;
   m[col+12]= cptr.w;
}

inline void glmMatrixF::setColumn(S32 col, const Point3F &cptr)
{
   m[col]   = cptr.x;
   m[col+4] = cptr.y;
   m[col+8] = cptr.z;
}


inline void glmMatrixF::getRow(S32 col, Point4F *cptr) const
{
   col *= 4;
   cptr->x = m[col++];
   cptr->y = m[col++];
   cptr->z = m[col++];
   cptr->w = m[col];
}

inline void glmMatrixF::getRow(S32 col, Point3F *cptr) const
{
   col *= 4;
   cptr->x = m[col++];
   cptr->y = m[col++];
   cptr->z = m[col];
}

inline void glmMatrixF::setRow(S32 col, const Point4F &cptr)
{
   col *= 4;
   m[col++] = cptr.x;
   m[col++] = cptr.y;
   m[col++] = cptr.z;
   m[col]   = cptr.w;
}

inline void glmMatrixF::setRow(S32 col, const Point3F &cptr)
{
   col *= 4;
   m[col++] = cptr.x;
   m[col++] = cptr.y;
   m[col]   = cptr.z;
}

inline Point3F glmMatrixF::getPosition() const
{
	glm::posit
	return Point3F( m[3], m[3+4], m[3+8] );
}

inline void glmMatrixF::displace( const Point3F &delta )
{
   m[3]   += delta.x;
   m[3+4] += delta.y;
   m[3+8] += delta.z;
}

inline VectorF glmMatrixF::getForwardVector() const
{
   VectorF vec;
   getColumn( 1, &vec );
   vec = glm::column(m, 0);

   return vec;
}

inline VectorF glmMatrixF::getRightVector() const
{
   VectorF vec;
   getColumn( 0, &vec );
   return vec;
}

inline VectorF glmMatrixF::getUpVector() const
{
   VectorF vec;
   getColumn( 2, &vec );
   return vec;
}

//------------------------------------
// Math operator overloads
//------------------------------------
inline glmMatrixF operator * ( const glmMatrixF &m1, const glmMatrixF &m2 )
{
   // temp = m1 * m2
   glmMatrixF temp;
   m_matF_x_matF(m1, m2, temp);
   return temp;
}

inline glmMatrixF& glmMatrixF::operator *= ( const glmMatrixF &m1 )
{
   glmMatrixF tempThis(*this);
   m_matF_x_matF(tempThis, m1, *this);
   return (*this);
}

//------------------------------------
// Non-member methods
//------------------------------------

inline void mTransformPlane(const glmMatrixF& mat, const Point3F& scale, const PlaneF&  plane, PlaneF * result)
{
   m_matF_x_scale_x_planeF(mat , &scale.x, &plane.x, &result->x);

}

#endif //_GLMMATRIX_H_
