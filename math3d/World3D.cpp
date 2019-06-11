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
/*   World3D implementation                                                 */
/*   A. Volkov, 2012                                                        */
/****************************************************************************/

#include "World3D.h"
#include "obfrsh.h"

/* Creates internal image for world 3d */
image* World3D::create_image(video_params* v_p)
{
 image* im = new image;
 for (int i=0; i<Ob_count; i++) {
 // (*im)+=Objects[i].create_image((*Base),Light,v_p); // change to address
   if (Objects[i]->visible((*Base),v_p)) {
    (*im)+=Objects[i]->create_image((*Base/*this*/),Light,v_p); //*Base changed to *this 12.29.2015 - not working good
   }
 }
 return im;
}

void World3D::Build(basis* b)
{
 Speed=vector(0,0,0);
 Rot=vector(0,0,0);
 SetBase((b==NULL)?this:b);
 SetLight(vector(-0.0,-0.0,-1.0));
 step=0;
}

void World3D::Step_of_life()
{
 (*Base)>>=Speed;
 (*Base)%=Rot;
 //12.28.2015 not working good
 // (*this)>>=Speed;
 // (*this)%=Rot;
 //
 step++;
}
