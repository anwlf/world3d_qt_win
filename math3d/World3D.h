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
/*   World3D header                                                         */
/*   A. Volkov, 2012                                                        */
/****************************************************************************/

#ifndef __WORLD_3_D
#define __WORLD_3_D

#include "obimages.h"
#include "object3D.h"
#include "obfrsh.h"

class World3D : public basis
 {
  protected:
   //facet_shape *Shapes;
   shape *Shapes; //01.23.2016
   //object *Objects; // change to address
   object **Objects;

   vector Light;
   int Sh_count,Ob_count;
  public:
   basis *Base;
   //
   long int step;
   vector Speed;
   vector Rot;
   World3D(basis* b=NULL)  {
       Sh_count = 0;
       Ob_count = 0;
       Build(b);
   }
   virtual ~World3D() { delete[] Shapes; delete[] Objects;}
   virtual image* create_image(video_params* v_p);
   void Step(void)  {Step_of_life();}
   basis* SetBase(basis *b) {basis *old_b = Base; Base = b; /*(*(basis*)this) = (*b);*/ return old_b;} //added (*(basis*)this) = (*b); 12.28.2015 not working good
   basis* GetBasis() {return this;}
  protected:
   vector SetLight(vector l) {vector old_l = Light; Light = l; return old_l;}
   virtual void Build(basis* b=NULL);
   virtual void Step_of_life();
 };

#endif
