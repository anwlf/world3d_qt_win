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
/*   IMAGES header                                                          */
/*   A. Volkov, 1991,2016                                                   */
/****************************************************************************/

#ifndef __O_IMAGES
#define __O_IMAGES

#define PROJECTION_TYPE REAL_TYPE //02.28.2016 moved here

#include <stddef.h>
#include "videodev.h"
#include "analytic.h"

//#define PROJECTION_TYPE REAL_TYPE

class video_system;

//typedef int point_proection[2];
typedef PROJECTION_TYPE point_proection[2]; //02.28.2016

class part_image
{
  public:
    part_image* next;
    REAL_TYPE distance;//, fill;
    virtual ~part_image() {}
    virtual void draw(video_system* v_s) {v_s=v_s;} //Abstract
};

//01.23.2016
//class points_image; //forward
class _point
{
  //friend class points_image;
  //protected:
  //  int color,x,y;
  public:
    //int color,x,y;
    int color; PROJECTION_TYPE x,y;                       //02.28.2016
    _point() {}
    //_point(int c, int _x, int _y)
    _point(int c, PROJECTION_TYPE _x, PROJECTION_TYPE _y) //02.28.2016
    {
      color=c; x=_x; y=_y;
    }
};

class v_point
{
  //friend class points_image;
  //protected:
  //  int color,x,y;
  public:
    //int color,x,y;
    int color; PROJECTION_TYPE  x,y;      //02.28.2016
    REAL_TYPE value;
    v_point() {}
    //v_point(int c, int _x, int _y, REAL_TYPE v)
    v_point(int c, PROJECTION_TYPE _x, PROJECTION_TYPE _y, REAL_TYPE v) //02.28.2016
    {
      color=c; x=_x; y=_y; value=v;
    }
};

//01.23.2016 - added _point inheritance
class point_image: public part_image, public _point
{
 public:
  //int color,x,y; //commented 01.23.2016
  //point_image(int c, int _x, int _y,
  point_image(int c, PROJECTION_TYPE _x, PROJECTION_TYPE _y, //02.28.2016
             part_image* _next,
             REAL_TYPE d) : _point(c, _x,_y)
  {
   //color=c; x=_x; y=_y;
   next=_next;
   distance=d;
  }
  virtual void draw(video_system* v_s);
};

//01.23.2016 - added
class points_image: public part_image
{
  protected:
    v_point* points;
    int count;
    int color;
    //virtual void calculate() {}
  public:
    points_image(int c, int cnt, part_image* _next, REAL_TYPE d) {
      color = c;
      count = cnt;
      if (count>0) points = new v_point[count];
      else points=NULL;
      next=_next;
      distance=d;
    }
    v_point& operator [](int i) {return points[i];}
    virtual ~points_image() { if (points) delete points; }
    virtual void draw(video_system* v_s);
};

//12.27.2015
class lock_image: public part_image
{
 public:
  int color,x,y,r;
  //lock_image(int c, int _x, int _y, int _r,
  lock_image(int c, PROJECTION_TYPE _x, PROJECTION_TYPE _y, PROJECTION_TYPE _r, //02.28.2016
             part_image* _next,
             REAL_TYPE d)
  {
   color=c; x=_x; y=_y; next=_next;r=_r;
   distance=d;
  }
  virtual void draw(video_system* v_s);
};

//12.08.2015
class circle_image: public part_image
{
 public:
  //int color,x,y,r;
  int color; PROJECTION_TYPE x,y,r; //02.28.2016
  //circle_image(int c, int _x, int _y, int _r,
  circle_image(int c, PROJECTION_TYPE _x, PROJECTION_TYPE _y, PROJECTION_TYPE _r, //02.28.2016
             part_image* _next,
             REAL_TYPE d)
  {
   color=c; x=_x; y=_y; next=_next;r=_r;
   distance=d;
  }
  virtual void draw(video_system* v_s);
};

class poly_image: public part_image
{
 public:
  int color, num_points;
  //int* poly_points;
  //int* poly_points;
  PROJECTION_TYPE* poly_points; //02.28.2016
  //poly_image* next;
  REAL_TYPE /*distance,*/ value;

  //poly_image(int c, int n_p, int* p_p,
  poly_image(int c, int n_p, PROJECTION_TYPE* p_p,
             part_image* _next,
             REAL_TYPE d, REAL_TYPE v)
  {
   color=c; num_points=n_p; poly_points=p_p; next=_next;
   distance=d; value=v;
  }
  virtual ~poly_image() {/*delete poly_points;*/}
  virtual void draw(video_system* v_s);
};

//added 02.01.2016
enum gradient_type {none,linear,conical,radial,custom};
class gradient
{
    protected:
        REAL_TYPE* intens_values;
        int values_num;
    public:
        REAL_TYPE transp;
        gradient() {intens_values=NULL;values_num=0; transp=1.0;}
        gradient(int n) {intens_values=new REAL_TYPE[n];values_num=n; transp=1.0;}
        virtual
        ~gradient() {if (intens_values!=NULL) delete[] intens_values;}
        virtual gradient_type type() {return none;}
        REAL_TYPE& operator[](int i) {return intens_values[i];}
};

class linear_gradient : public gradient
{
    protected:        
    public:
        //int start_x;  //start point x
        PROJECTION_TYPE start_x;  //start point x 02.28.2016
        //int start_y;
        PROJECTION_TYPE start_y; //02.28.2016
        //int stop_x;   //stop point x
        PROJECTION_TYPE stop_x;   //stop point x 02.28.2016
        //int stop_y;
        PROJECTION_TYPE stop_y; //02.28.2016
        linear_gradient() {}
        //linear_gradient(int s_x, int s_y, int e_x, int e_y) : gradient(2) {start_x=s_x; start_y=s_y;stop_x=e_x;stop_y=e_y;}
        linear_gradient(PROJECTION_TYPE s_x, PROJECTION_TYPE s_y, PROJECTION_TYPE e_x, PROJECTION_TYPE e_y) : gradient(2) {start_x=s_x; start_y=s_y;stop_x=e_x;stop_y=e_y;} //02.28.2016
        virtual gradient_type type() {return linear;}
};

class conical_gradient : public gradient
{
    public:
        //int center_x; //center point x
        PROJECTION_TYPE center_x; //center point x 02.28.2016
        //int center_y; //center point x
        PROJECTION_TYPE center_y; //center point x 02.28.2016
        REAL_TYPE angle;
        REAL_TYPE stop_angle;
        conical_gradient() {}
        //conical_gradient(int c_x, int c_y) : gradient(3) {center_x=c_x; center_y=c_y; angle=0; stop_angle=0;}
        conical_gradient(PROJECTION_TYPE c_x, PROJECTION_TYPE c_y) : gradient(3) {center_x=c_x; center_y=c_y; angle=0; stop_angle=0;} //02.29.2016
        virtual gradient_type type() {return conical;}
};

class radial_gradient : public gradient
{
    protected:
    public:
        PROJECTION_TYPE center_x;  //center point x
        PROJECTION_TYPE center_y;
        PROJECTION_TYPE center_r;
        REAL_TYPE value_2_point;
        /*
        int focal_x;   //focal point x
        int focal_y;
        int focal_r;
        */
        radial_gradient() {}
        //radial_gradient(int c_x, int c_y, int c_r, int f_x, int f_y, int f_r) : gradient(3) {center_x=c_x; center_y=c_y; center_r=c_r; focal_x=f_x;focal_y=f_y; focal_r=f_r;}
        radial_gradient(PROJECTION_TYPE c_x, PROJECTION_TYPE c_y, PROJECTION_TYPE c_r, REAL_TYPE v2p) : gradient(2) {center_x=c_x; center_y=c_y; center_r=c_r; value_2_point=v2p;}
        virtual gradient_type type() {return radial;}
};

class custom_gradient : public gradient
{
    public:
      gradient* grad[2];
      custom_gradient() {grad[0]=grad[1]=NULL;}
      //~custom_gradient() {if (grad[0]!=NULL) delete grad[0]; if (grad[1]!=NULL) delete grad[1];}
      virtual gradient_type type() {return custom;}
};

class gradient_poly_image: public poly_image
{
    public:
        gradient* grad;
        //gradient_poly_image(int c, int n_p, int* p_p,
        gradient_poly_image(int c, int n_p, PROJECTION_TYPE* p_p, //02.28.2016
                            part_image* _next,
                            REAL_TYPE d, REAL_TYPE v, gradient* g) : poly_image(c,n_p,p_p,_next,d,v)
        {
            grad=g;
        }
        virtual void draw(video_system* v_s);
        virtual ~gradient_poly_image() {if (grad!=NULL) delete grad; delete[] poly_points;} //02.11.2016 - delete in the draw
};

//

class image: public part_image
{
 //poly_image* image_elements;
  part_image* image_elements;
 public:
  image() {image_elements = NULL; next=NULL;}
  virtual ~image();

  virtual void draw(video_system* v_s);
  void clear();
  //image& operator +=(poly_image* p_i);
  image& operator +=(part_image* p_i);
  image& operator +=(image* p_i); //uncommented 1.2.2015
};

#endif
