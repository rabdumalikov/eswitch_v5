#undef NDEBUG
#include <assert.h>

#include "eswitch_v4.hpp"

enum Place{ california, washington, new_york, new_jersey, las_vegas };

int main( /* EXAMPLE NAME: NO IMPLICIT BREAK */ )
{    
    using namespace eswitch_v4;

    Place p = california;

    eswitch( p ) >> 
        case_( _1 == california ) >> []{} >>
        case_( _1 == california ) >> []{ assert( false ); } >>
        default_                  >> []{ assert( false ); };
}