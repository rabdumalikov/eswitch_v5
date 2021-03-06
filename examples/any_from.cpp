//  Copyright (c) 2019-present Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[any_from]
#include <cassert>
#include <eswitch_v5.hpp>

enum Place{ california, washington, new_york, new_jersey, las_vegas };

int main()
{
    using namespace eswitch_v5;

    for( const auto p : { california, washington, new_york } )
    {
        bool executed = false;

        eswitch( p )
        (
            Case( _1 == any_from( california, washington, new_york ) ) { executed = true; },
            Default { assert( false ); }
        );

        assert( executed );
    }
}
///[any_from]
