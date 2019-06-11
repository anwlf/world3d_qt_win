/***********************************************************/
/*   ASTRONOMY header                                      */
/*   A. Volkov, 2015                                       */
/***********************************************************/

#ifndef ASTRONOMY_H
#define ASTRONOMY_H

/*
#include "math3d/obimages.h"
#include "math3d/object3D.h"
#include "math3d/obfrsh.h"
*/
//#include "math3d/analytic.h"
#include "physics.h"

class planetoid : public ph_body
{
        static sphere shape;// = sphere(12);
    public:
        planetoid(REAL_TYPE m, basis o_b, int color, int size, ph_body* gb) : ph_body(m,o_b,shape,color,size, gb)
        {

        }
};

class planet_ring : public ph_body
{
        static ring shape;// = ring(32,0.001,2,1.2);
    public:
        planet_ring(REAL_TYPE m, basis o_b, int color, int size, ph_body* gb) : ph_body(m,o_b,shape,color,size, gb)
        {

        }
};

class planet : public ph_body
{
        static sphere shape;// = sphere(16);
        //static sphere_points_shape shape;
    protected:
        planetoid** satelite;
        int satelite_count;
    protected:
        void add_satelite(REAL_TYPE size_ratio, int color, REAL_TYPE distance)
        {
            planetoid** s = satelite;
            int s_c = satelite_count;
            satelite_count++;
            satelite = new planetoid*[satelite_count];
            for (int i=0; i<s_c; i++) {
                satelite[i] = s[i];
            }
            delete s;
            satelite[s_c] = new planetoid(mass * pow(size_ratio,3), (basis)(*((basis*)this)+distance), color, object_size * size_ratio, this);
        }
    public:
        planet(REAL_TYPE m, basis o_b, int color, int size, ph_body* gb) : ph_body(m,o_b,shape,color,size, gb)
        {
            satelite_count = 0;
            satelite = NULL;
        }
        planet(REAL_TYPE m, basis o_b, int color, int size, ph_body* gb, int sat_cnt, REAL_TYPE sat_ratio) : ph_body(m,o_b,shape,color,size, gb)//planet(m, o_b, color, size, gb)
        {
            satelite_count = sat_cnt;
            satelite = new planetoid*[satelite_count];
            REAL_TYPE sat_mass = mass * pow(sat_ratio,3);
            int sat_size = size * sat_ratio;
            //
            basis b = (*(basis*)this);//basis(location(*this));
            //b%=vector(rand(),rand(),rand());
            vector shift = vector(size+(size*1.0*rand())/RAND_MAX,(size*1.0*rand())/RAND_MAX,0);
            for (int i=0; i<satelite_count; i++) {
                //random
                int calc_size = (rand() % sat_size)/3+1;
                REAL_TYPE calc_mass = sat_mass * pow(calc_size / sat_size, 3);
                //((location)b)
                //b-=vector(size*2.0,size*0.3,0);
                b-=shift;
                //b*=vector(1,2,3);
                satelite[i] = new planetoid(calc_mass, /*(location)(*((basis*)this)+/ *distance* /rand())*/b, /*color*/rand(), calc_size, this);
            }
        }
        virtual void step_of_life()
        {
            ph_body::step_of_life();
            for (int i=0; i<satelite_count; i++) {
                satelite[i]->step_of_life();
                (*satelite[i])+=_speed;
            }
        }
        virtual image* create_image(basis& v_b, vector& l_v, video_params* v_p) {
            image* im = /*new image;*/ph_body::create_image(v_b,l_v,v_p);
            //(*im)+=ph_body::create_image(v_b,l_v,v_p);
            for (int i=0; i<satelite_count; i++) {
                if (satelite[i]->visible(v_b,v_p)) { // test 12.07.2015
                    (*im)+=satelite[i]->create_image(v_b,l_v,v_p);
                }
            }
            return im;
        }
};

//#define Planet_scaled_size(rel_size) (log2(rel_size*10.0)*50.0) // Log2 - temporary for better view. 12.15.2015
//#define Planet_scaled_size(rel_size) (rel_size*2.0)
#define Planet_scaled_size(rel_size) (rel_size*20.0)

class star : public ph_body
{
        static sphere shape;// = sphere(32);
    protected:
        //planet** planets;
        //int planets_count;
    public:
        planet** planets; //moved here 02.17.2016
        int planets_count;//moved here 02.17.2016
        //bool selected; //12.22.2015 - testing
        star(REAL_TYPE m, basis o_b, int color, int size) : ph_body(m,o_b,shape,color,size)
        {
            planets=NULL;
            planets_count=0;
            //selected = false; //12.22.2015 - testing
        }
        star(REAL_TYPE m, basis o_b, int color, int size, int planets_cnt, REAL_TYPE planets_ratio) : ph_body(m,o_b,shape,color,size)
        {
          planets_count = planets_cnt;
          planets = new planet*[planets_count];
          //selected = false; //12.22.2015 - testing
          REAL_TYPE planets_mass = mass * pow(planets_ratio,3);
          int planets_size = size * planets_ratio;
          //
          if (planets_size==0) planets_size=1; //02.17.2016
          //
          basis b = (*(basis*)this);//basis(location(*this));
          //b%=vector(rand(),rand(),rand());
          vector shift = vector(size+(size*1.0*rand())/RAND_MAX,(size*1.0*rand())/RAND_MAX,0);
          for (int i=0; i<planets_count; i++) {
              //random
              //int calc_size = (rand() % planets_size)/3+1;
              REAL_TYPE calc_size = ((rand() % planets_size)/3.0+1.0)*0.1; //02.17.2016
              REAL_TYPE calc_mass = planets_mass * pow(calc_size / planets_size, 3);
              //((location)b)
              //b-=vector(size*2.0,size*0.3,0);
              b-=shift;
              //b*=vector(1,2,3);
              planets[i] = new planet(calc_mass, /*(location)(*((basis*)this)+/ *distance* /rand())*/b, ((color & rand())|0xF0F0F0), Planet_scaled_size(calc_size), this);
          }
        }
        virtual void step_of_life()
        {
            ph_body::step_of_life();
            for (int i=0; i<planets_count; i++) {
                planets[i]->step_of_life();
                (*planets[i])+=_speed;
            }
        }
        virtual image* create_image(basis& v_b, vector& l_v, video_params* v_p) {
            image* im = NULL;
            if (!visible(v_b,v_p)) {
                location l = v_b(*(location*)this);
                REAL_TYPE d=/*abs*/(l[2]); //
                if (d>0) {
                    im = new image;
                    int x =v_p->max_x/2+v_p->scale*(l[0]/d);
                    int y =v_p->max_y/2-v_p->scale*v_p->aspect_ratio*(l[1]/d);
                    //12.22.2015 - testing
                    //if (selected) (*im)+=new circle_image(facets_colors[0],x,y,5,NULL,d);
                    //else
                    //(*im)+= new point_image(facets_colors[0],x,y,NULL,d); //01.23.2016
                    (*im)+= new point_image(facet_color,x,y,NULL,d);   //01.23.2016
                    //
                    //im = new point_image(facets_colors[0],x,y,NULL,d);
                }
            } else {
                //im = /*new image;*/ph_body::create_image(v_b,l_v,v_p);
                im = new image;
                location l = v_b(*(location*)this);
                REAL_TYPE d=/*abs*/(l[2]); //
                int x =v_p->max_x/2+v_p->scale*(l[0]/d);
                int y =v_p->max_y/2-v_p->scale*v_p->aspect_ratio*(l[1]/d);
                //
                int r = object_size*v_p->scale/(d*2);
                if (r>0.5) {
                  //(*im)+=ph_body::create_image(v_b,l_v,v_p);
                  //(*im)+=new circle_image(facets_colors[0],x,y,r,NULL,d); //01.23.2016
                    (*im)+=new circle_image(facet_color,x,y,r,NULL,d); //01.23.2016
                } else {
                    //12.22.2015 - testing
                    //if (selected) (*im)+=new circle_image(facets_colors[0],x,y,5,NULL,d);
                    //else
                    //(*im)+= new point_image(facets_colors[0],x,y,NULL,d); //01.23.2016
                    (*im)+= new point_image(facet_color,x,y,NULL,d); //01.23.2016
                    //
                  //(*im)+= new point_image(facets_colors[0],x,y,NULL,d);
                }
                for (int i=0; i<planets_count; i++) {
                    if (planets[i]->visible(v_b,v_p)) { // test 12.07.2015
                        vector lt = ort(*(location*)this - *(location*)planets[i]);
                        (*im)+=planets[i]->create_image(v_b,/*l_v*/lt,v_p);
                    }
                }
            }
            return im;
            l_v=l_v;
        }
};

#endif // ASTRONOMY_H
