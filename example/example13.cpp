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

bool is_positive( const int i ) { return i > 0; }
bool is_odd( const int i ) { return i % 2 != 0; }

int main( /* EXAMPLE NAME: MIX PREDICATES WITH CONDITIONS */ )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( 7, 1 ) >>
        case_( _1 != 0 && ( is_positive, _1 ) && ( is_odd, _1 ) ) >> [&]{ executed = true; } >>
        default_ >> []{ assert( false ); };  

    assert( executed );
}
