//  Copyright (c) 2019-2021 Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[stringify_enum]
#include <cassert>
#include <string>
#include "eswitch_v5.hpp"

enum Place{ california, washington, new_york, new_jersey, las_vegas };

const char * to_string( const Place p )
{
    using namespace eswitch_v5;

    return eswitch( p ) 
        (   
            Case( california ) { return "california"; },
            Case( washington ) { return "washington"; },
            Case( new_york )   { return "new_york"; },
            Case( new_jersey ) { return "new_jersey"; },
            Case( las_vegas )  { return "las_vegas"; },
            Default            { return "unknown"; }
        );
}

int main()
{
    const std::string actualResult[] = { "california", "washington", "new_york", "new_jersey", "las_vegas" };

    for( const auto p : { california, washington, new_york, new_jersey, las_vegas } )
    {
        assert( std::string{ to_string( p ) } == actualResult[ static_cast< int >( p ) ] );
    }
}
///[stringify_enum]
