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

int main( /* EXAMPLE NAME: LVALUE case( _1.in( i, j ) ) */ )
{
    using namespace eswitch_v5;

    int someVal = 20;

    bool executed = false;

    int i = 1;
    int j = 22;

    eswitch( someVal ) >>
        case_( _1.in( i, j ) ) >> [&]{ executed = true; } >>
        default_ >> []{ assert( false ); };

    assert( executed );
}
