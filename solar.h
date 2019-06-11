/***********************************************************/
/*   SOLAR SYSTEM DEFINITIONS header                       */
/*   A. Volkov, 2015                                       */
/***********************************************************/

#ifndef SOLAR_H
#define SOLAR_H

#include "astronomy.h"

// Astrnomical Unit (m)
#define AU 1.5E11
// range coeficient
#define range_AU_coef 10E-16//tmp about was 14 02.07.2016
// mass relative to Sun/10E6
#define Sun_absolute_mass 1.98855E30 // kg
#define Sun_mass 1000000.0
#define Mercury_mass 0.16601
#define Venus_mass 2.4478383
#define Earth_mass 3.00348959632
#define Mars_mass 0.3227151
#define Jupiter_mass 954.79194
#define Saturn_mass 285.8860
#define Saturn_ring_mass 0.001 //tmp
#define Uranus_mass 43.6624
#define Neptune_mass 51.51389
#define Pluto_mass 0.007396

#define Planet_absolute_mass(rel_mass) (rel_mass*10E-6*Sun_absolute_mass*range_AU_coef)

// Planets sizes relative Earth
#define Sun_size 109.0
//#define Sun_size 20.90 //01.27.2016 - tmp
#define Mercury_size 0.38
#define Venus_size 0.95
#define Earth_size 1.0
#define Mars_size 0.53
#define Jupiter_size 11.19
#define Saturn_size 9.40
#define Saturn_ring_size 9.40
#define Uranus_size 4.04
#define Neptune_size 3.88
#define Pluto_size 0.18

//#define Planet_scaled_size(rel_size) (log(rel_size*10.0)*50.0) // Log - temporary for better view. 12.09.2015
//#define Planet_scaled_size(rel_size) (log2(rel_size*10.0)*50.0) // Log2 - temporary for better view. 12.15.2015
//  #define Planet_scaled_size(rel_size) (rel_size*20.0)
//#define Planet_scaled_size(rel_size) (rel_size*200.0)
//#define Planet_scaled_size(rel_size) rel_size

// Planets and sun colors
#define Sun_color       0xffff00
#define Mercury_color   0xffff0f
#define Venus_color     0xffffff
#define Earth_color     0xffff00
#define Mars_color      0xff0000
#define Jupiter_color   0xff9f00
#define Saturn_color    0xffef05
#define Saturn_ring_color   0xffef05
#define Uranus_color    0xf0f0f0
#define Neptune_color   0xfffff0
#define Pluto_color     0x0fffff

class Solar_planet: public planet
{
  public:
    virtual const char* name() {return "";}
    Solar_planet(REAL_TYPE m, basis o_b, int color, int size, ph_body* gb) : planet(m,o_b, color,size,gb) {}
    Solar_planet(REAL_TYPE m, basis o_b, int color, int size, ph_body* gb, int sat_cnt, REAL_TYPE sat_ratio) : planet(m,o_b, color,size,gb,sat_cnt,sat_ratio) {}
};

class Mercury: public Solar_planet    // Mercury 0.4AU
{
    //protected:
    public:
        virtual const char* name() {return "Mercury";}
        Mercury(basis o_b, star* sun) : Solar_planet(Planet_absolute_mass(Mercury_mass),basis(o_b - vector(0.4 * 1000)),Mercury_color,Planet_scaled_size(Mercury_size),sun)
        {
            //(*(location*)this)[0] -= 0.4 * 1000; //tmp
        }
};

class Venus: public Solar_planet      // Venus 0.7AU
{
    //protected:
    public:
      virtual const char* name() {return "Venus";}
      Venus(basis o_b, star* sun) : Solar_planet(Planet_absolute_mass(Venus_mass),basis(o_b - vector(0.7 * 1000)),Venus_color,Planet_scaled_size(Venus_size),sun)
        {
            //(*(location*)this)[0] -= 0.7 * 1000; //tmp
        }
};

class Earth: public Solar_planet      // Earth 1AU
{
    //protected:
    public:
      virtual const char* name() {return "Earth";}
        Earth(basis o_b, star* sun) : Solar_planet(Planet_absolute_mass(Earth_mass),basis(o_b - vector(1000)),Earth_color,Planet_scaled_size(Earth_size),sun,1,0.05) //tmp moon
        {
            //(*(location*)this)[0] -= 1000; //tmp
        }
};

class Mars: public Solar_planet       // Mars 1.5AU
{
    //protected:
    public:
      virtual const char* name() {return "Mars";}
        Mars(basis o_b, star* sun) : Solar_planet(Planet_absolute_mass(Mars_mass),basis(o_b - vector(1.4 * 1000)),Mars_color,Planet_scaled_size(Mars_size),sun)
        {
            //(*(location*)this)[0] -= 1.4 * 1000; //tmp
        }
};

class Jupiter: public Solar_planet    // Jupiter 5.2AU
{
    //protected:
    public:
      virtual const char* name() {return "Jupiter";}
        Jupiter(basis o_b, star* sun) : Solar_planet(Planet_absolute_mass(Jupiter_mass),basis(o_b - vector(5.2 * 1000)),Jupiter_color,Planet_scaled_size(Jupiter_size),sun,4,0.5) //tmp satelites
        {
            //(*(location*)this)[0] -= 5.2 * 1000; //tmp
            //(*(location*)this)-=vector(5.2 * 1000,0,0);
        }
};

class Saturn: public Solar_planet     // Saturn 9.5AU
{
    //protected:
    public:
      virtual const char* name() {return "Saturn";}
        Saturn(basis o_b, star* sun) : Solar_planet(Planet_absolute_mass(Saturn_mass),basis(o_b - vector(9.5 * 1000)),Saturn_color,Planet_scaled_size(Saturn_size),sun)
        {
            //(*(location*)this)[0] -= 9.5 * 1000; //tmp
        }
};


//Does not work
class Saturn_ring: public planet_ring // Saturn ring 9.5AU // tmp solution 12.09.2015
{
    //protected:
    public:
        Saturn_ring(basis o_b, star* sun) : planet_ring(Planet_absolute_mass(Saturn_ring_mass),basis(o_b - vector(9.5 * 1000)),Saturn_ring_color,Planet_scaled_size(Saturn_ring_size),sun)
        {
            //(*(location*)this)[0] -= 9.5 * 1000; //tmp
        }
};

class Uranus: public Solar_planet     // Uranus 19.6AU
{
    //protected:
    public:
      virtual const char* name() {return "Uranus";}
        Uranus(basis o_b, star* sun) : Solar_planet(Planet_absolute_mass(Uranus_mass),basis(o_b - vector(19.6 * 1000)),Uranus_color,Planet_scaled_size(Uranus_size),sun)
        {
            //(*(location*)this)[0] -= 19.6 * 1000; //tmp
        }
};

class Neptune: public Solar_planet    // Neptune 30AU
{
    //protected:
    public:
      virtual const char* name() {return "Neptune";}
        Neptune(basis o_b, star* sun) : Solar_planet(Planet_absolute_mass(Neptune_mass),basis(o_b - vector(30 * 1000)),Neptune_color,Planet_scaled_size(Neptune_size),sun)
        {
            //(*(location*)this)[0] -= 30 * 1000; //tmp
        }
};

class Pluto: public Solar_planet      // Pluto 39AU
{
    //protected:
    public:
      virtual const char* name() {return "Pluto";}
        Pluto(basis o_b, star* sun) : Solar_planet(Planet_absolute_mass(Pluto_mass),basis(o_b - vector(39 * 1000)),Pluto_color,Planet_scaled_size(Pluto_size),sun)
        {
            //(*(location*)this)[0] -= 39 * 1000; //tmp
        }
};

class Sun : public star
{
    //protected:
    public:
        planet* earth() {return (planets[2]);}
        planet* jupiter() {return (planets[4]);}
        planet* uranus() {return (planets[7]);}
        Sun(basis o_b) : star(Planet_absolute_mass(Sun_mass),o_b,Sun_color,Planet_scaled_size(Sun_size)/**3*/)  // size*3 - tmp // Sun - 99.86% Solar system mass
        {
            planets_count = 11;
            planets = new planet*[planets_count];
            planets[0] = new Mercury(o_b,this);
            planets[1] = new Venus(o_b,this);
            planets[2] = new Earth(o_b,this);
            planets[3] = new Mars(o_b,this);
            planets[4] = new Jupiter(o_b,this);
            planets[5] = new Saturn(o_b,this);
            planets[6] = (planet*)new Saturn_ring(o_b,this); // tmp solution 12.09.2015
            planets[7] = new Uranus(o_b,this);
            planets[8] = new Neptune(o_b,this);
            planets[9] = new Uranus(o_b,this);
            planets[10] = new Pluto(o_b,this);
        }

};

#endif // SOLAR_H
