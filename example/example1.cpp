//  Copyright (c) 2019 Rustam Abdumalikov
//
//  "eswitch_v4" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
#include <assert.h>

#include "eswitch_v4.hpp"
#include "memory"
#include <iostream>

enum Place{ california, washington, new_york, new_jersey, las_vegas };

const char* example_ENUM_TO_STRING_via_lambda( const Place p )
{
    using namespace eswitch_v4;

    return eswitch( p ) >> 
        case_( _1 == california ) >> []{ return "california"; } >>
        case_( _1 == washington ) >> []{ return "washington"; } >>
        case_( _1 == new_york )   >> []{ return "new_york"; }   >>
        case_( _1 == new_jersey ) >> []{ return "new_jersey"; } >>
        case_( _1 == las_vegas )  >> []{ return "las_vegas"; }  >>
        default_ >> []{ return "unknown"; }
            >> in_place_return_;
}

const char* example_ENUM_TO_STRING_via_single_statement( const Place p )
{
    using namespace eswitch_v4;

    return eswitch( p ) >> 
         case_( _1 == california ) >> to_return( "california" ) >>
         case_( _1 == washington ) >> to_return( "washington" ) >>
         case_( _1 == new_york )   >> to_return( "new_york" )   >>
         case_( _1 == new_jersey ) >> to_return( "new_jersey" ) >>
         case_( _1 == las_vegas )  >> to_return( "las_vegas" )  >>
         default_ >> to_return( "unknown" )
            >> in_place_return_;
}

int main( /* EXAMPLE NAME: RETURN FROM ESWITCH */ )
{
    for( const auto p : { california, washington, new_york, new_jersey, las_vegas } )
    {
        assert( std::string{ example_ENUM_TO_STRING_via_lambda( p ) } == example_ENUM_TO_STRING_via_single_statement( p ) );
        std::cout << example_ENUM_TO_STRING_via_single_statement( p ) << std::endl;
    }
}