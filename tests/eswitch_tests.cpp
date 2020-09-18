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
    template< typename T >
    auto operator[]( T && t )
    {
        return std::move( t );
    }

    template< typename ... Ts >
    auto operator()( Ts &&... t )
    {
        return eswitch_v4::conditions_with_predicate{ std::forward< Ts >( t )... };
    }
};


TEST_CASE( "Factorials are computed", "[factorial]" ) {
    // REQUIRE( Factorial(1) == 1 );
    // REQUIRE( Factorial(2) == 2 );
    using namespace eswitch_v4;
    //using namespace std;

    potential_switch ps;

    auto r = ps[
        Case( _1 == washington || _1 == new_york ) { std::cout << "Hi1\n"; } ^ fallthrough_,
        Case( _1 == washington ) { std::cout << "Hi2\n"; }//, 
        //Case( _1 == new_york ) { std::cout << "Hi3\n"; } 
        ];

    auto r2 = ps(
        Case( _1 == washington || _1 == new_york ) { std::cout << "Hi1\n"; } ^ fallthrough_,
        Case( _1 == washington ) { std::cout << "Hi2\n"; }, 
        Case( _1 == new_york ) { std::cout << "Hi3\n"; } 
    );


    // auto rr = {
    //     Case( _1 == washington || _1 == new_york ) { std::cout << "Hi1\n"; } ^ fallthrough_,
    //     Case( _1 == washington ) { std::cout << "Hi2\n"; }//, 
    //     //Case( _1 == new_york ) { std::cout << "Hi3\n"; } 
    // };
    r( std::make_tuple( washington ) );


    bool executed = false;

    //eswitch( washington, new_jersey, new_york )
      //  >> case_( _1 == washington ) >> [&](){ executed = true; };
    
    //ASSERT_EQ(executed, true );

}
