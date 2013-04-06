/*++
Copyright (c) De  Giuli Informatica Ltda.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#ifndef _DGI_UNITS_
#define _DGI_UNITS_

namespace dgi
{
    //SI Units
    template<int M, int K, int S>
    struct Unit
    {
        enum{ m=M, kg=K, s=S };
    };

    template<typename Unit>
    struct Value
    {
        long double m_value;
        explicit Value(long double value) : m_value(value) {}
    };

    //--- Length ---
    using Length = Value<Unit<1,0,0> >;     //m
    constexpr Value<Length> operator"" m(long double lenght)
    {
        return Value<Length>(lenght);
    }
    using Area = Value<Unit<2,0,0> >;       //m2
    constexpr Value<Area> operator"" m2(long double area)
    {
        return Value<Area>(area);
    }
    using Volume = Value<Unit<3,0,0> >;     //m3
    constexpr Value<Volume> operator"" m3(long double volume)
    {
        return Value<Volume>(volume);
    }

    //--- Time ---
    using Time = Value<Unit<0,0,1> >;       //s
    constexpr Value<Time> operator"" s(long double time)
    {
        return Value<Time>(time);
    }
    using Time2 = Value<Unit<0,0,2> >;      //s2
    constexpr Value<Time2> operator"" s2(long double time2)
    {
        return Value<Time2>(time2);
    }

    //--- Speed ---
    using Speed = Value<Unit<1,0,-1> >;     //m/s
    constexpr Value<Speed> operator"" m-s(long double speed)
    {
        return Value<Speed>(speed);
    }

    //--- Accelaration ---
    using Acceleration = Value<Unit<1,0,-2> >;     //m/s2
    constexpr Value<Acceleration> operator"" m-s2(long double acceleration)
    {
        return Value<Acceleration>(acceleration);
    }
}

#endif  //_DGI_UNITS_