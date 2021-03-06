//  Copyright (c) 2019-present Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[implicit_break]
#include <cassert>
#include <eswitch_v5.hpp>

enum Place{ california, washington, new_york, new_jersey, las_vegas };

int main()
{    
    using namespace eswitch_v5;

    Place p = california;

    eswitch( p )
    (
        Case( _1 == california ) {},
        Case( _1 == california ) { assert( false ); },
        Default                  { assert( false ); }
    );
}
///[implicit_break]
