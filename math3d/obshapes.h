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
/*   SHAPES header                                                          */
/*   A. Volkov, 1991                                                        */
/****************************************************************************/
/*   Updated: A. Volkov, 2016 - added shape class and included              */
/*                              virtual keyword to the create_image         */
/****************************************************************************/

#ifndef __O_SHAPES
#define __O_SHAPES

#include "analytic.h"
#include "obimages.h"

class shape_framework
{
  int vertexs_count;
  location** vertexs;

  public:
   shape_framework(int v_c);
   ~shape_framework();
   inline int count() {return vertexs_count;}
   point_proection* proect(REAL_TYPE size, basis v_b,video_params* v_p);
   inline point_proection* proection() {return v_proections;}
   location& operator [](int v);

  protected:
   point_proection* v_proections;
};

class contour
{
 int vertexs_count;
 int* vertexs_nums;

 public:
  contour() {}
  contour(int v_c, int *v_n);
  contour(int v_c ...);
  ~contour();
  inline int count() {return vertexs_count;}
  inline int operator[](int n) {return vertexs_nums[n];}
  //int* proection(point_proection* v_p);
  PROJECTION_TYPE* proection(point_proection* v_p); //02.28.2016
};

class facet
 {
   contour facet_contour;
   vector facet_normal;
   shape_framework* framework;
   //added for test 02.01.2016
   REAL_TYPE height; //added 02.03.2016 not used
  protected:
   int gradient_points[2];
  public:
   int num;
   gradient* facet_gradient;
   facet *next;
   facet(shape_framework* s_f, contour f_c)
   {framework=s_f; facet_contour = f_c; next = NULL; height=0; facet_gradient=NULL; num=-1;}
   void set_normal(int p1, int p2, int p3);
   /* Set normal according 3 basic points */
   void set_normal();
   /* Set normal according 3 first points ??? */
   //void set_gradient(int* p1, int* p2, REAL_TYPE v1, REAL_TYPE v2);//added 02.03.2016
   //void set_gradient();
   REAL_TYPE light_lit(vector& l_v);
   /* Return intensivity of facet lit */
   REAL_TYPE distance(basis v_b);
   /* Return distance to facet's far point */
   int is_visible(REAL_TYPE size, basis v_b );
   /* Return 0 if not visible */
   //int* proection();
   PROJECTION_TYPE* proection();
   /*
   poly_image* create_image(int c, basis v_b, vector l_v)
    {poly_image* p = new poly_image(c,
                                    facet_contour.count(),
                                    proection(),
                                    NULL,
                                    distance(v_b),
                                    light_lit(l_v));
    return p;}*/
   poly_image* create_image(int c, basis v_b, vector l_v)
    {
       //int* pr = proection();
       PROJECTION_TYPE* pr = proection(); //02.28.2016
       poly_image* p;
       //
       if (facet_contour.count()>2) {
        //gr = new linear_gradient(&pr[0],&pr[facet_contour.count()-2]);
        //set_gradient(&pr[0],&pr[facet_contour.count()-2],1.0,0.5);
        //set_gradient();
        p = new gradient_poly_image(c,
                                    facet_contour.count(),
                                    //proection(),
                                    pr,
                                    NULL,
                                    distance(v_b),
                                    light_lit(l_v),
                                    facet_gradient);
       } else {
        p = new poly_image(c,
                           facet_contour.count(),
                           pr,//proection(),
                           NULL,
                           distance(v_b),
                           light_lit(l_v)
                           );

       }
    return p;}
   inline location operator [](int v)
    {return (*framework)[facet_contour[v]];}
 };

//Added 01.22.2016
class shape
{
  protected:
    //virtual void calculate() {}
  public:
    //Changed 01.23.2016 - colors array to single color
    //virtual image* create_image(int *f_c, REAL_TYPE size, basis v_b,
    //                            vector l_v, video_params* v_p) {return NULL;}
    virtual image* create_image(int f_c, REAL_TYPE size, basis v_b,
                                vector l_v, video_params* v_p) {f_c=f_c; size=size;v_b=v_b; l_v=l_v; v_p=v_p; return NULL;}
};

class facet_shape: public shape     /* in local world light and videobasis */ //public shape added 01.22.2016
{                                   /* all videoproections                 */
 protected:
  shape_framework* framework;
  int facets_count;
  facet* facets;
  facet* last_facet;
  virtual void calculate(facet* f, vector l_v) {f=f;l_v=l_v;}
 public:
  facet_shape() {framework = NULL; facets_count = 0; facets = NULL;}
  facet_shape(shape_framework& s_f)
        {framework = &s_f; facets_count = 0; facets = NULL;}
  int count()
   {return facets_count;}
  //void add_facet(contour c, gradient* g); //01.03.2016
  facet* add_facet(contour c); //01.03.2016
/*  int is_visible(REAL_TYPE size, basis v_b, video_params* v_p); ??? */
  //Changed 01.23.2016 - colors array to single color
  //virtual image* create_image(int *f_c, REAL_TYPE size, basis v_b,
  //                            vector l_v, video_params* v_p);
  virtual image* create_image(int f_c, REAL_TYPE size, basis v_b,
                              vector l_v, video_params* v_p);
};

#endif
