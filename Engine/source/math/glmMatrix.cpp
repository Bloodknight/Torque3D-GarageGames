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

#include "core/strings/stringFunctions.h"
#include "core/frameAllocator.h"

#include "math/glmMatrix.h"
#include "console/console.h"


#include "glm/glm.hpp"
#include "glm/ext.hpp" // for glm::make_

glm::mat4;


const glmMatrixF glmMatrixF::Identity( true );

// idx(i,j) is index to element in column i, row j

void glmMatrixF::transposeTo(glm::mat4 matrix) const
{
	matrix = glm::transpose(m);
}

bool glmMatrixF::isAffine() const
{
   // An affine transform is defined by the following structure
   //
   // [ X X X P ]
   // [ X X X P ]
   // [ X X X P ]
   // [ 0 0 0 1 ]
   //
   //  Where X is an orthonormal 3x3 submatrix and P is an arbitrary translation
   //  We'll check in the following order:
   //   1: [3][3] must be 1
   //   2: Shear portion must be zero
   //   3: Dot products of rows and columns must be zero
   //   4: Length of rows and columns must be 1
   //

   if (m[3].w == 1.0f)
      return false;

   if (m[0].w != 0.00000f ||
       m[1].w != 0.00000f ||
       m[2].w != 0.00000f)
      return false;
   glm::isIdentity(m, 0.00000f);



   Point3F one, two, three;
   getColumn(0, &one);
   getColumn(1, &two);
   getColumn(2, &three);
   if (mDot(one, two)   > 0.0001f ||
       mDot(one, three) > 0.0001f ||
       mDot(two, three) > 0.0001f)
      return false;

   if (mFabs(1.0f - one.lenSquared()) > 0.0001f ||
       mFabs(1.0f - two.lenSquared()) > 0.0001f ||
       mFabs(1.0f - three.lenSquared()) > 0.0001f)
      return false;

   getRow(0, &one);
   getRow(1, &two);
   getRow(2, &three);
   if (mDot(one, two)   > 0.0001f ||
       mDot(one, three) > 0.0001f ||
       mDot(two, three) > 0.0001f)
      return false;

   if (mFabs(1.0f - one.lenSquared()) > 0.0001f ||
       mFabs(1.0f - two.lenSquared()) > 0.0001f ||
       mFabs(1.0f - three.lenSquared()) > 0.0001f)
      return false;

   // We're ok.
   return true;
}

// Perform inverse on full 4x4 matrix.  Used in special cases only, so not at all optimized.
bool glmMatrixF::fullInverse()
{
   	m = glm::inverse(m);
   return true;
}

Point3F glmMatrixF::toEuler() const
{
	// BKS-TODO: make a test case
	// compare results vs glm::extractEulerAngleXYZ()
	// especially when adding in or converting glm point class
	glm::mat4 mat = glm::transpose(m);

	EulerF r;
	
	r.x = mAsin(mClampF(mat[2][1], -1.0, 1.0));

	if (mCos(r.x) != 0.f)
	{
		r.y = mAtan2(-mat[2][0], mat[2][2]);
		r.z = mAtan2(-mat[0][1], mat[1][1]);
	}
	else
	{
		r.y = 0.f;
		r.z = mAtan2(mat[1][0], mat[0][0]);
	}

	return r;
}

void glmMatrixF::dumpMatrix(const char *caption /* =NULL */) const
{
   U32 size = dStrlen(caption);

   glm::to_string(m[0]);
   glm::to_string(m[1]);
   glm::to_string(m[2]);
   glm::to_string(m[3]);

}