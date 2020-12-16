//  Copyright (c) 2019-2021 Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[match_mixed_condition]
#include <cassert>
#include <eswitch_v5.hpp>

enum Place{ california, washington, new_york, new_jersey, las_vegas };

bool is_positive( const int i ) { return i > 0; }
bool is_odd( const int i ) { return i % 2 != 0; }

int main()
{
    using namespace eswitch_v5;

    bool executed = false;

    eswitch( 7, 1 )
    (
        Case( _1 != 0 && ( is_positive, _1 ) && ( is_odd, _1 ) ) { executed = true; },
        Default { assert( false ); }
    );

    assert( executed );
}
///[match_mixed_condition]