//  Copyright (c) 2019-2021 Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[explicit_fallthrough]
#include <cassert>
#include "eswitch_v5.hpp"

enum Place{ california, washington, new_york, new_jersey, las_vegas };

int main()
{    
    using namespace eswitch_v5;

    Place p = california;

    {
        bool executed_1st_case = false;
        bool executed_2nd_case = false;

        eswitch( p )
        ( 
            Case( california ) { executed_1st_case = true; } ^ fallthrough_,
            Case( new_york )   { executed_2nd_case = true; },
            Default            { assert( false ); }
        );

        assert( executed_1st_case && executed_2nd_case );
    }

    {
        bool executed_1st_case = false;
        bool executed_2nd_case = false;
        bool executed_default_case = false;

        eswitch( p )
        ( 
            Case( california ) { executed_1st_case = true; } ^ fallthrough_,
            Case( new_york )   { executed_2nd_case = true; } ^ fallthrough_,
            Default            { executed_default_case = true; }
        );


        assert( executed_1st_case && executed_2nd_case && executed_default_case );
    }    
}
///[explicit_fallthrough]
