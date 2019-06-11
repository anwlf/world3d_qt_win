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
/*   OBJECTS  implementation                                                */
/*   A. Volkov, 1991                                                        */
/****************************************************************************/

#include "object3D.h"

object::object(void) : basis()
{
 object_shape = NULL;
 //facets_colors = NULL; //01.23.2016
 facet_color = 0;        //01.23.2016
 object_size = 0;
}

object::object(basis o_b, /*facet_*/shape& sh, int color, /*int*/REAL_TYPE size) //01.23.2016 faset_shape to shape
       :basis(o_b)
{
 object_shape = &sh;
 //facets_colors = new int[object_shape->count()]; ////01.23.2016
 //for (int i=0; i<object_shape->count(); i++) //01.23.2016
 // facets_colors[i]=color;                    //01.23.2016
 //01.23.2016
 facet_color=color;
 object_size = size;
}

//01.23.2016 - no need
/*
object::~object(void) //1.1.2016
{
  //delete facets_colors; facets_colors[i]=color;
}
*/

/*
image* object::create_image(basis& v_b, vector& l_v, video_params* v_p)
{
 if (object_shape!=NULL)
  return object_shape->create_image(facets_colors, object_size,
				    (*this)(v_b),
				    (*this)(l_v), v_p);
 else return NULL;
}
*/

image* object::create_image(basis& v_b, vector& l_v, video_params* v_p)
{
 if (object_shape!=NULL)
  return object_shape->create_image(/*facets_colors[0]*/facet_color, object_size,
                                    (*this)(v_b),
                                    (*this)(l_v), v_p);
 else return NULL;
}
