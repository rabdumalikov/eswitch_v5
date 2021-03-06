//  Copyright (c) 2019-present Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[default]
#include <cassert>
#include <eswitch_v5.hpp>

enum Place{ california, washington, new_york, new_jersey, las_vegas };

int main()
{
    using namespace eswitch_v5;

    const Place p = las_vegas;

    bool executed = false;

    eswitch( p )
    ( 
        Case( california ) { assert( false ); },
        Case( washington ) { assert( false ); },
        Case( new_york )   { assert( false ); },
        Case( new_jersey ) { assert( false ); },
        Default            { executed = true; }
    );

    assert( executed );
}
///[default]
