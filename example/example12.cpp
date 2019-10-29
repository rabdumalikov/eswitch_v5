#undef NDEBUG
#include <assert.h>

#include "eswitch_v4.hpp"
#include <iostream>

enum Place{ california, washington, new_york, new_jersey, las_vegas };

bool is_nonzero( const int i ) { return i != 0; }
bool is_positive( const int i, const int j ) { return i > 0 && j > 0; }

int main( /* EXAMPLE NAME: FREE FUNCTION PREDICATES */ )
{
    using namespace eswitch_v4;

    {
        bool executed = false;

        eswitch( 0, 1 ) >>
            case_( ( is_nonzero, _1 ) || ( is_nonzero, _2 )  ) >> [&]{ executed = true; } >>
            default_ >> []{ assert( false ); };  

        assert( executed );
    }

    {
        bool executed = false;

        eswitch( 1, 2 ) >>
            case_( ( is_positive, _1, _2 ) ) >> [&]{ executed = true; } >>
            default_ >> []{ assert( false ); };  

        assert( executed );

    }
}