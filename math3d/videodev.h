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
/*   VIDEO devices header                                                   */
/*   A. Volkov, 1991                                                        */
/****************************************************************************/
/*   updated - draw_circle - fill, A. Volkov, 2015                          */
/****************************************************************************/

#ifndef __VIDEO_S
#define __VIDEO_S

#include "obimages.h"
#include "analytic.h"

class video_params
{
 public:
  int scale,max_x,max_y;
  float aspect_ratio;

  video_params()
   {scale=1; max_x=80; max_y=25; aspect_ratio=0.5;} //old text terminal
  video_params (int s, int m_x, int m_y, float a_r)
   {scale=s; max_x=m_x; max_y=m_y; aspect_ratio=a_r;}
};

class image;
class gradient;

class video_system : public video_params
{
 public:
  video_system() {}
  virtual ~video_system() {}    //12.03.2015

  void cadre(image& im);
  virtual void set_gradient(gradient* g,int color);
  //virtual void draw_poly(int num_points,int *poly_points,int color,
  virtual void draw_poly(int num_points,PROJECTION_TYPE *poly_points,int color, //02.28.2016
                         REAL_TYPE value);
  //virtual void draw_poly(int num_points,int *poly_points,int color, /*REAL_TYPE value,*/ gradient* g);
  virtual void draw_poly(int num_points, PROJECTION_TYPE *poly_points,int color, gradient* g); //02.28.2016
  //virtual void draw_point(int x,int y,int color);
  virtual void draw_point(PROJECTION_TYPE x,PROJECTION_TYPE y,int color); //02.28.2016
  //virtual void draw_point(int x,int y,int color, REAL_TYPE value);
  virtual void draw_point(PROJECTION_TYPE x,PROJECTION_TYPE y,int color, REAL_TYPE value); //02.28.2016
  //virtual void draw_circle(int x,int y, int r, int color, bool fill=true);
  virtual void draw_circle(PROJECTION_TYPE x, PROJECTION_TYPE y, PROJECTION_TYPE r, int color, bool fill=true); //12.27.2015
  void set_scale(int s_c)  {scale=s_c;}
  void set_video_params (int s, int m_x, int m_y, float a_r)
          {scale=s; max_x=m_x; max_y=m_y; aspect_ratio=a_r;}
 protected:
    virtual void setup_cadre() {}
    virtual void show_cadre()  {}
};

#endif
