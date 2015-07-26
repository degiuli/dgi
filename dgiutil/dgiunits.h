<<<<<<< HEAD
﻿/*--
The MIT License (MIT)
=======
﻿/*++
Copyright (c) De  Giuli Informatica Ltda.
>>>>>>> origin/master

Copyright (c) 2012-2013 De Giuli Informática Ltda. (http://www.degiuli.com.br)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
--*/

#ifndef _DGI_UNITS_
#define _DGI_UNITS_

#include <cmath>
#include <ratio>

namespace dgi
{
    /*
    ** SI Prefix
    */
#ifndef _HAS_CPP0X
    const long double yocto   = std::pow(10,-24);       //y
    const long double zepto   = std::pow(10,-21);       //z
    const long double atto    = std::pow(10,-18);       //a
    const long double femto   = std::pow(10,-15);       //f
    const long double pico    = std::pow(10,-12);       //p
    const long double nano    = std::pow(10,-9);        //n
    const long double micro   = std::pow(10,-6);        //μ
    const long double milli   = std::pow(10,-3);        //m --> m is already used for meter
    const long double centi   = std::pow(10,-2);        //c
    const long double deci    = std::pow(10,-1);        //d
    const long double deca    = std::pow(10,1);         //da
    const long double hecto   = std::pow(10,2);         //h
    const long double kilo    = std::pow(10,3);         //k
    const long double mega    = std::pow(10,6);         //M
    const long double giga    = std::pow(10,9);         //G
    const long double tera    = std::pow(10,12);        //T
    const long double peta    = std::pow(10,15);        //P
    const long double exa     = std::pow(10,18);        //E
    const long double zetta   = std::pow(10,21);        //Z
    const long double yotta   = std::pow(10,24);        //Y
<<<<<<< HEAD
	
	//const double pi = M_PI;
	//const double pi = 3.141592653589793238462643383279502884e+00;
	//std::cout << std::setprecision(50)<< pi << std::endl;
	//constexpr double const_pi() { return std::atan(1)*4; }
=======
>>>>>>> origin/master
#endif

    /*
    ** SI Units
    */
<<<<<<< HEAD
	/*
	typedef mpl::vector_c<int,1,0,0,0,0,0,0> mass;
	typedef mpl::vector_c<int,0,1,0,0,0,0,0> length; // or position 
	typedef mpl::vector_c<int,0,0,1,0,0,0,0> time;
	typedef mpl::vector_c<int,0,0,0,1,0,0,0> charge;
	typedef mpl::vector_c<int,0,0,0,0,1,0,0> temperature;
	typedef mpl::vector_c<int,0,0,0,0,0,1,0> intensity;
	typedef mpl::vector_c<int,0,0,0,0,0,0,1> amount_of_substance;*/
	template<int M, int K, int S, int Amp=0, int Klv=0, int Mole=0, int CD=0>
=======
    template<int M, int K, int S, int Amp=0, int Klv=0, int Mole=0, int CD=0>
>>>>>>> origin/master
    struct Unit
    {
        enum{ m=M,              //meter - lenght
              kg=K,             //kilogram - mass
              s=S,              //second - time/duration
              A=Amp,            //ampere - current
              K=Klv,            //kelvin - temperature
              mol=Mole,         //mole - amount of substance
              cd=CD             //candela - luminous intense
        };
    };

    template<typename UNIT>
    struct Value
    {
        long double m_value;
        explicit Value(long double value) : m_value(value) {}
        
        static constexpr int get_m(){ return Unit::m; }
        static constexpr int get_Kg(){ return Unit::Kg; }
        static constexpr int get_s(){ return Unit::s; }
        static constexpr int get_A(){ return Unit::A; }
        static constexpr int get_K(){ return Unit::K; }
        static constexpr int get_mol(){ return Unit::mol; }
        static constexpr int get_cd(){ return Unit::cd; }

        //constructors
        explicit Value(long double d):val(d){}
        constexpr Value():val(0){}
    };

    //--- Length --- m
    using Length = Value<Unit<1,0,0> >;
    constexpr Value<Length> operator"" m(long double lenght)
    {
        return Value<Length>(lenght);
    }
    //--- m2
    using Area = Value<Unit<2,0,0> >;
    constexpr Value<Area> operator"" m2(long double area)
    {
        return Value<Area>(area);
    }
    //--- m3
    using Volume = Value<Unit<3,0,0> >;
    constexpr Value<Volume> operator"" m3(long double volume)
    {
        return Value<Volume>(volume);
    }

    //--- Time --- s
    using Time = Value<Unit<0,0,1> >;
    constexpr Value<Time> operator"" s(long double time)
    {
        return Value<Time>(time);
    }

    //--- Speed --- m/s
    using Speed = Value<Unit<1,0,-1> >;
    constexpr Value<Speed> operator"" ms_1(long double speed)
    {
        return Value<Speed>(speed);
    }

    //--- Accelaration --- m/s2
    using Acceleration = Value<Unit<1,0,-2> >;
    constexpr Value<Acceleration> operator"" ms_2(long double acceleration)
    {
        return Value<Acceleration>(acceleration);
    }

    //--- Mass Density --- kg/m3
    using MassDensity = Value<Unit<-3,1,0> >;
    constexpr Value<MassDensity> operator"" kgm_3(long double massdensity)
    {
        return Value<MassDensity>(massdensity);
    }

    //--- Surface Density --- kg/m2
    using SurfaceDensity = Value<Unit<-2,1,0> >;
    constexpr Value<SurfaceDensity> operator"" kgm_2(long double surfacedensity)
    {
        return Value<SurfaceDensity>(surfacedensity);
    }

    //--- Specific Volume --- m3/kg
    using SpecificVolume = Value<Unit<3,-1,0> >;
    constexpr Value<SpecificVolume> operator"" m3kg_1(long double specificvolume)
    {
        return Value<SpecificVolume>(specificvolume);
    }

    //--- Current Density --- A/m2
    using CurrentDensity = Value<Unit<-2,0,0,1> >;
    constexpr Value<CurrentDensity> operator"" Am_2(long double currentdensity)
    {
        return Value<CurrentDensity>(currentdensity);
    }

    //--- Magnetic Fiels Strength (H) --- A/m
    using MagneticFielsStrength = Value<Unit<-1,0,0,1> >;
    constexpr Value<MagneticFielsStrength> operator"" Am_1(long double magfieldstrength)
    {
        return Value<MagneticFielsStrength>(magfieldstrength);
    }

    //--- Concentration --- mol/m3
    using Concentration = Value<-3,0,0,0,0,1> >;
    constexpr Value<Concentration> operator"" molm_3(long double concentration)
    {
        return Value<Concentration>(concentration);
    }

    //--- Luminance --- Lv = cd/m2
    using Luminance = Value<-2,0,0,0,0,0,1> >;
    constexpr Value<Luminance> operator"" Lv(long double luminance)
    {
        return Value<Luminance>(luminance);
    }

    //--- Frquency (hertz) --- Hz = s-1
    using Frequency = Value<Unit<-1,0,0> >;
    constexpr Value<Frequency> operator"" Hz(long double frequency)
    {
        return Value<Frequency>(frequency);
    }

    //--- Froce (newton) --- N = kg.m/s2
    using Force = Value<Unit<1,1,-2> >;
    constexpr Value<Force> operator"" N(long double force)
    {
        return Value<Force>(force);
    }

    //--- Pressure (pascal) --- Pa = N/m2 = kg/m.s2
    using Pressure = Value<Unit<-1,1,-2> >;
    constexpr Value<Pressure> operator"" Pa(long double pressure)
    {
        return Value<Pressure>(pressure);
    }

    //--- Energy (joule) --- J = N.m = m2.kg/s2
    using Energy = Value<Unit<2,1,-2> >;
    constexpr Value<Energy> operator"" J(long double energy)
    {
        return Value<Energy>(energy);
    }

    //--- Power (watts) --- W = J/s = m2.kg/s3
    using Power = Value<Unit<2,1,-3> >;
    constexpr Value<Power> operator"" W(long double power)
    {
        return Value<Power>(power);
    }

    //--- Eletric Charge (coulomb) --- C = A.s
    using EletricCharge = Value<Unit<0,0,1,1> >;
    constexpt Value<EletricCharge> operator"" C(long double eletriccharge)
    {
        return Value<EletricCharge>(eletriccharge);
    }

    //--- Eletric Potential (volt) --- V = W/A = m2.kg/s3.A
    using EletricPotential = Value<Unit<2,1,-3,-1> >;
    constexpt Value<EletricPotential> operator"" V(long double eletricpotential)
    {
        return Value<EletricPotential>(eletricpotential);
    }

    //--- Capacitance (farad) --- F = C/V = A2.s4/m2.kg
    using Capacitance = Value<Unit<-2,-1,4,2> >;
    constexpt Value<Capacitance> operator"" F(long double capacitance)
    {
        return Value<Capacitance>(capacitance);
    }

    //--- Eletric Resistence (ohm) --- Ω = V/A = m2.kg/s3.A2
    using EletricResistence = Value<Unit<2,1,-3,-2> >;
    constexpt Value<EletricResistence> operator"" Ω(long double eletricresistence)
    {
        return Value<EletricResistence>(eletricresistence);
    }

    //--- Eletric Conductance (siemens) --- S = A/V = s3.A2/m2.kg
    using EletricConductance = Value<Unit<-2,-1,3,2> >;
    constexpt Value<EletricConductance> operator"" S(long double eletricconductance)
    {
        return Value<EletricConductance>(eletricconductance);
    }

    //--- Magnetic Flux (weber) --- Wb = V.s = m2.kg/s2.A
    using MagneticFlux = Value<Unit<2,1,-2,-1> >;
    constexpt Value<MagneticFlux> operator"" Wb(long double magneticflux)
    {
        return Value<MagneticFlux>(magneticflux);
    }

    //--- Magnetic Flux Density (tesla) --- T = Wb/m2 = kg/s2.A
    using MagneticFluxDensity = Value<Unit<0,1,-2,-1> >;
    constexpt Value<MagneticFluxDensity> operator"" T(long double magneticfluxdensity)
    {
        return Value<MagneticFluxDensity>(magneticfluxdensity);
    }

    //--- Indutance (henry) --- H = Wb/A
    using Indutance = Value<Unit<2,1,-2,-2> >;
    constexpt Value<Indutance> operator"" H(long double indutance)
    {
        return Value<Indutance>(indutance);
    }

    //--- Celcius Temperature (degree Celsius) --- ºC = K (0ºC = 273,15K)
    using CelciusTemp = Value<Unit<0,0,0,0,1> >;
    constexpt Value<CelciusTemp> operator"" ºC(long double celsius)
    {
        return Value<CelciusTemp>(celcius);
    }

    //--- Luminuous Flux (lumem) --- lm = cd.sr = cd
    using LuminuousFlux = Value<Unit<0,0,0,0,0,0,1> >;
    constexpt Value<LuminuousFlux> operator"" lm(long double luminuousflux)
    {
        return Value<LuminuousFlux>(luminuousflux);
    }

    //--- Illuminance (lux) --- lx = lm/m2 = cd/m2
    using Illuminance = Value<Unit<-2,0,0,0,0,0,1> >;
    constexpt Value<Illuminance> operator"" lx(long double illuminance)
    {
        return Value<Illuminance>(illuminance);
    }

    //--- RadionuclideActivity (becquerel) --- Bq = s-1
    using RadionuclideActivity = Value<Unit<-1,0,0> >;
    constexpr Value<RadionuclideActivity> operator"" Bq(long double radionuclideactivity)
    {
        return Value<RadionuclideActivity>(radionuclideactivity);
    }
<<<<<<< HEAD

/*
constexpr float PI = 3.1415926535897932;
constexpr float RAD2DEG = 180.0f / PI;
constexpr float DEG2RAD = PI / 180.0f;

class Degrees;

class Radians {
	float value;
public:
	constexpr explicit Radians(float radiansValue) :value(radiansValue) {}
	Radians(const Degrees& degreesValue); // defined below

	constexpr float getValue() const { return value; }
	
	constexpr Radians operator -() const { return Radians(-value); }
	constexpr Radians operator +(const Radians& rhs) const { return Radians(value + rhs.value); }
	constexpr Radians operator -(const Radians& rhs) const { return Radians(value - rhs.value); }
	constexpr Radians operator *(float rhs) const { return Radians(value * rhs); }
	constexpr Radians operator /(float rhs) const { return Radians(value / rhs); }
	constexpr float   operator /(const Radians& rhs) const 	{ return (value / rhs.value); }
	constexpr friend Radians operator*(float f, const Radians& d) { return Radians(d.value * f); }};

class Degrees {
	float value;
public:
	constexpr explicit Degrees(float degreesValue) :value(degreesValue) {}
	Degrees(const Radians& radiansValue) :value(radiansValue.getValue() * RAD2DEG) {}

	constexpr float getValue() const { return value; }
	
	constexpr Degrees operator -() const { return Degrees(-value); }
	constexpr Degrees operator +(const Degrees& rhs) const { return Degrees(value + rhs.value); }
	constexpr Degrees operator -(const Degrees& rhs) const { return Degrees(value - rhs.value); }
	constexpr Degrees operator *(float rhs) const { return Degrees(value * rhs); }
	constexpr Degrees operator /(float rhs) const { return Degrees(value / rhs); }
	constexpr float   operator /(const Degrees& rhs) const 	{ return (value / rhs.value); }
	constexpr friend Degrees operator*(float f, const Degrees& d) { return Degrees(d.value * f); }};

inline Radians::Radians(const Degrees& degreesValue) :value(degreesValue.getValue() * DEG2RAD) {}

inline Radians operator"" _rad(long double r) { return Radians(r); }
inline Radians operator"" _rad(unsigned long long r) { return Radians(r); }
inline Degrees operator"" _deg(long double d) { return Degrees(d); }
inline Degrees operator"" _deg(unsigned long long d) { return Degrees(d); }

*/
=======
>>>>>>> origin/master
}

#endif  //_DGI_UNITS_