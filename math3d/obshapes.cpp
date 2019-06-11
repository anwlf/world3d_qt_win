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
/*   SHAPES  implementation                                                 */
/*   A. Volkov, 1991                                                        */
/****************************************************************************/

#include <stdarg.h>
#include "obshapes.h"
#include "eventcon.h"

shape_framework::shape_framework(int v_c)
{
   vertexs_count = v_c;
   vertexs = new location*[vertexs_count];   //?
   for (int i=0; i<vertexs_count; i++) {
    vertexs[i]=new location;
   }
   v_proections = new point_proection[vertexs_count];
}

shape_framework::~shape_framework() { //12.31.2015
  delete vertexs;
  delete v_proections;
}

point_proection* shape_framework::proect(REAL_TYPE size,basis v_b,
                                         video_params* v_p)
{
 for (int i=0; i<vertexs_count; i++)
  {
   _EVENT_H;
   location vrtx=(*vertexs[i])*size;
   vrtx=v_b(vrtx);
   //REAL_TYPE d=abs(vrtx); //vrtx[2]
   //if (d!=0)  				    // ?????
   //REAL_TYPE d=abs(vrtx[2]); //??
   REAL_TYPE d=vrtx[2]; //??
   if (d>0)
   {
       v_proections[i][0]=v_p->max_x/2+v_p->scale*(vrtx[0]/d);
       v_proections[i][1]=v_p->max_y/2-v_p->scale*v_p->aspect_ratio*(vrtx[1]/d);
   }
   else return NULL;
  };
  return v_proections;
}

location& shape_framework::operator[](int v)
{
 return (*vertexs[v]);
}

contour::contour(int v_c, int* v_n)
{
 vertexs_count=v_c;
 vertexs_nums=v_n;
}

contour::contour(int v_c, ...)
{
 vertexs_count = v_c;
 vertexs_nums = new int[vertexs_count];
 va_list ap;
 va_start(ap,v_c);
 for (int i=0; i<vertexs_count; i++)
  vertexs_nums[i] = va_arg(ap, int);
 va_end(ap);
}

contour::~contour() { //12.31.2015
  //if (vertexs_nums!=NULL)
  //  delete vertexs_nums; //does not work
}

//int* contour::proection(point_proection* v_p)
PROJECTION_TYPE* contour::proection(point_proection* v_p) //02.28.2016
{
 //point_proection *v_proections = new point_proection[vertexs_count];
 //point_proection *v_proections = (point_proection*)new int[vertexs_count*2];    //test 02.24.2016
 point_proection *v_proections = (point_proection*)new PROJECTION_TYPE[vertexs_count*2];
 for (int i=0; i<vertexs_count; i++)
  {
   v_proections[i][0] = v_p[vertexs_nums[i]][0];
   v_proections[i][1] = v_p[vertexs_nums[i]][1];
  }
 //return (int*) v_proections;
 return (PROJECTION_TYPE*)v_proections; //02.28.2016
}

/*  facet methods  */

void facet::set_normal(int p1, int p2, int p3)
   // Set normal according 3 basic points                       ????
{
 facet_normal =
  ort(((*framework)[p2]-(*framework)[p1])%
      ((*framework)[p3]-(*framework)[p1]));
}

void facet::set_normal()
   // Set normal according 3 first points      ???
{
// if Point1=Point2 or Point0 ????????????????????
 int p0=0,p1=1,p2=2;
 while (((*framework)[facet_contour[p1]]==(*framework)[facet_contour[p0]])
         &&(p1<facet_contour.count()))
  p1++;
 while (((*framework)[facet_contour[p2]]==(*framework)[facet_contour[p1]])
         &&(p2<facet_contour.count()))
  p2++;
 facet_normal =
  ort(((*framework)[facet_contour[p1]]-(*framework)[facet_contour[p0]])%
      ((*framework)[facet_contour[p2]]-(*framework)[facet_contour[p0]]));
}

/*void facet::set_gradient(int* p1, int* p2, REAL_TYPE v1, REAL_TYPE v2) //added 02.03.2016
{
    gr = new linear_gradient(p1,p2);
    (*gr)[0]=v1;
    (*gr)[1]=v2;
}*/

REAL_TYPE facet::light_lit(vector& l_v)
   // Return intensivity of facet lit
{
 vector l=l_v;
 l.normalization();
 return facet_normal*l; //l_v
}

REAL_TYPE facet::distance(basis v_b)
   // Return distance to facet's far point
{
 REAL_TYPE d=0,r;
 int c=facet_contour.count();
 for (int i=0; i<c; i++)
  {
   r=abs(v_b((*framework)[facet_contour[i]]));
   if (r>d) d=r;
  };
  return d;
}

int facet::is_visible(REAL_TYPE size, basis v_b /* , video_params* v_p */)
   /* Return 0 if not visible */
{
 if (ort(v_b-size*(*this)[0])*facet_normal>0)   /* Primitive */
  {
   for (int i=0; i<facet_contour.count(); i++)
    if (v_b((*this)[i])[2]<0) return 0;
   return 1;
  }
 else return 0;
}

//int* facet::proection()
PROJECTION_TYPE* facet::proection() //02.28.2016
{
 return facet_contour.proection(framework->proection());
}

/*  facet_shape methods */
//Changed 01.23.2016 - colors array to single color
/*
image* facet_shape::create_image(int *f_c, REAL_TYPE size, basis v_b,
                                 vector l_v, video_params* v_p)
{
 framework->proect(size,v_b,v_p);
 image* im=new(image);
 facet* f=facets;
 int i=0;
 while (f!=NULL)
  {
   _EVENT_H;
   if (f->is_visible(size,v_b / *,v_p* /))
    (*im)+=f->create_image(f_c[i],v_b,l_v);
   f=f->next;
   i++;
  };
 return im;
}
*/

image* facet_shape::create_image(int f_c, REAL_TYPE size, basis v_b,
                                 vector l_v, video_params* v_p)
{
 framework->proect(size,v_b,v_p);
 image* im=new(image);
 facet* f=facets;
 int i=0;
 while (f!=NULL)
  {
   _EVENT_H;
   if (f->is_visible(size,v_b /*,v_p*/)) {
       this->calculate(f,l_v);                  // moved here 02.08.2016
       (*im)+=f->create_image(f_c,v_b,l_v);
       //this->calculate(f,l_v);
   }
   f=f->next;
   i++;
  };
 return im;
}

//void facet_shape::add_facet(contour c, gradient* g) //02.03.2016
facet* facet_shape::add_facet(contour c) //02.03.2016
{
 facet *f = new facet(framework,c); //??
 f->set_normal();
 facets_count++;

 if (facets==NULL)
  facets = f;
 else
  last_facet->next = f;

 last_facet = f;
 return f;//02.03.2016
}
