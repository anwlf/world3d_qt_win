
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
/*   Analytic MATH implementation                                           */
/*   A. Volkov, 1991                                                        */
/****************************************************************************/

#include "analytic.h"

/*    vector methods     */
vector& vector::operator +=(vector v)
   {VO(e[k]+=v.e[k]);
    return (*this);}
vector& vector::operator -=(vector v)
   {VO(e[k]-=v.e[k]);
    return (*this);}
vector operator -(vector v)
   {vector rv; VO(rv.e[k]=-v.e[k]); return rv;}
vector operator +(vector v1,vector v2)
   {vector rv; VO(rv.e[k]=v1.e[k]+v2.e[k]); return rv;}
vector operator -(vector v1,vector v2)
   {vector rv; VO(rv.e[k]=v1.e[k]-v2.e[k]); return rv;}
vector operator *(vector v,REAL_TYPE r)
   {vector rv; VO(rv.e[k]=v.e[k]*r); return rv;}
vector operator *(REAL_TYPE r,vector v)
   {vector rv; VO(rv.e[k]=v.e[k]*r); return rv;}
vector operator %(vector v1,vector v2)
   {return vector(v1.e[1]*v2.e[2]-v1.e[2]*v2.e[1],
		  v1.e[2]*v2.e[0]-v1.e[0]*v2.e[2],
		  v1.e[0]*v2.e[1]-v1.e[1]*v2.e[0]);}
vector operator /(vector v,REAL_TYPE r)
   {vector rv; VO(rv.e[k]=v.e[k]/r); return rv;}
REAL_TYPE operator *(vector v1,vector v2)
   {REAL_TYPE vv=0; VO(vv+=(v1.e[k]*v2.e[k])); return vv;}
REAL_TYPE abs(vector v)
   {REAL_TYPE vv=0; VO(vv+=(v.e[k]*v.e[k]));
    if (vv>0)
     return sqrt(vv);
    else
     return vv;}
vector ort(vector v)
   {if (abs(v)!=0) return v/abs(v); else return vector(1,0,0);}
REAL_TYPE vector::normalization()
   {REAL_TYPE n=abs(*this);
    if (n>0)
     {VO(e[k]=e[k]/n);}
    else
     (*this)=vector(1,0,0);
    return n;}
int operator ==(vector v1,vector v2)
{
 int eq=1;
 VO(eq=(eq&&(v1.e[k]==v2.e[k])));
 return eq;
}

/*    matrix methods    */
matrix& matrix::operator*=(matrix m)
   {matrix mm=(*this);
    MO({e[i][j]=0;VO(e[i][j]+=(mm.e[i][k]*m.e[k][j]))});
    return (*this);}
matrix operator -(matrix m1,matrix m2)
   {matrix rm; MO(rm.e[i][j]=m1.e[i][j]-m2.e[i][j]); return rm;}
matrix operator +(matrix m1,matrix m2)
   {matrix rm; MO(rm.e[i][j]=m1.e[i][j]+m2.e[i][j]); return rm;}
matrix operator *(matrix m,REAL_TYPE r)
   {matrix rm; MO(rm.e[i][j]=m.e[i][j]*r); return rm;}
matrix operator *(REAL_TYPE r,matrix m)
   {matrix rm; MO(rm.e[i][j]=m.e[i][j]*r); return rm;}
matrix operator /(matrix m,REAL_TYPE r)
   {matrix rm; MO(rm.e[i][j]=m.e[i][j]/r); return rm;}
matrix operator *(matrix m1,matrix m2)
   {matrix rm;
    MO({rm.e[i][j]=0;VO(rm.e[i][j]+=(m1.e[i][k]*m2.e[k][j]))});
    return rm;}

vector operator *(matrix m,vector v)
   {vector rv;
    for (int i=0;i<3;i++)
     {rv.e[i]=0;
      VO(rv.e[i]+=(v.e[k]*m.e[i][k]));}
    return rv;}

matrix operator ~(matrix m)
   {matrix rm; MO(rm.e[i][j]=m.e[j][i]); return rm;}

rotation::rotation(vector w)
{vector el=w;
 REAL_TYPE d=el.normalization(),
	   sd=sin(d),cd=cos(d),ecd=1-cd;
  if (d>0)
   {
    e[0][1]=-sd*el[2];
    e[1][0]=sd*el[2];
    e[1][2]=-sd*el[0];
    e[2][1]=sd*el[0];
    e[2][0]=-sd*el[1];
    e[0][2]=sd*el[1];
    MO(e[i][j]=((i==j)?cd+ecd*el[i]*el[j]:
		       ecd*el[i]*el[j]+e[i][j]));
   }
  else
   MO(e[i][j]=((i==j)?1:0));
 }

int operator ==(matrix m1,matrix m2)
{
 int eq=1;
 MO(eq=(eq&&(m1.e[i][j]==m2.e[i][j])));
 return eq;
}

rotation::rotation(REAL_TYPE w0,REAL_TYPE w1,REAL_TYPE w2)
   {REAL_TYPE ca=cos(w0),sa=sin(w0),
	      cb=cos(w1),sb=sin(w1),
	      cg=cos(w2),sg=sin(w2);
	   e[0][0]=cb*cg         ;e[0][1]=-cb*sg        ;e[0][2]=sb    ;
	   e[1][0]=sa*sb*cg+ca*sg;e[1][1]=ca*cg-sa*sb*sg;e[1][2]=-sa*cb;
	   e[2][0]=sa*sg-ca*sb*cg;e[2][1]=ca*sb*sg+sa*cg;e[2][2]=ca*cb ;
   }


