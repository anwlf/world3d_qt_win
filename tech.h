#ifndef TECH_H
#define TECH_H

#include "physics.h"

class tech_object: public ph_body
{
  protected:
    const char* _name;
    virtual void calculate() {}
  public:
    basis* Base() { return this; }
    vector* Speed() { return &_speed; }
    vector* Rot() { return &_rotation; }
    const char* Name() { return _name; }
    tech_object(REAL_TYPE m, basis o_b, /*facet_*/shape& sh, int color, REAL_TYPE size=1, ph_body* gb=NULL):ph_body(m, o_b, sh, color, size, gb) {}
    virtual void step_of_life() {calculate(); ph_body::step_of_life();}
};

class ellipsoid_saucer: public tech_object
{
  protected:
    static ellipsoid shape;
    //static ellipsoid_points_shape shape;
  public:
    ellipsoid_saucer(REAL_TYPE m, basis o_b, int color, REAL_TYPE size=1, ph_body* gb=NULL) : tech_object(m, o_b, shape, color, size, gb) {_name="ellipsoid";}
};

class cube_saucer: public tech_object
{
  protected:
    //static cube shape;
    //static sphere_points_shape shape;
    static sphere shape;
  public:
    cube_saucer(REAL_TYPE m, basis o_b, int color, REAL_TYPE size=1, ph_body* gb=NULL) : tech_object(m, o_b, shape, color, size, gb) {_name="cube";}
};

class cone_saucer: public tech_object
{
  protected:
    static cone shape;
  public:
    cone_saucer(REAL_TYPE m, basis o_b, int color, REAL_TYPE size=1, ph_body* gb=NULL) : tech_object(m, o_b, shape, color, size, gb) {_name="cone";}
};

class space_station: public tech_object
{
  protected:
    static torus shape_0;
    static cylinder shape_1;
    object* central;
    virtual void calculate()
    {
        //*(basis*)central = *((basis*)this);
        //memcpy((basis*)central,((basis*)this),sizeof(basis));
    }
  public:
    space_station(REAL_TYPE m, basis o_b, int color, REAL_TYPE size=1, ph_body* gb=NULL) : tech_object(m, o_b, shape_0, color, size, gb)
    {
        central= new object(o_b, shape_1, color, size*0.5);
        _name="space_station";
    }
    //virtual
    ~space_station() {delete central;}
    virtual image* create_image(basis& v_b, vector& l_v, video_params* v_p)
    {
        image* im = central->create_image(v_b,l_v,v_p);
        (*im)+=tech_object::create_image(v_b,l_v,v_p); //commented tmp for test
        return im;
    }
    virtual void step_of_life() { *(basis*)central = *((basis*)this); tech_object::step_of_life();*(basis*)central = *((basis*)this);}
    virtual void step_of_life(vector& s, vector& r)
    {
        *this%=r; (*this)+=s;
        *central%=r; (*central)+=s;
    }
};


#endif // TECH_H

