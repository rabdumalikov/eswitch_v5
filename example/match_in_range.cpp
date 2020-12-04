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

int main()
{
    using namespace eswitch_v5;

    int someVal = 10;

    {
        bool executed = false;

        eswitch( someVal )
        (
            Case(  _1.in( 6, 10 ) ) { assert( false ); },
            Case(  _1.in( 9, 25 ) ) { executed = true; },
            Default { assert( false ); }
        );

        assert( executed );
    }

    {
        bool executed = false;

        eswitch( someVal )
        (
            Case(  _1 > 6 && _1 < 10 ) { assert( false ); },
            Case(  _1 > 9 && _1 < 25 ) { executed = true; },
            Default { assert( false ); }
        );

        assert( executed );
    }

    {
        bool executed = false;

        eswitch( someVal )
        (
            Case(  _1.within( 6, 10 ) )  { executed = true; },
            Case(  _1.within( 11, 25 ) ) { assert( false ); },
            Default { assert( false ); }
        );

        assert( executed );
    }

    {
        bool executed = false;

        eswitch( someVal )
        (
            Case( _1 >= 6  && _1 <= 10 ) { executed = true; },
            Case( _1 >= 11 && _1 <= 25 ) { assert( false ); },
            Default { assert( false ); }
        );

        assert( executed );
    }
}
