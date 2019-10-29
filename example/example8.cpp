#undef NDEBUG
#include <assert.h>

#include "eswitch_v4.hpp"
#include <iostream>

enum Place{ california, washington, new_york, new_jersey, las_vegas };

int main( /* EXAMPLE NAME: ANY ORDER OF MATCHING ARGUMENTS IN CASE */ )
{
    using namespace eswitch_v4;

    const Place p1 = california;
    const Place p2 = washington;
    const Place p3 = new_york;

    {
        bool executed = true;

        eswitch( p1, p2, p3 ) >>
            case_( _1 == california && _3 == new_york && _2 == washington ) >> [&]{ executed = true; } >>
            default_ >> []{ assert( false ); };
            
        assert( executed );
    }

    {
        bool executed = true;

        eswitch( p1, p2, p3 ) >>
            case_( _3 == new_york && _1 == california && _2 == washington ) >> [&]{ executed = true; } >>
            default_ >> []{ assert( false ); };

        assert( executed );
    }

    {
        bool executed = true;

        eswitch( p1, p2, p3 ) >>
            case_( _3 == new_york && _2 == washington && _1 == california ) >> [&]{ executed = true; } >>
            default_ >> []{ assert( false ); };

        assert( executed );
    }
}