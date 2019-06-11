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
/*   OBJECTS header                                                         */
/*   A. Volkov, 1991                                                        */
/****************************************************************************/
/*   updated A. Volkov, 2015                                                */
/****************************************************************************/

#ifndef __OBJECTS_3D
#define __OBJECTS_3D

#include "analytic.h"
#include "obshapes.h"

class object: public basis
{
    //facet_shape *object_shape;
    shape *object_shape; //01.23.2016
  protected:         //12.08.2015
    //01.23.2016 - change to singlr color for shape
    //int *facets_colors;
    int facet_color;
  //int object_size;  // commented 12.04.2015
 //protected:         //12.04.2015
  REAL_TYPE object_size;  //12.04.2015 - tested <REAL_TYPE> instead int - affects speed a little
 public:
  object(void);
  //object(basis o_b, facet_shape& sh, int color, int size=1); //commented 01.08.2016
  //object(basis o_b, facet_shape& sh, int color, REAL_TYPE size=1); //01.23.2016
  object(basis o_b, shape& sh, int color, REAL_TYPE size=1); //01.23.2016
  virtual ~object(void) {} //02.19.2016 - test for polymorph. objects delete compile warning//1.9.2016 //1.1.2016 //01.23.2016 - no need
  //virtual bool visible(basis& v_b, video_params* v_p) {return object_size*v_p->scale/abs(v_b(*(location*)this))>=0.5;} //test 12.07.2015
  virtual bool visible(basis& v_b, video_params* v_p) {return object_size*v_p->scale/ /*abs*/(v_b(*(location*)this)[2])>1.0;} //test 12.07.2015
  virtual bool visible(basis& v_b, video_params* v_p, int view_port_x, int view_port_y, int view_port_w, int view_port_h)
  {
    location loc = v_b(*(location*)this);
    if (loc[2]>0) { //12.28.2015
      int x = v_p->max_x/2 + loc[0]*v_p->scale/loc[2];
      int y = v_p->max_y/2 - loc[1]*v_p->scale*v_p->aspect_ratio/loc[2];
      return (x>view_port_x) && (x<view_port_x+view_port_w )&& (y>view_port_y) && (y<view_port_y+view_port_h);
    }
    return false; //12.28.2015
    /*
    return (v_b(*(location*)this)[0]*v_p->scale/(v_b(*(location*)this)[2])+v_p->max_x/2>view_port_x)&&(v_b(*(location*)this)[0]*v_p->scale/(v_b(*(location*)this)[2])+v_p->max_x/2<view_port_x+view_port_w) &&
           (v_b(*(location*)this)[1]*v_p->scale*v_p->aspect_ratio/(v_b(*(location*)this)[2])+v_p->max_y/2>view_port_y)&&(v_b(*(location*)this)[1]*v_p->scale*v_p->aspect_ratio/(v_b(*(location*)this)[2])+v_p->max_y/2<view_port_y+view_port_h);
    */
  } //test 12.18.2015
  virtual image* create_image(basis& v_b, vector& l_v, video_params* v_p);
};

#endif
