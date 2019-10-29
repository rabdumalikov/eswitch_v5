#undef NDEBUG
#include <assert.h>

#include "eswitch_v4.hpp"
#include <iostream>

enum Place{ california, washington, new_york, new_jersey, las_vegas };

int main( /* EXAMPLE NAME: SIMPLIFIED SYNTAX IF CASE HAVE ONLY ONE ARGUMENT */ )
{
    using namespace eswitch_v4;

    const Place p1 = california;
    const Place p2 = washington;
    const Place p3 = new_york;


    {
        bool executed = true;

        eswitch( p1 ) >>
            case_( california ) >> [&]{ executed = true; } >>
            case_( washington ) >> [ ]{ assert( false ); } >>
            case_( new_york )   >> [ ]{ assert( false ); } >>
            default_            >> [ ]{ assert( false ); };

        assert( executed );

        /* basically code above will be interpreted as:

            eswitch( p1 ) >>
                case_( _1 == california ) >> [&]{ executed = true; } >>
                case_( _1 == washington ) >> [ ]{ assert( false ); } >>
                case_( _1 == new_york )   >> [ ]{ assert( false ); } >>
                default_                  >> [ ]{ assert( false ); };
        */ 
    }

    {
        bool executed = true;

        eswitch( p1, p2, p3 ) >>
            case_( california ) >> [&]{ executed = true; } >>
            case_( _3 == new_york && _1 == california && _2 == washington ) >> [&]{ assert( false ); } >>            
            default_ >> []{ assert( false ); };

        assert( executed );
    }

  
}