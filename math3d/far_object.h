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
/*   FAR OBJECTS header                                                     */
/*   A. Volkov, 2012                                                        */
/****************************************************************************/

#ifndef __FAR_OBJECTS
#define __FAR_OBJECTS

#include "analytic.h"
#include "videodev.h"

class far_object: public vector {	    // only direction to the object
	int color;
    public:
	far_object(void);
	far_object(vector d, int c);
	point_image* create_image(basis& v_b, video_params* v_p);
};

#endif
