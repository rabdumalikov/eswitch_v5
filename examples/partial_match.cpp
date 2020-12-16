//  Copyright (c) 2019-2021 Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[partial_match]
#include <cassert>
#include <eswitch_v5.hpp>
#include <iostream>

enum Place{ california, washington, new_york, new_jersey, las_vegas };

int main()
{
    using namespace eswitch_v5;

    const Place p1 = california;
    const Place p2 = washington;
    const Place p3 = new_york;

    auto status = eswitch( p1, p2, p3 )
    (
        Case( _3 == new_york )                                        { return true; },
        Case( _1 == california && _3 == new_york )                    { return false; },       
        Case( _2 == washington && _3 == new_york )                    { return false; },       
        Case( _1 == california && _2 == washington )                  { return false; },
        Case( _1 == california && _2 == washington && _3 == new_york ){ return false; },
        Default { assert( false ); return false; }
    );

    assert( status );
}
///[partial_match]
