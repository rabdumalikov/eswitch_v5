#undef NDEBUG
#include <assert.h>

#include "eswitch_v4.hpp"

enum Place{ california, washington, new_york, new_jersey, las_vegas };

int main( /* EXAMPLE NAME: EXPLICIT FALLTHROUGH */ )
{    
    using namespace eswitch_v4;

    Place p = california;

    {
        bool executed_1st_case = false;
        bool executed_2nd_case = false;

        eswitch( p ) >> 
            case_( _1 == california ) >> [&]{ executed_1st_case = true; } >> fallthrough_ >>
            case_( _1 == california ) >> [&]{ executed_2nd_case = true; } >> // here goes implicit break, thats why "default" won't be executed!
            default_                  >> [&]{ assert( false ); };

        assert( executed_1st_case && executed_2nd_case );
    }

    {
        bool executed_1st_case = false;
        bool executed_2nd_case = false;
        bool executed_default_case = false;

        eswitch( p ) >> 
            case_( _1 == california ) >> [&]{ executed_1st_case = true; } >> fallthrough_ >>
            case_( _1 == california ) >> [&]{ executed_2nd_case = true; } >> fallthrough_ >>
            default_                  >> [&]{ executed_default_case = true; };

        assert( executed_1st_case && executed_2nd_case && executed_default_case );
    }    
}