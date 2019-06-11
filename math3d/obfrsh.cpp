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
/*   Facet ROTATION shapes implementation				    */
/*   A. Volkov, 1991							    */
/****************************************************************************/

#include "obfrsh.h"
#include "eventcon.h"

#include <stdio.h>  //02.08.2016 for test, printf

//Line Cross Routine - tmp from C# 02.11.2016

//#define bool int
//#define true 1
//#define false 0

bool LineCrossPoint(double x11, double y11, double x12, double y12, double x21, double y21, double x22, double y22, double& x, double& y, bool& one_line)
        {
            double m1=0, m2=0, a1=0, a2=0;
            bool is_m1 = false, is_m2 = false;
            one_line = false;
            if (x11 != x12)
            {
                m1 = ((y12 - y11) / (x12 - x11));
                a1 = -m1 * x11 + y11;
                is_m1 = true;
            }
            if (x21 != x22)
            {
                m2 = ((y22 - y21) / (x22 - x21));
                a2 = -m2 * x21 + y21;
                is_m2 = true;
            }
            if (is_m1 && is_m2 && (m1 != m2)) //simplest cross point
            {
                //m1*x + a1 = m2*x + a2; x= (a2-a1)/(m1-m2); y = m1*x + a1;
                x = (a2 - a1) / (m1 - m2);
                y = m1 * x + a1;
                return true;
            }
            else if (is_m1 && is_m2)
            {
                if (a1 == a2)
                {
                    return one_line = true;
                }
            }
            else if (is_m2)
            {
                x = x11;
                y = (m2 * x) + a2;
                return true;
            }

            else if (is_m1)
            {
                x = x21;
                y = (m1 * x) + a1;
                return true;
            }
            else
            {
                if (x11 == x21)
                {
                    return one_line = true;
                }
            }
            return false;
        }

void perpendicular(PROJECTION_TYPE x0, PROJECTION_TYPE y0, PROJECTION_TYPE x1, PROJECTION_TYPE y1, PROJECTION_TYPE x_from, PROJECTION_TYPE y_from, PROJECTION_TYPE& x, PROJECTION_TYPE& y)
{
	PROJECTION_TYPE dx = x1 - x0, dy = y1 - y0;
	PROJECTION_TYPE lenLine2 = dx  * dx + dy * dy;
	PROJECTION_TYPE partDiv = ((x_from - x0) * dx + (y_from - y0) * dy) / lenLine2;
	x = (x0 + partDiv * (x1 - x0));
	y = (y0 + partDiv * (y1 - y0));
}

inline REAL_TYPE x_angle_rad(REAL_TYPE x, REAL_TYPE y=0)
{
    REAL_TYPE cos_a=x/sqrt((pow(x,2)+pow(y,2)));
    return (y>0)?2.0*M_PI-acos(cos_a):acos(cos_a);
}

//

void rotation_shape::set_shape(int koeff, int line_point_c, location* p,
                   int Up,int Down/*, gradient_type gt, int* g_points_nums*/)
{
 basis self;
 framework = new shape_framework(koeff*line_point_c);

 int i,j;

 for (i=0; i<koeff; i++)
  {
   _EVENT_H;
   for (j=0; j<line_point_c; j++)
    (*framework)[i*line_point_c+j] = self(p[j]);
   self%=vector(0,2*M_PI/koeff,0);
  };

 r_facet = new facet*[koeff*line_point_c];
 coefficient=koeff;
 line_point_count=line_point_c;

 for (i=0; i<koeff; i++)
  for (j=0; j<(line_point_c-1); j++)
   {
    _EVENT_H;
    facet* f=add_facet(contour(5,   i*line_point_c+j,
			  (i*line_point_c+j+1),
			 ((i+1)*line_point_c+j+1)%(koeff*line_point_c),
			 ((i+1)*line_point_c+j)%(koeff*line_point_c),
			   i*line_point_c+j));
    f->num=i*line_point_c+j;
    r_facet[f->num]=f;
    /*
    gradient* gr = NULL;
    switch (gt) {
     case linear: gr = new linear_gradient(); break;
     case conical: gr = new conical_gradient(); break;
     case radial: gr = new radial_gradient(); break;
    }
    f->grad = gr;
    */
   };
/* Up facet */
   if (Up)
    {
     int *up_p_n = new int[koeff];
     for (i=0; i<koeff; i++)
      up_p_n[koeff-i-1] = (i*line_point_c+line_point_c-1)%(koeff*line_point_c);
     facet* f=add_facet(contour(koeff,up_p_n));
     up_facet = f;
    };

/* Down facet */
   if (Down)
    {
     int *down_p_n = new int[koeff];
     for (i=0; i<koeff; i++) down_p_n[i] = (i*line_point_c)%(koeff*line_point_c);
     facet* f=add_facet(contour(koeff,down_p_n));
     down_facet = f;
    };
}

rotation_shape::rotation_shape(int koeff, int line_point_c, location* p)
{
    up_facet=NULL;
    down_facet=NULL;
    //r_facet = new facet*[koeff*line_point_c];
    coefficient=0;
    line_point_count=0;
    set_shape(koeff,line_point_c,p,1,1/*,none,NULL*/);
}

rotation_shape::~rotation_shape()
{
    delete[] r_facet;
}

cube::cube(REAL_TYPE h)
{
 location c_l[2];
 c_l[0]=location(h,-h,h);
 c_l[1]=location(h, h,h);
 set_shape(4,2,c_l,1,1/*,none,NULL*/);
}

cylinder::cylinder(int r_k, REAL_TYPE h,REAL_TYPE r)
{
 location c_l[2];
 c_l[0]=location(r,-h,0);
 c_l[1]=location(r, h,0);
 //int points[2] = {0,1};
 set_shape(r_k,2,c_l,1,1/*,linear,points*/);
 gradient_points_numbers[0]=0;
 gradient_points_numbers[1]=1;
 gradient_points_numbers[2]=3;
 gradient_points_numbers[3]=2;
}

void cylinder::calculate(facet* f, vector l_v) //02.03.2016
{
    if ((f->next!=NULL)&&(f->next->next!=NULL)) {
        //find prev  02.06.2016
        /*
        facet* fp=this->facets;
        if (fp==f) {
          while (fp->next->next->next!=NULL) {
            fp=fp->next;
          }
        } else {
          while (fp->next!=f) {
            fp=fp->next;
          }
        }
        */
        //int* pr = f->proection();
        PROJECTION_TYPE* pr = f->proection(); //02.28.2016
        PROJECTION_TYPE x0=pr[gradient_points_numbers[0]*2];
        PROJECTION_TYPE y0=pr[gradient_points_numbers[0]*2+1];
        PROJECTION_TYPE x1=pr[gradient_points_numbers[1]*2];
        PROJECTION_TYPE y1=pr[gradient_points_numbers[1]*2+1];
        PROJECTION_TYPE x2=pr[gradient_points_numbers[2]*2];
        PROJECTION_TYPE y2=pr[gradient_points_numbers[2]*2+1];
        PROJECTION_TYPE x3=pr[gradient_points_numbers[3]*2];
        PROJECTION_TYPE y3=pr[gradient_points_numbers[3]*2+1];
        PROJECTION_TYPE start_x=(x0+x1)/2;
        PROJECTION_TYPE start_y=(y0+y1)/2;
        PROJECTION_TYPE stop_x,stop_y;
        //if (f->facet_gradient) delete f->facet_gradient;
        perpendicular(x2, y2, x3, y3, start_x, start_y, stop_x, stop_y);
        f->facet_gradient = new linear_gradient(start_x,start_y,stop_x,stop_y);
        //printf("(%i,%i),(%i,%i)\r\n",start_x,start_y,stop_x,stop_y);
        REAL_TYPE l1=f->light_lit(l_v);//fp->light_lit(l_v);
        REAL_TYPE l2=(f->next->next->next!=NULL)?f->next->light_lit(l_v):this->facets->light_lit(l_v);
        (*f->facet_gradient)[0]=l1;
        (*f->facet_gradient)[1]=l2;
    }
}

//added 12.30.2010
ring::ring(int r_k, REAL_TYPE h,REAL_TYPE R, REAL_TYPE r)
{
    location c_l[4];
    c_l[0]=location(r,-h,0);
    c_l[1]=location(R,-h,0);
    c_l[2]=location(R, h,0);
    c_l[3]=location(r,-h,0);
    //int points[2] = {1,2};
    set_shape(r_k,4,c_l,0,0/*,linear,points*/);
    gradient_points_numbers[0]=1;
    gradient_points_numbers[1]=2;
}

cone::cone(int r_k, REAL_TYPE h,REAL_TYPE r)
{
 location c_l[2];
 c_l[0]=location(r,-h,0);
 c_l[1]=location(0, h,0);
 //int points[2] = {1,0};
 set_shape(r_k,2,c_l,0,1/*,conical,points*/);
 gradient_points_numbers[0]=1;//2;//1; //2//0//2 //0//2
 gradient_points_numbers[1]=0;//0;//2; //0//2//0 //1//1
 gradient_points_numbers[2]=2;//1;  //02.05.2016
 gradient_points_numbers[3]=3;
}

//#define PI 3.14159265359

void cone::calculate(facet* f, vector l_v)
{
  if (f->next!=NULL) {
    /*find prev  02.06.2016
    facet* fp=this->facets;
    if (fp==f) {
      while (fp->next->next!=NULL) {
        fp=fp->next;
      }
    } else {
      while (fp->next!=f) {
        fp=fp->next;
      }
    }
    */
    //int* pr = f->proection();
    PROJECTION_TYPE* pr = f->proection(); //02.28.2016
    //if (f->facet_gradient) delete f->facet_gradient;
    //((conical_gradient*)f->facet_gradient)->angle=30; //tmp test 02.04.2016
    REAL_TYPE x0=pr[gradient_points_numbers[0]*2];
    REAL_TYPE y0=pr[gradient_points_numbers[0]*2+1];
    REAL_TYPE x1=pr[gradient_points_numbers[1]*2];
    REAL_TYPE y1=pr[gradient_points_numbers[1]*2+1];
    REAL_TYPE x2=pr[gradient_points_numbers[2]*2];
    REAL_TYPE y2=pr[gradient_points_numbers[2]*2+1];
    REAL_TYPE x3=pr[gradient_points_numbers[3]*2];
    REAL_TYPE y3=pr[gradient_points_numbers[3]*2+1];
    //printf("(%f,%f),(%f,%f),(%f,%f),(%f,%f)\r\n",x0,y0,x1,y1,x2,y2,x3,y3);
    f->facet_gradient = new conical_gradient(x0,y0);
    REAL_TYPE x=(x1-x0);
    REAL_TYPE y=(y1-y0);
    //REAL_TYPE cos_a=x/sqrt((pow(x,2)+pow(y,2)));
    //REAL_TYPE a1 = (y>0)?2.0*PI-acos(cos_a):acos(cos_a);
    REAL_TYPE a1 = x_angle_rad(x,y);
    x=(x3-x2);
    y=(y3-y2);
    //cos_a=x/sqrt((pow(x,2)+pow(y,2)));
    //REAL_TYPE a2 = (y>0)?2.0*PI-acos(cos_a):acos(cos_a);
    REAL_TYPE a2 = x_angle_rad(x,y);
    REAL_TYPE l1=f->light_lit(l_v);//fp->light_lit(l_v);
    REAL_TYPE l2=(f->next->next!=NULL)?f->next->light_lit(l_v):this->facets->light_lit(l_v);
    ((conical_gradient*)f->facet_gradient)->angle=a1;//(a1<a2)?a1:a2;
    ((conical_gradient*)f->facet_gradient)->stop_angle=a2;//(a1<a2)?a2:a1;
    (*f->facet_gradient)[0]=l1;//(a1<a2)?l1:l2;
    (*f->facet_gradient)[1]=l2;//(a1<a2)?l2:l1;
  }
}

sphere::sphere(int r_k, REAL_TYPE r)
{
 basis self;
 location *c_l = new location[r_k+1];
 for (int i=0; i<=r_k; i++)
  {
   c_l[i]=self(location(0.001,-r,0));
   self%=vector(0,0,M_PI/r_k);
  };
 set_shape(r_k,r_k+1,c_l,0,0/*,radial,NULL*/);
 delete[] c_l;
 /*
 gradient_points_numbers[0]=0;
 gradient_points_numbers[1]=1;
 gradient_points_numbers[2]=3;
 gradient_points_numbers[3]=2;
 */
 gradient_points_numbers[0]=1;
 gradient_points_numbers[1]=0;
 gradient_points_numbers[2]=2;
 gradient_points_numbers[3]=3;
}

//02.09.2016
void sphere::calculate(facet* f, vector l_v)
{
    if ((f->next!=NULL)&&(f->next->next!=NULL)) {
        //int* pr = f->proection();
        PROJECTION_TYPE* pr = f->proection(); //02.28.2016
        PROJECTION_TYPE x0=pr[gradient_points_numbers[0]*2];
        PROJECTION_TYPE y0=pr[gradient_points_numbers[0]*2+1];
        PROJECTION_TYPE x1=pr[gradient_points_numbers[1]*2];
        PROJECTION_TYPE y1=pr[gradient_points_numbers[1]*2+1];
        PROJECTION_TYPE x2=pr[gradient_points_numbers[2]*2];
        PROJECTION_TYPE y2=pr[gradient_points_numbers[2]*2+1];
        PROJECTION_TYPE x3=pr[gradient_points_numbers[3]*2];
        PROJECTION_TYPE y3=pr[gradient_points_numbers[3]*2+1];
        //printf("(%i,%i),(%i,%i),(%i,%i),(%i,%i):",x0,y0,x1,y1,x2,y2,x3,y3);
        PROJECTION_TYPE c_x=0,c_y=0,c_r=0;
        bool o_l=false;
        bool cross = LineCrossPoint(x0,y0,x1,y1,x2,y2,x3,y3,c_x,c_y,o_l);
        c_r = sqrt(pow(c_x-x0,2)+pow(c_y-y0,2));
        if (c_r!=0) {
          if (cross) {
            f->facet_gradient = new conical_gradient((int)c_x,(int)c_y);
            REAL_TYPE x=(x1-c_x);
            REAL_TYPE y=(y1-c_y);
            REAL_TYPE a1 = x_angle_rad(x,y);
            x=(x2-c_x);
            y=(y2-c_y);
            REAL_TYPE a2 = x_angle_rad(x,y);
            ((conical_gradient*)f->facet_gradient)->angle=a1;
            ((conical_gradient*)f->facet_gradient)->stop_angle=a2;
            //printf(" c(%i,%i),r:%i\r\n",(int)c_x,(int)c_y,(int)c_r);
          } else {
            PROJECTION_TYPE x,y;
            perpendicular(x2, y2, x3, y3, x0, y0, x, y);
            f->facet_gradient = new linear_gradient(x0,y0,x,y);
            //printf("Linear gradient (%i,%i),(%i,%i)\r\n",(int)x0,(int)y0,(int)x3,(int)y3);
          }
          int i = f->num/line_point_count;
          int j = f->num%line_point_count;
          REAL_TYPE l1;
          REAL_TYPE l2;
          //if (j>=line_point_count/2-1) { //02.15.2016
            l1=f->light_lit(l_v);//fp->light_lit(l_v);
            l2=((i+1)<coefficient)?r_facet[(i+1)*line_point_count+j]->light_lit(l_v):r_facet[j]->light_lit(l_v);
            //printf("j up = %i, l1=%f, l2=%f, c_x=%i, c_y=%i\r\n",j,l1,l2,(int)c_x,(int)c_y);
          /*} else {
            l1=(i>0)?r_facet[(i-1)*line_point_count+j]->light_lit(l_v):r_facet[j]->light_lit(l_v);
            l2=f->light_lit(l_v);
            //printf("j down = %i, l1=%f, l2=%f, c_x=%i, c_y=%i\r\n",j,l1,l2,(int)c_x,(int)c_y);
          }*/
          (*f->facet_gradient)[0]=l1;
          (*f->facet_gradient)[1]=l2;
        } else f->facet_gradient = NULL;
    }
}


//added 12.30.2010
ellipsoid::ellipsoid(int r_k, REAL_TYPE r1, REAL_TYPE r2)
{
 location *c_l = new location[r_k];
 for (int i=1; i<=r_k; i++)
  {
     c_l[i-1]=vector(r1*-sin(i*M_PI/(r_k+1)),r2*-cos(i*M_PI/(r_k+1)));
  };
 set_shape(r_k,r_k,c_l,1,1/*,radial,NULL*/);
 delete[] c_l;
 gradient_points_numbers[0]=1;
 gradient_points_numbers[1]=0;
 gradient_points_numbers[2]=2;
 gradient_points_numbers[3]=3;
}


/*02.09.2016
void ellipsoid::calculate(facet* f, vector l_v)
{
    if ((f->next!=NULL)&&(f->next->next!=NULL)) {
        //find prev  02.06.2016
        / *
        facet* fp=this->facets;
        if (fp==f) {
          while (fp->next->next->next!=NULL) {
            fp=fp->next;
          }
        } else {
          while (fp->next!=f) {
            fp=fp->next;
          }
        }
        * /
        int* pr = f->proection();
        int x0=pr[gradient_points_numbers[0]*2];
        int y0=pr[gradient_points_numbers[0]*2+1];
        int x1=pr[gradient_points_numbers[1]*2];
        int y1=pr[gradient_points_numbers[1]*2+1];
        int x2=pr[gradient_points_numbers[2]*2];
        int y2=pr[gradient_points_numbers[2]*2+1];
        int x3=pr[gradient_points_numbers[3]*2];
        int y3=pr[gradient_points_numbers[3]*2+1];
        //printf("(%i,%i),(%i,%i),(%i,%i),(%i,%i):",x0,y0,x1,y1,x2,y2,x3,y3);
        int center_x=(x0+x1)/2;
        int center_y=(y0+y1)/2;
        int focal_x=(x2+x3)/2;
        int focal_y=(y2+y3)/2;
        double c_x=0,c_y=0,c_r=0;
        bool o_l=false;
        //int c_r = sqrt(pow(x1-x0,2)+pow(y1-y2,2));
        bool cross = LineCrossPoint(x0,y0,x1,y1,x2,y2,x3,y3,c_x,c_y,o_l);
        c_r = sqrt(pow(c_x-x0,2)+pow(c_y-y0,2));
        //bool cross = LineCrossPoint(x1,y1,x2,y2,x0,y0,x3,y3,c_x,c_y,o_l);
        //bool cross = LineCrossPoint(x0,y0,x2,y2,x1,y1,x3,y3,c_x,c_y,o_l);
        if (c_r!=0) {
          //f->facet_gradient = new radial_gradient(center_x,center_y,c_r,focal_x,focal_y,0);
          if (cross) {
            //f->facet_gradient = new radial_gradient((int)c_x,(int)c_y,(int)c_r,focal_x,focal_y,(int)c_r);
            //f->facet_gradient = new radial_gradient((int)c_x,(int)c_y,(int)c_r,(int)c_x,(int)c_y,(int)c_r);
            f->facet_gradient = new conical_gradient((int)c_x,(int)c_y);
              //REAL_TYPE x=(x0-c_x);
              //REAL_TYPE y=(y0-c_y);
              REAL_TYPE x=(x1-c_x);
              REAL_TYPE y=(y1-c_y);
              REAL_TYPE a1 = x_angle_rad(x,y);
              //x=(x3-c_x);
              //y=(y3-c_y);
              x=(x2-c_x);
              y=(y2-c_y);
              REAL_TYPE a2 = x_angle_rad(x,y);
              //((conical_gradient*)f->facet_gradient)->angle=(a1<a2)?a1:a2;
              //((conical_gradient*)f->facet_gradient)->stop_angle=(a1<a2)?a2:a1;
              ((conical_gradient*)f->facet_gradient)->angle=a1;//(abs(x1-x0)>abs(x2-x3))?a2:a1;
              ((conical_gradient*)f->facet_gradient)->stop_angle=a2;//(abs(x1-x0)>abs(x2-x3))?a1:a2;
              //printf(" c(%i,%i),r:%i\r\n",(int)c_x,(int)c_y,(int)c_r);
          } else {
            f->facet_gradient = new linear_gradient(center_x,center_y,focal_x,focal_y);
          }
          //printf("(%i,%i),(%i,%i)\r\n",start_x,start_y,stop_x,stop_y);
          //REAL_TYPE l1=f->light_lit(l_v);//fp->light_lit(l_v);
          //REAL_TYPE l2=(f->next->next->next==NULL)?this->facets->light_lit(l_v):f->next->light_lit(l_v);
          //REAL_TYPE l2=(f->next->next->next!=NULL)?f->next->light_lit(l_v):this->facets->light_lit(l_v);
          int i = f->num/line_point_count;
          int j = f->num%line_point_count;
          REAL_TYPE l1; //=f->light_lit(l_v);//fp->light_lit(l_v);
          REAL_TYPE l2;
          //if (j>=line_point_count/2-1) { //02.15.2016
            l1=f->light_lit(l_v);//fp->light_lit(l_v);
            l2=((i+1)<coefficient)?r_facet[(i+1)*line_point_count+j]->light_lit(l_v):r_facet[j]->light_lit(l_v);
            //printf("j up = %i, l1=%f, l2=%f, c_x=%i, c_y=%i\r\n",j,l1,l2,(int)c_x,(int)c_y);
          / *} else {
            l1=(i>0)?r_facet[(i-1)*line_point_count+j]->light_lit(l_v):r_facet[j]->light_lit(l_v);//f->light_lit(l_v);//fp->light_lit(l_v);
            l2=f->light_lit(l_v);//((i-1)<coefficient)?r_facet[(i-1)*line_point_count+j]->light_lit(l_v):r_facet[j]->light_lit(l_v);
            //printf("j down = %i, l1=%f, l2=%f, c_x=%i, c_y=%i\r\n",j,l1,l2,(int)c_x,(int)c_y);
          }* /
          //if (j>line_point_count/2) {
            (*f->facet_gradient)[0]=l1;
            (*f->facet_gradient)[1]=l2;
          //} else {
          //    (*f->facet_gradient)[0]=l2;
          //    (*f->facet_gradient)[1]=l1;
          //}
        } else f->facet_gradient = NULL;
    }
}
*/

//02.24.2016
void ellipsoid::calculate(facet* f, vector l_v)
{
    if ((f->next!=NULL)&&(f->next->next!=NULL)) {
        //int* pr = f->proection();
        PROJECTION_TYPE* pr = f->proection(); //02.28.2016
        int x0=pr[gradient_points_numbers[0]*2];
        int y0=pr[gradient_points_numbers[0]*2+1];
        int x1=pr[gradient_points_numbers[1]*2];
        int y1=pr[gradient_points_numbers[1]*2+1];
        int x2=pr[gradient_points_numbers[2]*2];
        int y2=pr[gradient_points_numbers[2]*2+1];
        int x3=pr[gradient_points_numbers[3]*2];
        int y3=pr[gradient_points_numbers[3]*2+1];
        //printf("(%i,%i),(%i,%i),(%i,%i),(%i,%i):",x0,y0,x1,y1,x2,y2,x3,y3);
        int center_x=(x0+x1)/2;
        int center_y=(y0+y1)/2;
        int focal_x=(x2+x3)/2;
        int focal_y=(y2+y3)/2;
        double c_x=0,c_y=0,c_r=0;
        bool o_l=false;
        bool cross = LineCrossPoint(x0,y0,x1,y1,x2,y2,x3,y3,c_x,c_y,o_l);
        //c_r = sqrt(pow(c_x-x0,2)+pow(c_y-y0,2));
        c_r = sqrt(pow(c_x-center_x,2)+pow(c_y-center_y,2));
        if (c_r!=0) {
          f->facet_gradient = new custom_gradient();
          //conical_
              gradient* grad;
          if (cross) {
              grad = ((custom_gradient*)f->facet_gradient)->grad[0] = new conical_gradient((int)c_x,(int)c_y);
              REAL_TYPE x=(x1-c_x);
              REAL_TYPE y=(y1-c_y);
              REAL_TYPE a1 = x_angle_rad(x,y);
              x=(x2-c_x);
              y=(y2-c_y);
              REAL_TYPE a2 = x_angle_rad(x,y);
              ((conical_gradient*)grad)->angle=a1;//(abs(x1-x0)>abs(x2-x3))?a2:a1;
              ((conical_gradient*)grad)->stop_angle=a2;//(abs(x1-x0)>abs(x2-x3))?a1:a2;
              //printf(" c(%i,%i),r:%i\r\n",(int)c_x,(int)c_y,(int)c_r);
          } else {
            PROJECTION_TYPE x,y;
            perpendicular(x2,y2,x3,y3,center_x,center_y,x,y);
            grad = ((custom_gradient*)f->facet_gradient)->grad[0] = new linear_gradient(center_x,center_y,x,y);
          }
          int i = f->num/line_point_count;
          int j = f->num%line_point_count;
          REAL_TYPE l1; //=f->light_lit(l_v);//fp->light_lit(l_v);
          REAL_TYPE l2;
          l1=f->light_lit(l_v);
          l2=((i+1)<coefficient)?r_facet[(i+1)*line_point_count+j]->light_lit(l_v):r_facet[j]->light_lit(l_v);
          //(*f->facet_gradient)[0]=l1;
          //(*f->facet_gradient)[1]=l2;
          (*grad)[0]=l1;
          (*grad)[1]=l2;
          //cross = false; //test 02.28.2016
          if (cross) {
            REAL_TYPE xx = (center_x + focal_x)/2.0;
            REAL_TYPE yy = (center_y + focal_y)/2.0;
            REAL_TYPE d_x = xx - (REAL_TYPE)c_x;
            REAL_TYPE d_y = yy - (REAL_TYPE)c_y;
            REAL_TYPE c_rad =  sqrt(pow(xx,2)+pow(yy,2));
            REAL_TYPE d_r = (c_rad - sqrt(pow(center_x-focal_x,2)+pow(center_y-focal_y,2)))/c_rad;
            grad = ((custom_gradient*)f->facet_gradient)->grad[1] = new radial_gradient(xx+d_x,yy+d_y,c_rad,d_r);//(int)c_x,(int)c_y,(int)c_r);
            //((custom_gradient*)f->facet_gradient)->grad[1] = NULL; //tmp hide grad
            REAL_TYPE lc = 0;
            if ((j-1 < line_point_count/2)&&(j>1)) lc= r_facet[i*line_point_count+j-1]->light_lit(l_v);
            else lc = (j+2<line_point_count)?r_facet[i*line_point_count+j+1]->light_lit(l_v): r_facet[j]->light_lit(l_v);
            /*if (l1<l1) {
                (*grad)[0]=lc;
                (*grad)[1]=l1;
            } else*/ {
                (*grad)[0]=lc;
                (*grad)[1]=l1;
            }
            grad->transp = 0.1;
            //printf("radial (%f,%f,%f) %f\r\n",c_x,c_y,c_rad,d_r);
          } else {
            PROJECTION_TYPE g2x1=((x0+x2)/2);
            PROJECTION_TYPE g2y1=((y0+y2)/2);
            PROJECTION_TYPE g2x2;//=((x3+x1)/2);
            PROJECTION_TYPE g2y2;//=((y3+y1)/2);
            perpendicular(x1, y1, x3, y3, g2x1, g2y1, g2x2, g2y2);
            //printf("linear (%f,%f),(%f,%f)\r\n",g2x1,g2y1,g2x2,g2y2);
            grad = ((custom_gradient*)f->facet_gradient)->grad[1] = new linear_gradient(g2x1,g2y1,g2x2,g2y2);
            REAL_TYPE lc = 0;
            if ((j-1 < line_point_count/2)&&(j>1)) lc= r_facet[i*line_point_count+j-1]->light_lit(l_v);
            else lc = (j+2<line_point_count)?r_facet[i*line_point_count+j+1]->light_lit(l_v): r_facet[j]->light_lit(l_v);
            (*grad)[0]=lc;
            (*grad)[1]=l2;
            grad->transp = 0.1;
          }
        } else f->facet_gradient = NULL;
    }
}


torus::torus(int r_k, REAL_TYPE r, REAL_TYPE R)
{
 basis self;
 location *c_l = new location[r_k+1];
 for (int i=0; i<=r_k; i++)
  {
   c_l[i]=self(location(-r,0,0))+vector(-R,0,0);
   self%=vector(0,0,2*M_PI/r_k);
  };
 set_shape(r_k,r_k+1,c_l,0,0/*,none,NULL*/);
 delete[] c_l;
 gradient_points_numbers[0]=1;
 gradient_points_numbers[1]=0;
 gradient_points_numbers[2]=2;
 gradient_points_numbers[3]=3;
}

void torus::calculate(facet* f, vector l_v)
{
  if ((f->next!=NULL)&&(f->next->next!=NULL)) {
      //int* pr = f->proection();
      PROJECTION_TYPE* pr = f->proection(); //02.28.2016
      int x0=pr[gradient_points_numbers[0]*2];
      int y0=pr[gradient_points_numbers[0]*2+1];
      int x1=pr[gradient_points_numbers[1]*2];
      int y1=pr[gradient_points_numbers[1]*2+1];
      int x2=pr[gradient_points_numbers[2]*2];
      int y2=pr[gradient_points_numbers[2]*2+1];
      int x3=pr[gradient_points_numbers[3]*2];
      int y3=pr[gradient_points_numbers[3]*2+1];
      //printf("(%i,%i),(%i,%i),(%i,%i),(%i,%i):",x0,y0,x1,y1,x2,y2,x3,y3);
      int center_x=(x0+x1)/2;
      int center_y=(y0+y1)/2;
      int focal_x=(x2+x3)/2;
      int focal_y=(y2+y3)/2;
      double c_x=0,c_y=0,c_r=0;
      bool o_l=false;
      bool cross = LineCrossPoint(x0,y0,x1,y1,x2,y2,x3,y3,c_x,c_y,o_l);
      //c_r = sqrt(pow(c_x-x0,2)+pow(c_y-y0,2));
      c_r = sqrt(pow(c_x-center_x,2)+pow(c_y-center_y,2));
      if (c_r!=0) {
        f->facet_gradient = new custom_gradient();
        //conical_
            gradient* grad;
        if (cross) {
            grad = ((custom_gradient*)f->facet_gradient)->grad[0] = new conical_gradient((int)c_x,(int)c_y);
            REAL_TYPE x=(x1-c_x);
            REAL_TYPE y=(y1-c_y);
            REAL_TYPE a1 = x_angle_rad(x,y);
            x=(x2-c_x);
            y=(y2-c_y);
            REAL_TYPE a2 = x_angle_rad(x,y);
            ((conical_gradient*)grad)->angle=a1;//(abs(x1-x0)>abs(x2-x3))?a2:a1;
            ((conical_gradient*)grad)->stop_angle=a2;//(abs(x1-x0)>abs(x2-x3))?a1:a2;
            //printf(" c(%i,%i),r:%i\r\n",(int)c_x,(int)c_y,(int)c_r);
        } else {
          PROJECTION_TYPE x,y;
          perpendicular(x2,y2,x3,y3,center_x,center_y,x,y);
          grad = ((custom_gradient*)f->facet_gradient)->grad[0] = new linear_gradient(center_x,center_y,x,y);
        }
        int i = f->num/line_point_count;
        int j = f->num%line_point_count;
        REAL_TYPE l1; //=f->light_lit(l_v);//fp->light_lit(l_v);
        REAL_TYPE l2;
        l1=f->light_lit(l_v);
        l2=((i+1)<coefficient)?r_facet[(i+1)*line_point_count+j]->light_lit(l_v):r_facet[j]->light_lit(l_v);
        //(*f->facet_gradient)[0]=l1;
        //(*f->facet_gradient)[1]=l2;
        (*grad)[0]=l1;
        (*grad)[1]=l2;
        //cross = false; //test 02.28.2016
        if (cross) {
          REAL_TYPE xx = (center_x + focal_x)/2.0;
          REAL_TYPE yy = (center_y + focal_y)/2.0;
          REAL_TYPE d_x = xx - (REAL_TYPE)c_x;
          REAL_TYPE d_y = yy - (REAL_TYPE)c_y;
          REAL_TYPE c_rad =  sqrt(pow(xx,2)+pow(yy,2));
          REAL_TYPE d_r = (c_rad - sqrt(pow(center_x-focal_x,2)+pow(center_y-focal_y,2)))/c_rad;
          grad = ((custom_gradient*)f->facet_gradient)->grad[1] = new radial_gradient(xx+d_x,yy+d_y,c_rad,d_r);//(int)c_x,(int)c_y,(int)c_r);
          //((custom_gradient*)f->facet_gradient)->grad[1] = NULL; //tmp hide grad
          REAL_TYPE lc = 0;
          if ((j-1 < line_point_count/2)&&(j>1)) lc= r_facet[i*line_point_count+j-1]->light_lit(l_v);
          else lc = (j+2<line_point_count)?r_facet[i*line_point_count+j+1]->light_lit(l_v): r_facet[j]->light_lit(l_v);
          /*if (l1<l1) {
              (*grad)[0]=lc;
              (*grad)[1]=l1;
          } else*/ {
              (*grad)[0]=lc;
              (*grad)[1]=l1;
          }
          grad->transp = 0.1;
          //printf("radial (%f,%f,%f) %f\r\n",c_x,c_y,c_rad,d_r);
        } else {
          PROJECTION_TYPE g2x1=((x0+x2)/2);
          PROJECTION_TYPE g2y1=((y0+y2)/2);
          PROJECTION_TYPE g2x2;//=((x3+x1)/2);
          PROJECTION_TYPE g2y2;//=((y3+y1)/2);
          perpendicular(x1, y1, x3, y3, g2x1, g2y1, g2x2, g2y2);
          //printf("linear (%f,%f),(%f,%f)\r\n",g2x1,g2y1,g2x2,g2y2);
          grad = ((custom_gradient*)f->facet_gradient)->grad[1] = new linear_gradient(g2x1,g2y1,g2x2,g2y2);
          REAL_TYPE lc = 0;
          if ((j-1 < line_point_count/2)&&(j>1)) lc= r_facet[i*line_point_count+j-1]->light_lit(l_v);
          else lc = (j+2<line_point_count)?r_facet[i*line_point_count+j+1]->light_lit(l_v): r_facet[j]->light_lit(l_v);
          (*grad)[0]=lc;
          (*grad)[1]=l2;
          grad->transp = 0.1;
        }
      } else f->facet_gradient = NULL;
  }
}
