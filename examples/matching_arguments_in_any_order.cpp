//  Copyright (c) 2020 Rustam Abdumalikov
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

enum Place{ california, washington, new_york, new_jersey, las_vegas };

int main()
{
    using namespace eswitch_v5;

    const Place p1 = california;
    const Place p2 = washington;
    const Place p3 = new_york;

    {
        bool executed = true;

        eswitch( p1, p2, p3 )
        (
            Case( _1 == california && _3 == new_york && _2 == washington ) { executed = true; },
            Default { assert( false ); }
        );

        assert( executed );
    }

    {
        bool executed = true;

        eswitch( p1, p2, p3 )
        (
            Case( _3 == new_york && _1 == california && _2 == washington ) { executed = true; },
            Default { assert( false ); }
        );

        assert( executed );
    }

    {
        bool executed = true;

        eswitch( p1, p2, p3 )
        (
            Case( _3 == new_york && _2 == washington && _1 == california ) { executed = true; },
            Default { assert( false ); }
        );

        assert( executed );
    }
}