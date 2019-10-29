#undef NDEBUG
#include <assert.h>

#include "eswitch_v4.hpp"
#include <iostream>

enum Place{ california, washington, new_york, new_jersey, las_vegas };

int main( /* EXAMPLE NAME: ANY FROM HELPER FUNCTION */ )
{
    using namespace eswitch_v4;

    for( const auto p : { california, washington, new_york } )
    {
        bool executed = false;

        eswitch( p ) >>
            case_( _1 == any_from( california, washington, new_york ) ) >> [&]{ executed = true; } >>
            default_ >> [&]{ assert( false ); };

        assert( executed );
    }
}