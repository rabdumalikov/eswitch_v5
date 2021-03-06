//  Copyright (c) 2019-present Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[match_by_predicate_as_free_function]
#include <cassert>
#include <eswitch_v5.hpp>

enum Place{ california, washington, new_york, new_jersey, las_vegas };

bool is_nonzero( const int i ) { return i != 0; }
bool is_positive( const int i, const int j ) { return i > 0 && j > 0; }

int main()
{
    using namespace eswitch_v5;

    {
        bool executed = false;

        eswitch( 0, 1 ) 
        (
            Case( ( is_nonzero, _1 ) || ( is_nonzero, _2 )  ) { executed = true; },
            Default { assert( false ); }  
        );

        assert( executed );
    }

    {
        bool executed = false;

        eswitch( 1, 2 ) 
        (
            Case( ( is_positive, _1, _2 ) ) { executed = true; },
            Default { assert( false ); }  
        );

        assert( executed );

    }
}
///[match_by_predicate_as_free_function]