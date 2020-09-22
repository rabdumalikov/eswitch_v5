//  Copyright (c) 2019 Rustam Abdumalikov
//
//  "eswitch_v4" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "eswitch_v4.hpp"
#include <memory>
#include <sstream>
#include <iostream>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2/catch.hpp"

enum Place { unknown = 0, new_york=5, washington=129, new_jersey=501 };

#define Case( cnds ) ( cnds ) >> [&]
#define Default  ( _1 == extension::any{} ) >> [&]

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

TEST_CASE( "eswitch_v5::equal_match", "" ) 
{
    using namespace eswitch_v4;
    using namespace std;

    SECTION( "match_first_case" )
    {
        auto r = 
        eswitch2( washington )
        (
            Case( _1 == washington ) { return 123; },
            Case( _1 == new_york )   { return 'c'; } 
        );

        REQUIRE( r == 123 );    
    }

    SECTION( "match_second_case" )
    {
        auto r = 
        eswitch2( new_york )
        (
            Case( _1 == washington ) { return 123; },
            Case( _1 == new_york )   { return 'c'; } 
        );

        REQUIRE( r == 'c' );    
    }
}

TEST_CASE( "eswitch_v5::fallthrough", "" ) 
{
    using namespace eswitch_v4;
    using namespace std;

    SECTION( "match_1st_and_2nd_case" )
    {
        int i = 0;

        eswitch2( washington )
        (
            Case( _1 == washington ) { i += 1; } ^ fallthrough_,
            Case( _1 != new_york )   { i += 2; } 
        );

        REQUIRE( i == 3 );    
    }

    SECTION( "no_match" )
    {
        int i = 0;

        eswitch2( washington )
        (
            Case( _1 != washington ) { i += 1; } ^ fallthrough_,
            Case( _1 == new_york )   { i += 2; } 
        );

        REQUIRE( i == 0 );    
    }

    SECTION( "no_match" )
    {
        int i = 0;

        eswitch2( washington )
        (
            Case( _1 != washington ) { i += 1; } ^ fallthrough_,
            Case( _1 == new_york )   { i += 2; },
            Default { i += 6; }
        );

        REQUIRE( i == 6 );    
    }
}


TEST_CASE( "eswitch_v5::not_equal_match", "" ) 
{
    using namespace eswitch_v4;
    using namespace std;

    SECTION( "match_first_case" )
    {
        auto r = 
        eswitch2( washington )
        (
            Case( _1 != washington ) { return 123; },
            Case( _1 != new_york )   { return 'c'; } 
        );

        REQUIRE( r == 'c' );    
    }

    SECTION( "match_second_case" )
    {
        auto r = 
        eswitch2( new_york )
        (
            Case( _1 != washington ) { return 123; },
            Case( _1 != new_york )   { return 'c'; } 
        );

        REQUIRE( r == 123 );    
    }
}

TEST_CASE( "eswitch_v5::default_match", "" ) 
{
    using namespace eswitch_v4;
    using namespace std;

    SECTION( "match_default_case" )
    {
        auto r = 
        eswitch2( new_jersey )
        (
            Case( _1 != new_jersey ) { return 123; },
            Default { return 5.1; }
        );

        REQUIRE( r == 5.1 );    
    }

    SECTION( "match_default_case" )
    {
        auto r = 
        eswitch2( new_jersey )
        (
            Case( _1 == washington ) { return 123; },
            Case( _1 == new_york )   { return 'c'; },
            Default { return 57.5; }
        );

        REQUIRE( r == 57.5 );    
    }

}

TEST_CASE( "eswitch_v5::not_equal_match2", "" ) 
{
    using namespace eswitch_v4;
    using namespace std;

    auto is_even = []( auto v ) { return v % 2 == 0; };
    auto is_odd  = []( auto v ) { return !(v % 2 == 0); };

    SECTION( "match_first_case" )
    {
        auto r = 
        eswitch2( 2 )
        (
            Case( ( is_even, _1 ) ) { return true; },
            Case( ( is_odd, _1 ) ) { return false; } 

        );

        REQUIRE( r == true );    
    }

    SECTION( "match_second_case" )
    {
        auto r = 
        eswitch2( 3 )
        (
            Case( ( is_even, _1 ) ) { return true; },
            Case( ( is_odd, _1 ) ) { return false; } 

        );

        REQUIRE( r == false );    
    }
}
