#undef NDEBUG
#include <assert.h>

#include "eswitch_v4.hpp"
#include <iostream>

enum Place{ california, washington, new_york, new_jersey, las_vegas };

int main( /* EXAMPLE NAME: EMPTY CASE BODY */ )
{
    using namespace eswitch_v4;

    for( const auto p : { california, washington, new_york } )
    {
        eswitch( p ) >>
            case_( _1 == any_from( california, washington, new_york ) ) >>
            case_( _1 == california ) >> []{ assert( false ); } >>
            default_ >> []{ assert( false ); };


        /// Code below won't compile on purpose, since I try 
        /// to prevent user from writting code which doesn't have sence.

        // eswitch( p ) >>
        //     case_( _1 == any_from( california, washington, new_york ) ) >>
        //     default_ >> []{ ... };
    }
}