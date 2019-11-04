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

template< int From, int To >
bool in_range( const int value )
{
    return value >= From && value <= To;
}

int main( /* EXAMPLE NAME: CUSTOM EXTENTION( FOR REGEX ) */ )
{
    using namespace eswitch_v4;

    int someVal = 20;

    bool executed = false;

    eswitch( someVal ) >>
        case_( ( in_range< 6, 10 >, _1 ) ) >> []{ assert( false ); } >>
        case_( ( in_range< 10, 25 >, _1 ) )  >> [&]{ executed = true; } >>
        default_ >> []{ assert( false ); };

    assert( executed );
}