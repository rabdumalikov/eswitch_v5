#undef NDEBUG
#include <assert.h>

#include "eswitch_v4.hpp"
#include "memory"
#include <iostream>

enum Place{ california, washington, new_york, new_jersey, las_vegas };

class Parser{};

int main( /* Logical operator "&&" and "||" in "case" */ )
{
    using namespace eswitch_v4;

    {
        std::unique_ptr< Parser > parser( new Parser{} );
        
        const Place p = california;

        bool executed = false;

        eswitch( p, parser ) >> 
            case_( _1 == california && _2 == nullptr ) >> [&]{ assert( false ); } >>
            case_( _1 == california && _2 != nullptr ) >> [&]{ executed = true; };

        assert( executed );
    }    

    {
        std::unique_ptr< Parser > parser( new Parser{} );
        
        const Place p = california;

        bool executed = false;

        eswitch( p, parser ) >> 
            case_( _1 == california || _2 == nullptr ) >> [&]{ executed = true; } >>
            case_( _1 == california && _2 != nullptr ) >> [&]{ assert( false ); };

        assert( executed );
    }   
}