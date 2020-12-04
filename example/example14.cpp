//  Copyright (c) 2019 Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
#include <assert.h>

#include "eswitch_v5.hpp"
#include <iostream>
#include <cmath>

struct fcmp
{
    double value;
    explicit fcmp( const double input ) : value( input ) {}

    friend bool operator==( const double v, const fcmp cf )
    {
        const double epsilon = 0.0001f;
        return fabs( cf.value - v ) < epsilon;
    }

    friend bool operator!=( const double v, const fcmp cf )
    {
        return !( v == cf );
    }
};

CASE_OVERLOAD( double, fcmp );

int main( /* EXAMPLE NAME: CUSTOM EXTENTION( FOR DOUBLE ) */ )
{
    using namespace eswitch_v5;

    bool executed = false;

    double d = 2.0002;

    eswitch( d ) >> 
        case_( _1 == 2.0001 ) >> []{ assert( false ); } >>
        case_( _1 == 2.0002 ) >> [&]{ executed = true; };
    assert( executed );
}
