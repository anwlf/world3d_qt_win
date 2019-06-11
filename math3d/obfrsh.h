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
/*   Facet ROTATION shapes header                                           */
/*   A. Volkov, 1991                                                        */
/****************************************************************************/

#ifndef __O_FRSH
#define __O_FRSH

#include "obshapes.h"

class rotation_shape: public facet_shape
{
 public:
  rotation_shape() : facet_shape() {}
  rotation_shape(int koeff, int line_point_c, location* p);
  ~rotation_shape();
 protected:
  int coefficient,line_point_count;
  facet* up_facet;
  facet* down_facet;
  facet** r_facet; //[i][j] array
  int gradient_points_numbers[4];
  void set_shape(int koeff, int line_point_c, location* p, int Up,int Down/*, gradient_type gt, int* g_points_nums*/);
};

class cube: public rotation_shape
{
 public:
  cube(REAL_TYPE h=1);
};

class cylinder: public rotation_shape
{
  protected:
    virtual void calculate(facet* f, vector l_v);
  public:
    cylinder(int r_k=16, REAL_TYPE h=1,REAL_TYPE r=1);
};

//added 12.30.2010
class ring: public rotation_shape
{
 public:
  ring(int r_k=16, REAL_TYPE h=1,REAL_TYPE R=1, REAL_TYPE r=0.5);
};

class cone: public rotation_shape
{
protected:
  virtual void calculate(facet* f, vector l_v);
 public:
  cone(int r_k=16, REAL_TYPE h=1,REAL_TYPE r=1);
};

class sphere: public rotation_shape
{
 protected:
  virtual void calculate(facet* f, vector l_v);
 public:
  sphere(int r_k=16, REAL_TYPE r=1);
};

//added 12.30.2010
class ellipsoid: public rotation_shape
{
 protected:
  virtual void calculate(facet* f, vector l_v);
 public:
  ellipsoid(int r_k=16, REAL_TYPE r1=1, REAL_TYPE r2=1);
};

class torus: public rotation_shape
{
  protected:
    virtual void calculate(facet* f, vector l_v);
  public:
    torus(int r_k=16, REAL_TYPE r=0.5, REAL_TYPE R=1.5);
};

#endif
