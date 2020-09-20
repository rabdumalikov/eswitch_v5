//  Copyright (c) 2019 Rustam Abdumalikov
//
//  "eswitch_v4" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "eswitch_v4.hpp"
//#include "gtest/gtest.h"
#include <memory>
#include <sstream>
#include <iostream>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2/catch.hpp"

enum Place { unknown = 0, new_york=5, washington=129, new_jersey=501 };

#define Case( cnds ) ( cnds ) >> [&]

/*
    eswitch( param1, param2 )
    [
        Case( _1 == 20 || _2 == 50 ) -> Fallthrough { return true; },
        Case( _1 == 0  && _2 == 0 ) { return true; }, 
        Default { return false; }
    ];

    int i = 0;
    eswitch( param1, param2 )
    [
        Case( _1 == 20 || _2 == 50 ){} ^ Fallthrough ^ Likely,
        Case( _1 == 0  && _2 == 0 ) { i = 66; Break; }, 
        Default { return false; }
    ];

    /// posible only with macroses
    eswitch( i, j ) 
    {
        Case( _1 == 20 || _2 == 50 ){} ^ Fallthrough ^ Likely,
        Case( _1 == 0  && _2 == 0 ) { i = 66; Break; }, 
        Default { return false; }
    };

    eswitch( i, j ) 
    (
        Case( _1 == 20 || _2 == 50 ){} ^ Fallthrough ^ Likely,
        Case( _1 == 0  && _2 == 0 ) { i = 66; Break; }, 
        Default { return false; }
    );
*/
struct potential_switch
{
    template< typename ... Ts >
    auto operator()( Ts &&... t )
    {
        return eswitch_v4::conditions_with_predicate{ std::forward< Ts >( t )... };
    }
};


TEST_CASE( "Factorials are computed", "[factorial]" ) 
{
    using namespace eswitch_v4;
    using namespace std;
    
    auto r = eswitch2( new_york )
    (
        Case( _1 == washington || _1 == new_york ) { std::cout << "Hi1\n"; return 1; },
        Case( _1 == washington ) { std::cout << "Hi2\n"; return 0.0; }, 
        Case( _1 == new_york ) { std::cout << "Hi3\n"; return 'c'; } 
    );
    
    std::cout << "R=" << r << std::endl;
}
