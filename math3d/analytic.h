/****************************************************************************/
/*    Math3D Library. Performs 3D Vector and matrix calculations.           */
/*    Simle 3D graphics objects.					    */
/*    Copyright (C), 1991-2012,  Andrei Volkov                              */
/*                                                                          */
/*    This program is free software: you can redistribute it and/or modify  */
/*    it under the terms of the GNU General Public License as published by  */
/*    the Free Software Foundation, either version 3 of the License, or     */
/*    (at your option) any later version.                                   */
/*                                                                          */
/*    This program is distributed in the hope that it will be useful,       */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/*    GNU General Public License for more details.                          */
/*                                                                          */
/*    You should have received a copy of the GNU General Public License     */
/*    along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/****************************************************************************/
/*   Analytic MATH header						    */
/*   A. Volkov, 1991							    */
/****************************************************************************/

#ifndef __MVECT
#define __MVECT

#include <math.h>

//#define REAL_TYPE float     /* double , long double */
#define REAL_TYPE double
//#define REAL_TYPE qreal
#define I_MATRIX  matrix(1,0,0,0,1,0,0,0,1)
#define N_VECTOR  vector(0,0,0)

#define VO(OPERATOR) {for (int k=0; k<3; k++) OPERATOR;}

#define MO(OPERATOR) {int i,j; for (i=0;i<3;i++)  \
				for (j=0;j<3;j++) \
				 OPERATOR;}

class vector;
class matrix;

class vector
{
 protected:
  REAL_TYPE e[3];
 public:
  vector() {}
  vector(REAL_TYPE e0,REAL_TYPE e1=0,REAL_TYPE e2=0)
   {e[0]=e0,e[1]=e1,e[2]=e2;}
  REAL_TYPE& operator [](int i)     //  indexes
   {return e[i];}
  REAL_TYPE normalization();
  vector& operator +=(vector v);    //  adding vector
  vector& operator -=(vector v);    //  substracting vector
  friend vector operator -(vector v); //  inversion (multiplication by -1)
  friend vector operator +(vector v1,vector v2);  //  sum of two vectors
  friend vector operator -(vector v1,vector v2);  //  substraction of two vectors
  friend vector operator %(vector v1,vector v2);  //  multiplication of two vectors, cross product
  friend vector operator *(vector v,REAL_TYPE r); //  multiplication by scalar
  friend vector operator *(REAL_TYPE r,vector v); //  multiplication by scalar
  friend vector operator /(vector v,REAL_TYPE r); //  division by scalar
  friend REAL_TYPE operator *(vector v1,vector v2); //  multiplication of two vectors, dot product
  friend vector operator *(matrix m,vector v);
  friend REAL_TYPE abs(vector v);     // magnitude
  friend vector ort(vector v);        // 1-covector
  friend int operator ==(vector v1,vector v2);  // comparation
};


class matrix
{
 protected:
  REAL_TYPE e[3][3];
 public:
  matrix(REAL_TYPE e00,REAL_TYPE e01,REAL_TYPE e02,
	 REAL_TYPE e10,REAL_TYPE e11,REAL_TYPE e12,
	 REAL_TYPE e20,REAL_TYPE e21,REAL_TYPE e22)
   {e[0][0]=e00;e[0][1]=e01;e[0][2]=e02;
    e[1][0]=e10;e[1][1]=e11;e[1][2]=e12;
    e[2][0]=e20;e[2][1]=e21;e[2][2]=e22;}
  matrix() {}
  matrix(vector v0,vector v1,vector v2)
   {e[0][0]=v0[0];e[0][1]=v0[1];e[0][2]=v0[2];
    e[1][0]=v1[0];e[1][1]=v1[1];e[1][2]=v1[2];
    e[2][0]=v2[0];e[2][1]=v2[1];e[2][2]=v2[2];}

  vector operator [](int i)
   {return vector(e[i][0],e[i][1],e[i][2]);}
  vector operator ()(int i)
   {return vector(e[0][i],e[1][i],e[2][i]);}
  matrix& operator*=(matrix m);
  friend matrix operator -(matrix m1,matrix m2);
  friend matrix operator +(matrix m1,matrix m2);
  friend matrix operator *(matrix m,REAL_TYPE r);
  friend matrix operator *(REAL_TYPE r,matrix m);
  friend matrix operator /(matrix m,REAL_TYPE r);
  friend matrix operator *(matrix m1,matrix m2);
  friend vector operator *(matrix m,vector v);
  friend matrix operator ~(matrix m);
  friend int operator ==(matrix m1,matrix m2);
};


class location :public vector
{
 public:
  location() : N_VECTOR {}
  location(vector v) : vector(v) {}
  location(REAL_TYPE e0,REAL_TYPE e1=0,REAL_TYPE e2=0) :
   vector(e0,e1,e2) {}

  friend location& loc(location l)
   {return l.l_();}
  friend REAL_TYPE range(location c1,location c2)
   {return abs(c1-c2);}
 protected:
  inline location& l_() {return (*this);}
};

class rotation :public matrix
{
 public:
  rotation() : I_MATRIX {}
  rotation(vector w);
  rotation(matrix m) : matrix(m) {}
  rotation(REAL_TYPE w0,REAL_TYPE w1,REAL_TYPE w2);

  rotation& operator%=(vector w)
  {(*this)*=rotation(w);
    return (*this);}
  
  friend rotation& orts(rotation r)
  {return r.r_();}
 protected:
  inline rotation& r_() {return (*this);}
};

typedef rotation orientation;

class basis : public location, public orientation
{
 public:
  basis()                           : location(0,0,0),rotation(){}
  basis(orientation m,location c)   : location(c), rotation(m){}
  basis(location c)                 : location(c), rotation(){}

  location& operator >>=(vector v)
   {
    l_()+=r_()*v;
    return l_();
   }
  location& operator <<=(vector v)
   {l_()-=r_()*v;
    return l_();}
  basis& operator *=(rotation r)
   {r_()*=r;
    l_()=r*l_();
    return(*this);}
  basis operator ()(basis b)
   {return basis(rotation((~r_())*b.r_()),location(~r_()*(b.l_()-l_())));}
  basis operator [](basis b)
   {return basis(rotation(r_()*b.r_()),location(r_()*b.l_()+l_()));}
  vector operator ()(vector v)
   {return ~r_()*v;}
  vector operator [](vector v)
   {return r_()*v;}
  location operator ()(location c1)
   {return ~r_()*(c1-l_());}
  location operator [](location c1)
   {return l_()+r_()*c1;}
  friend basis operator ~(basis b)
   {return basis(rotation(~b.r_()),location(~b.r_()*(-b.l_())));}
};

#endif
