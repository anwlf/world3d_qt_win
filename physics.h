/***********************************************************/
/*   World 3D QT, physics header                           */
/*   A. Volkov 2015                                        */
/***********************************************************/

#ifndef PHYSICS_H
#define PHYSICS_H

//#include "math3d/object3D.h"
#include "pshapes.h"

#define G 6.674E-11  // Gravity Constant
//#define G 6.674E-13  // Gravity Constant - test 02.17.2016 to slow down speed

/*
REAL_TYPE gravity_force(REAL_TYPE m1, REAL_TYPE m2, REAL_TYPE d) {
    return (G*m1*m2)/(d*d); //pow2(d)
}
*/

class ph_body: public object
{
 protected:
    REAL_TYPE mass;
    vector _speed;
    vector _rotation;
    /*
    friend REAL_TYPE gravity_force(ph_body& b1, ph_body& b2) {
        return gravity_force(b1.mass, b2.mass, range(b1,b2));
    }
    */
 public:
    //static
    ph_body* gravity_body;
    ph_body(REAL_TYPE m) : object()
    {
        //::object();
        mass = m;
        //_rotation = vector(0, 0, 0);//vector(0.1 * rand()/RAND_MAX, 0.1 * rand()/RAND_MAX,0.1 * rand()/RAND_MAX);
        //_speed = vector(0, 0, 0.1);
        gravity_body = NULL;
        _speed = vector(0, 0, 0);
        _rotation = vector(0, 0, 0);
    }
    ph_body(REAL_TYPE m, basis o_b, /*facet_*/shape& sh, int color, /*int*/REAL_TYPE size=1, ph_body* gb=NULL) : object(o_b, sh, color, size)
    {
        //::object(o_b, sh, color, size);
        mass = m;
        //_rotation = /*vector(0, 0, 0);*/vector(0.1 * rand()/RAND_MAX, 0.1 * rand()/RAND_MAX,0.1 * rand()/RAND_MAX);
        //_speed = vector(0, 0, 13.0); //works
        //_speed = vector(0, 2, 12.0);
        gravity_body = gb;
        _rotation = vector(0, 0, 0);
        if (gravity_body!=NULL) {
            _speed = vector(0,0,sqrt(gravity_body->mass*G/range((location)(*gravity_body),(location)(*this))));
        }
        else _speed = vector(0, 0, 0);
    }
    virtual void step_of_life()
    {
        //*this%=_rotation; (*this)+=_speed;
        if (gravity_body!=NULL) _speed+=gravity_body->mass*G*ort((location)(*gravity_body)-(location)(*this))/pow(range((location)(*gravity_body),(location)(*this)),2);
        *this%=_rotation; (*this)+=_speed;
    }
    virtual void step_of_life(vector& s, vector& r)
    {
        *this%=r; (*this)+=s;
    }
};

#endif
