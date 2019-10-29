#undef NDEBUG
#include <assert.h>

#include "eswitch_v4.hpp"
#include <iostream>

enum Place{ california, washington, new_york, new_jersey, las_vegas };

int main( /* EXAMPLE NAME: PARTIAL MATCH */ )
{
    using namespace eswitch_v4;

    const Place p1 = california;
    const Place p2 = washington;
    const Place p3 = new_york;

    eswitch( p1, p2, p3 ) >>
        case_( _1 == california )                                       >> []{} >>
        case_( _2 == washington )                                       >> []{} >>
        case_( _3 == new_york )                                         >> []{} >>
        case_( _1 == california && _3 == new_york )                     >> []{} >>        
        case_( _2 == washington && _3 == new_york )                     >> []{} >>        
        case_( _1 == california && _2 == washington )                   >> []{} >>
        case_( _1 == california && _2 == washington && _3 == new_york ) >> []{} >>
        default_ >> []{ assert( false ); };  
}