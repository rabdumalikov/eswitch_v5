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

int main( /* EXAMPLE NAME:  case( _1.in( 1, 2 ) ) */ )
{
    using namespace eswitch_v5;

    int someVal = 20;

    bool executed = false;

    eswitch( someVal ) >>
        case_( _1.in( 1, 10 ) )  >> [&]{ assert( false ); } >>
        case_( _1.in( 11, 22 ) ) >> [&]{ executed = true; } >>
        default_ >> []{ assert( false ); };

    assert( executed );
}
