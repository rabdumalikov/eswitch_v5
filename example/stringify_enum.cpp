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
#include "memory"
#include <iostream>

enum Place{ california, washington, new_york, new_jersey, las_vegas };

const char * example_ENUM_TO_STRING_via_lambda( const Place p )
{
    using namespace eswitch_v5;

    return eswitch( p ) 
        (   
            Case( california ) { return "california"; },
            Case( washington ) { return "washington"; },
            Case( new_york )   { return "new_york"; },
            Case( new_jersey ) { return "new_jersey"; },
            Case( las_vegas )  { return "las_vegas"; },
            Default { return "unknown"; }
        );
}

int main( /* EXAMPLE: Stringify with eswitch */ )
{
    const std::string actualResult[] = { "california", "washington", "new_york", "new_jersey", "las_vegas" };

    for( const auto p : { california, washington, new_york, new_jersey, las_vegas } )
    {
        assert( std::string{ example_ENUM_TO_STRING_via_lambda( p ) } == 
            actualResult[ static_cast< int >( p ) ] );

        std::cout << example_ENUM_TO_STRING_via_lambda( p ) << std::endl;
    }
}