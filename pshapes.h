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
/*   Points shapes header                                           */
/*   A. Volkov, 2016                                                        */
/****************************************************************************/

#ifndef PSHAPES_H
#define PSHAPES_H

#include "math3d/obshapes.h"

//01.23.2016
class points_shape: public shape
{
  protected:
    REAL_TYPE size_coef;
  public:
    //points_shape() {size_coef=1.0;}
    points_shape(REAL_TYPE k) {size_coef=k;}
};

class sphere_points_shape: public points_shape
{
  protected:
  public:
    sphere_points_shape(REAL_TYPE k=1.0) : points_shape(k) {}
    virtual image* create_image(int f_c, REAL_TYPE size, basis v_b,
                                vector l_v, video_params* v_p) {
        location p = v_b(location(0,0,0));
        REAL_TYPE d=p[2];
        if (d<=0) return NULL;
        int r = (int)((v_p->scale*size*size_coef/2)/d);
        /*
        if (d>0)
        {
            //v_proections[i][0]=v_p->max_x/2+v_p->scale*(vrtx[0]/d);
            //v_proections[i][1]=v_p->max_y/2-v_p->scale*v_p->aspect_ratio*(vrtx[1]/d);
            REAL_TYPE x0 = v_p->max_x/2 + v_p->scale*((p[0]-r)/d);
            REAL_TYPE x1 = v_p->max_x/2 + v_p->scale*((p[0]+r)/d);
            REAL_TYPE y0 = v_p->max_y/2-v_p->scale*v_p->aspect_ratio*((p[1]-r)/d);
            REAL_TYPE y1 = v_p->max_y/2-v_p->scale*v_p->aspect_ratio*((p[1]+r)/d);
        }*/
        //
        int cnt = (int)pow(r*2,2);
        image* im = new image();
        points_image *pim = new points_image(f_c, cnt, NULL, d);
        vector l=l_v;
        l.normalization();
        int z=0;
        for (int i=-r; i<=r; i++) {
            for (int j=-r; j<=r; j++) {
                REAL_TYPE x = i;
                REAL_TYPE y = j;
                if ((pow(x,2)+pow(y,2))<pow(r,2)) {
                  p = v_b(location(x,y,sqrt(pow(r,2) - (pow(x,2)+pow(y,2)))));
                  if (p[2]>0) {
                    int _x = v_p->max_x/2+p[0];
                    int _y = v_p->max_y/2-v_p->aspect_ratio*p[1];
                    (*pim)[z++]=v_point(f_c,_x,_y,(ort(p)*(-l)));
                  }
                }
            }
        }
        (*im)+=pim;
        return im;
    }
};

/*
class ellipsoid_points_shape: public points_shape
{
  protected:
    REAL_TYPE x_coef,y_coef,z_coef;
  public:
    ellipsoid_points_shape(REAL_TYPE k=1.0,REAL_TYPE x_c=1.0,REAL_TYPE y_c=1.0,REAL_TYPE z_c=1.0) : points_shape(k) {x_coef=1.0/pow(x_c,2);y_coef=1.0/pow(y_c,2);z_coef=1.0/pow(z_c,2);}
    virtual image* create_image(int f_c, REAL_TYPE size, basis v_b,
                                vector l_v, video_params* v_p) {
        location p = v_b(location(0,0,0));
        REAL_TYPE d=p[2];
        if (d<=0) return NULL;
        //int r = (int)((v_p->scale*size*size_coef/2)/d);
        int w = (int)((v_p->scale*size*size_coef*sqrt(x_coef)/2)/d);
        int h = (int)((v_p->scale*size*size_coef*sqrt(y_coef)/2)/d);
        int _d = (int)((v_p->scale*size*size_coef*sqrt(z_coef)/2)/d);
        //
        //int cnt = (int)pow(r*2,2);
        int cnt = w*h*4;
        image* im = new image();
        points_image *pim = new points_image(f_c, cnt, NULL, d);
        vector l=l_v;
        l.normalization();
        int z=0;
        //for (int i=-r; i<=r; i++) {
        //    for (int j=-r; j<=r; j++) {
        for (int i=-w; i<w; i++) {
            for (int j=-h; j<h; j++) {
                REAL_TYPE x = i;
                REAL_TYPE y = j;
                //if ((x_coef*pow(x,2)+y_coef*pow(y,2))</ *z_coef** /pow(r,2)) {
                //  location p1 = v_b(location(x,y,sqrt((pow(r,2) - (x_coef*pow(x,2)+y_coef*pow(y,2)))/z_coef)));
                if ((x_coef*pow(x,2)+y_coef*pow(y,2))<z_coef*pow(_d,2)) {
                  location p1 = v_b(location(x,y,sqrt((pow(_d,2) - (x_coef*pow(x,2)+y_coef*pow(y,2)))/z_coef)));
                  if (p1[2]>0) {
                    int _x = v_p->max_x/2+p1[0];
                    int _y = v_p->max_y/2-v_p->aspect_ratio*p1[1];
                    if (z<cnt) (*pim)[z++]=v_point(f_c,_x,_y,(ort(p-p1)*(l)));
                    else goto END;
                  }
                }
            }
        }
END:
        points_image *pim_n = new points_image(f_c, z, NULL, d);
        for (int i=0; i<z; i++) {
            (*pim_n)[i]=(*pim)[i];
        }
        delete pim;
        (*im)+=pim_n;
        return im;
    }
};
*/

#include <stdio.h>

class ellipsoid_points_shape: public points_shape
{
  protected:
    REAL_TYPE a,b,c;
    REAL_TYPE x_coef,y_coef,z_coef;
  public:
    ellipsoid_points_shape(REAL_TYPE k=1.0,REAL_TYPE x_c=1.0,REAL_TYPE y_c=1.0,REAL_TYPE z_c=1.0) : points_shape(k)
    {
        a=x_c; b=y_c; c=z_c;
        x_coef=1.0/pow(x_c,2);
        y_coef=1.0/pow(y_c,2);
        z_coef=1.0/pow(z_c,2);
    }
    virtual image* create_image(int f_c, REAL_TYPE size, basis v_b,
                                vector l_v, video_params* v_p) {
        double sz = (a>b)?((a>c)?a:((b>c)?b:c)):((a>c)?a:c);
        sz*=(size*size_coef);
        location p = v_b(location(0,0,0));
        REAL_TYPE d=p[2];
        if (d<=0) return NULL;
        int steps = (int)((v_p->scale*sz)/d);
        if (steps<=0) return NULL;
        //else if (steps>100) steps=100;
        int cnt = pow(steps*2,2)*2;
        double step = sz/(steps*2);
        printf("Steps: %i, step: %e\r\n",steps,step);
        image* im = new image();
        points_image *pim = new points_image(f_c, cnt, NULL, d);
        vector l=l_v;
        l.normalization();
        int n=0;
        for (int i=-steps; i<steps; i++) {
            for (int j=-steps; j<steps; j++) {
                if ((x_coef*pow(i,2)+y_coef*pow(j,2))<1) {
                    double x = step*i;
                    double y = step*j;
                    double z = sqrt((1.0 - (x_coef*pow(i,2)+y_coef*pow(j,2)))/z_coef)*step;
                    location p1 = v_b(location(x,y,z));
                    //if (p1[2]>0) {
                        //int _x = v_p->max_x/2+p1[0];
                        //int _y = v_p->max_y/2-v_p->aspect_ratio*p1[1];
                        int _x = v_p->max_x/2+v_p->scale*(p1[0]/d);
                        int _y = v_p->max_y/2-v_p->scale*v_p->aspect_ratio*(p1[1]/d);
                        if (n<cnt) (*pim)[n++]=v_point(f_c,_x,_y,(ort(p-p1)*(l)));
                        else goto END;
                    //}
                }
            }
        }
END:
        points_image *pim_n = new points_image(f_c, n, NULL, d);
        for (int i=0; i<n; i++) {
            (*pim_n)[i]=(*pim)[i];
        }
        delete pim;
        (*im)+=pim_n;
        return im;
    }
};

#endif // PSHAPES_H
