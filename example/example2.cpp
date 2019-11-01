//  Copyright (c) 2019 Rustam Abdumalikov
//
//  "eswitch_v4" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
#include <assert.h>

#include "eswitch_v4.hpp"
#include <iostream>

enum Place{ california, washington, new_york, new_jersey, las_vegas };

int main( /* EXAMPLE NAME: DEFAULT CASE */ )
{
    using namespace eswitch_v4;

    const Place p = las_vegas;

    bool executed = false;

    eswitch( p ) >> 
        case_( _1 == california ) >> []{ assert( false ); } >>
        case_( _1 == washington ) >> []{ assert( false ); } >>
        case_( _1 == new_york )   >> []{ assert( false ); } >>
        case_( _1 == new_jersey ) >> []{ assert( false ); } >>
        default_ >> [&]{ executed = true; };

    assert( executed );
}