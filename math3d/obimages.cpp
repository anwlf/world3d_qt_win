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
/*   IMAGES  implementation                                                 */
/*   A. Volkov, 1991                                                        */
/****************************************************************************/
/*   updated - circle_image, lock_image, A. Volkov, 2015                    */
/****************************************************************************/

#include "obimages.h"
#include "eventcon.h"

void point_image::draw(video_system* v_s)
{
 _EVENT_H;
 v_s->draw_point(x,y,color);
}

//01.23.2016
void points_image::draw(video_system* v_s)
{
 _EVENT_H;
 for (int i=0; i<count; i++) {
  v_s->draw_point(points[i].x,points[i].y,points[i].color,points[i].value);
 }
}


//12.27.2015
void lock_image::draw(video_system* v_s)
{
 _EVENT_H;
 v_s->draw_circle(x,y,r,color,false);
}

//12.08.2015
void circle_image::draw(video_system* v_s)
{
 _EVENT_H;
 v_s->draw_circle(x,y,r,color);
}

void poly_image::draw(video_system* v_s)
{
 _EVENT_H;
 v_s->draw_poly(num_points,poly_points,color,value);
}

void gradient_poly_image::draw(video_system* v_s)
{
    _EVENT_H;
    if (grad==NULL) v_s->draw_poly(num_points,poly_points,color,value);
    else v_s->draw_poly(num_points,poly_points,color/*,value*/,grad);
}

//image& image::operator +=(poly_image* p_i)
image& image::operator +=(part_image* p_i)
{
 if (p_i==NULL) return *this;
 //poly_image *p = image_elements;
 part_image *p = image_elements;
 
 if (image_elements==NULL)
  {image_elements=p_i; p_i->next=NULL;}
 else
  {
   while (!((p->next==NULL)||
	 (p->next->distance<p_i->distance)))
    {
     _EVENT_H;
     p = p->next;
    };
   if ((p==image_elements)&&(p->distance<p_i->distance))
    {p_i->next=p; image_elements = p_i;}
   else
    {p_i->next=p->next; p->next=p_i;};
  };
 return *this;
}

/* commented 1.1.2015
image& image::operator +=(image* p_i)
{
 if (p_i==NULL) return *this;
 //poly_image* p = p_i->image_elements;
 //poly_image* p_n = p;
 part_image* p = p_i->image_elements;
 part_image* p_n = p;

 while (p!=NULL)
  {
   p_n=p->next;
   (*this)+=p;
   p=p_n;
  };
 return *this;
}
*/

// 1.2.2015
image& image::operator +=(image* p_i)
{
  if (p_i==NULL) return *this;
  //poly_image* p = p_i->image_elements;
  //poly_image* p_n = p;
  part_image* p = p_i->image_elements;
  part_image* p_n = p;

  while (p!=NULL)
   {
    p_n=p->next;
    (*this)+=p;
    p=p_n;
   };
  //1.2.2016 - memory cleanup
  p_i->image_elements = NULL;
  delete p_i;
  //
  return *this;
}

image::~image()
{
  /*
   while (image_elements!=NULL)
    {
     //poly_image* p = image_elements->next;
     part_image* p = image_elements->next;
     //delete image_elements->poly_points;
     delete image_elements;
     image_elements = p;
    }
    */
    //delete image_elements; //test 1.1.2012
  clear();
}

void image::clear()
{
  while (image_elements!=NULL)
    {
     //poly_image* p = image_elements->next;
     part_image* p = image_elements->next;
     //delete image_elements->poly_points;
     delete image_elements;
     image_elements = p;
    }
}

void image::draw(video_system* v_s)
{
  //poly_image* p_i = image_elements;
  part_image* p_i = image_elements;
  while (p_i!=NULL)
  {
    p_i->draw(v_s);
    p_i=p_i->next;
  };
}
