//  Copyright (c) 2019-present Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[match_for_std_tuple]
#include <cassert>
#include <string>
#include <eswitch_v5.hpp>

int main()
{
    using namespace eswitch_v5;

    { // match
    auto tup = std::make_tuple( 1, 0, 0, 1 );

    auto result = eswitch( tup )
    (
        Case( 1, 0, 0, 1 ) { return 9; },
        Case( 1, 1, 1, 1 ) { return 15; },     
        Default { return -1; }
    );

    assert( result == 9 );
    }


    { // partial-match
    auto tup = std::make_tuple( 1, 0, 0, 1 );

    auto result = eswitch( tup )
    (
        Case( _2 == 0 && _3 == 0 ) { return true; },   
        Default { return false; }
    );

    assert( result );
    }

    { // no-match
    auto tup = std::make_tuple( 1, 0, 0, 1 );

    auto result = eswitch( tup )
    (
        Case( 1, 1, 0, 1 ) { return 13; },
        Case( 1, 1, 1, 1 ) { return 15; },     
        Default { return -1; }
    );

    assert( result == -1 );
    }
}
///[match_for_std_tuple]
