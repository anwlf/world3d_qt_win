/****************************************************************************/
/*    Math3D Library. Performs 3D Vector and matrix calculations.           */
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
/*   FAR OBJECTS  implementation                                            */
/*   A. Volkov, 2012                                                        */
/****************************************************************************/

#include "far_object.h"
#include <float.h>

far_object::far_object(void) : vector() {
    color = 0;
}

far_object::far_object(vector d, int c) : vector(d) {
    color = c;
}

point_image* far_object::create_image(basis& v_b, video_params* v_p) {
  location direction = ort(v_b(*this));
  if (direction[2]>0) {
    int x=v_p->max_x/2+v_p->scale*(direction[0]/direction[2]);
    int y=v_p->max_y/2-v_p->scale*v_p->aspect_ratio*(direction[1]/direction[2]);
    return new point_image(color,x,y,NULL,FLT_MAX);
  }
  return NULL;
}
