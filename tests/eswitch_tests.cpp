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

#define Case( ... ) case_( __VA_ARGS__ ) % [&]
#define Default  ( _1 == extension::any{} ) % [&]

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
        eswitch( washington )
        (
            Case( _1 == washington ) { return 123; },
            Case( _1 == new_york )   { return 'c'; } 
        );

        REQUIRE( r == 123 );    
    }

    SECTION( "match_second_case" )
    {
        auto r = 
        eswitch( new_york )
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

        eswitch( washington )
        (
            Case( _1 == washington ) { i += 1; } ^ fallthrough_,
            Case( _1 != new_york )   { i += 2; } 
        );

        REQUIRE( i == 3 );    
    }

    SECTION( "no_match" )
    {
        int i = 0;

        eswitch( washington )
        (
            Case( _1 != washington ) { i += 1; } ^ fallthrough_,
            Case( _1 == new_york )   { i += 2; } 
        );

        REQUIRE( i == 0 );    
    }

    SECTION( "no_match" )
    {
        int i = 0;

        eswitch( washington )
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
        eswitch( washington )
        (
            Case( _1 != washington ) { return 123; },
            Case( _1 != new_york )   { return 'c'; } 
        );

        REQUIRE( r == 'c' );    
    }

    SECTION( "match_second_case" )
    {
        auto r = 
        eswitch( new_york )
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
        eswitch( new_jersey )
        (
            Case( _1 != new_jersey ) { return 123; },
            Default { return 5.1; }
        );

        REQUIRE( r == 5.1 );    
    }

    SECTION( "match_default_case" )
    {
        auto r = 
        eswitch( new_jersey )
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
        eswitch( 2 )
        (
            Case( ( is_even, _1 ) ) { return true; },
            Case( ( is_odd, _1 ) ) { return false; } 

        );

        REQUIRE( r == true );    
    }

    SECTION( "match_second_case" )
    {
        auto r = 
        eswitch( 3 )
        (
            Case( ( is_even, _1 ) ) { return true; },
            Case( ( is_odd, _1 ) ) { return false; } 

        );

        REQUIRE( r == false );    
    }
}



TEST_CASE( "eswitch_v5::std_any_failure", "" ) 
{
    using namespace eswitch_v4;
    using namespace std;

    std::any a = 10;

    REQUIRE_THROWS_AS( eswitch( a )
    (
        Case( _1 == my_type< float >{} ) { return true; }
    ), std::bad_optional_access );    
}

TEST_CASE( "eswitch_v5::std_any_default", "" ) 
{
    using namespace eswitch_v4;
    using namespace std;

    std::any a = 10;

    REQUIRE_FALSE( 
        eswitch( a )
        (
            Case( _1 == my_type< float >{} ) { return true; },
            Default { return false; }
        ) 
    );    
}

TEST_CASE( "eswitch_v5::std_any_success", "" ) 
{
    using namespace eswitch_v4;
    using namespace std;

    SECTION( "match_2nd_case" )
    {
        std::any a = 10;

        REQUIRE( 
            eswitch( a )
            (
                Case( _1 == my_type< float >{} ) { return false; },
                Case( _1 == my_type< int >{} ) { return true; },
                Default { return false; }
            ) 
        );    
    }

    SECTION( "match_1st_case" )
    {
        float f = .7;
        std::any a = f;

        REQUIRE( 
            eswitch( a )
            (
                Case( _1 == my_type< float >{} ) { return true; },
                Case( _1 == my_type< int >{} ) { return false; },
                Default { return false; }
            ) 
        );    
    }

    SECTION( "match_1st_case" )
    {
        float f = .7;
        std::any a = f;

        REQUIRE( 
            eswitch( a )
            (
                Case( _1 == my_type< float >{} || _1 == my_type< int >{} ) { return true; },
                Default { return false; }
            ) 
        );    
    }

    SECTION( "match_complex_type" )
    {
        std::any a = std::string{ "Hello" };

        REQUIRE( 
            eswitch( a )
            (
                Case( _1 == my_type< float >{} ) { return false; },
                Case( _1 == my_type< std::string >{} ) { return true; },
                Default { return false; }
            ) 
        );    
    }

}

TEST_CASE( "eswitch_v5::std_any_only", "" ) 
{
    using namespace eswitch_v4;
    using namespace std;

    SECTION( "match_2nd_case" )
    {
        std::any a = 10;

        auto r = eswitch( a )
        (
            Case( _1 == my_type< float >{} )  { return 0; },
            Case( _1 == my_type< int >{} )( const int val ) { return val + 11; },
            Default { return 0; }
        );

        REQUIRE( r == 21 ); 
    }

    SECTION( "match_3rd_case" )
    {
        std::any a = std::string{"STR"};

        auto r = eswitch( a )
        (
            Case( _1 == my_type< float >{} ) { return ""; },
            Case( _1 == my_type< int >{} )( const int val ) { return ""; },
            Case( _1 == my_type< std::string >{} )( const std::string & val ) { return val + "hello"; },
            Default { return ""; }
        );

        REQUIRE( r == "STRhello" ); 
    }
}

TEST_CASE( "eswitch_v5::std_variant_only", "" ) 
{
    using namespace eswitch_v4;
    using namespace std;

    SECTION( "match_2nd_case" )
    {
        std::variant< int, double > a = 19.9;

        auto r = eswitch( a )
        (
            Case( _1 == my_type< double >{} )( const double val ) { return val - 1; },
            Case( _1 == my_type< int >{} )  { return 0; },
            Default { return 0; }
        );

        REQUIRE( r == 18.9 ); 
    }

    SECTION( "match_3rd_case" )
    {
        std::variant< int, float, std::string > a = std::string{"STR"};

        auto r = eswitch( a )
        (
            Case( _1 == my_type< float >{} ) { return ""; },
            Case( _1 == my_type< int >{} )( const int val ) { return ""; },
            Case( _1 == my_type< std::string >{} )( const std::string & val ) { return val + "hello"; },
            Default { return ""; }
        );

        REQUIRE( r == "STRhello" ); 
    }

    SECTION( "match_Default_case" )
    {
        std::variant< int, double, std::string > a = 16.7;

        auto r = eswitch( a )
        (
            //Case( _1 == my_type< float >{} ) { return ""; },
            Case( _1 == my_type< int >{} )( const int val ) { return ""; },
            Case( _1 == my_type< std::string >{} )( const std::string & val ) { return val + "hello"; },
            Default { return "Default"; }
        );

        REQUIRE( r == "Default" ); 
    }
}

TEST_CASE( "eswitch_v5::std_variant_plus_std_any", "" ) 
{
    using namespace eswitch_v4;
    using namespace std;

    SECTION( "match_2nd_case" )
    {
        std::any a = std::string{ "S" };
        std::variant< int, double > v = 19.9;

        auto r = eswitch( a, v )
        (
            Case( _1 == my_type< std::string >{} && _2 == my_type< double >{} ) { return true; },
            Case( _1 == my_type< int >{} && _2 == my_type< int >{} ) { return false; },
            Default { return false; }
        );

        REQUIRE( r ); 
    }

    SECTION( "match_2nd_case" )
    {
        std::any a = std::string{ "S" };
        std::variant< int, double > v = 19.9;

        auto r = eswitch( a )
        (
            Case( _1 == my_type< std::string >{} )( const std::string & val ) { return val; },
            Case( _1 == my_type< int >{} ) { return ""; },
            Default { return ""; }
        );
        
        REQUIRE( r == "S" ); 
    }
}

TEST_CASE( "eswitch_v5::default_cast_at_any_position", "" ) 
{
    using namespace eswitch_v4;
    using namespace std;

    SECTION( "default_at_1st_position" )
    {
        int i = 10;
        auto r = eswitch( i )
        (
            Default { return false; },
            Case( _1 == 10 ) { return true; }
        );

        REQUIRE( r ); 
    }

    SECTION( "default_at_2nd_position" )
    {
        int i = 10;
        auto r = eswitch( i )
        (
            Case( _1 == 14 ) { return 1; },
            Default { return 2; },
            Case( _1 == 10 ) { return 3; }
        );

        REQUIRE( r == 3 ); 
    }

    SECTION( "default_at_3rd_position" )
    {
        int i = 10;
        auto r = eswitch( i )
        (
            Case( _1 == 14 ) { return 1; },
            Case( _1 == 12 ) { return 2; },
            Default { return 3; },
            Case( _1 == 10 ) { return 4; }
        );

        REQUIRE( r == 4 ); 
    }

    SECTION( "default_at_3rd_2_position" )
    {
        int i = 10;
        auto r = eswitch( i )
        (
            Case( _1 == 14 ) { return 1; },
            Case( _1 == 10 ) { return 4; },
            Default { return 3; },
            Case( _1 == 12 ) { return 2; }
        );

        REQUIRE( r == 4 ); 
    }
}

TEST_CASE( "eswitch_v5::std_pair", "" ) 
{
    using namespace eswitch_v4;
    using namespace std;

    std::pair pr{ 10, std::string{"Hello" } };

    SECTION( "1st_position" )
    {
        auto r = eswitch( pr )
        (
           Case( _1 == 10 && _2 == "Hello" ) { return true; },
           Default { return false; }
        );

        REQUIRE( r ); 
    }

    SECTION( "no_match" )
    {
        auto r = eswitch( pr )
        (
           Case( _1 == 19 && _2 == "H" ) { return true; },
           Default { return false; }
        );

        REQUIRE( !r ); 
    }
}

TEST_CASE( "eswitch_v5::std_tuple", "" ) 
{
    using namespace eswitch_v4;
    using namespace std;

    std::tuple pr{ 7, std::string{ "Hope" }, true };

    SECTION( "match" )
    {
        auto r = eswitch( pr )
        (
            Default { return false; },
            Case( _1 == 7 && _2 == "Hope" && _3 == true ) { return true; }
        );

        REQUIRE( r ); 
    }

    SECTION( "no_match" )
    {
        auto r = eswitch( pr )
        (
            Default { return false; },
            Case( _1 == 7 && _2 == "pe" && _3 == true ) { return true; }
        );

        REQUIRE( !r ); 
    }

    SECTION( "match_without_indexes" )
    {
        auto r = eswitch( pr )
        (
            Default { return false; },
            Case( 7, "Hope", true ) { return true; }
        );

        REQUIRE( r ); 
    }

    SECTION( "match_with_one_index" )
    {
        auto r = eswitch( pr )
        (
            Default { return false; },
            Case( 7, "Hope", _3 == true ) { return true; }
        );

        REQUIRE( r ); 
    }
    
    
    SECTION( "match_with_one_index" )
    {
        auto r = eswitch( pr )
        (
            Default { return false; },
            Case( 7, "Hope", _3 != true ) { return true; }
        );

        REQUIRE( !r ); 
    }

    SECTION( "no_match_without_indexes" )
    {
        auto r = eswitch( pr )
        (
            Default { return false; },
            Case( 7, "Hpe", true ) { return true; }
        );

        REQUIRE( !r ); 
    }

    SECTION( "no_match_without_indexes" )
    {
        auto r = eswitch( pr )
        (
            Default { return false; },
            Case( 7, _2 != "Hpe", true ) { return true; }
        );

        REQUIRE( r ); 
    }
}

TEST_CASE( "eswitch_v5::match_without_indexes", "" ) 
{
    using namespace eswitch_v4;
    using namespace std;

    SECTION( "match_without_indexes" )
    {
        auto r = eswitch( 7, std::string{ "Hope" }, true )
        (
            Default { return false; },
            Case( 7, "Hope", true ) { return true; }
        );

        REQUIRE( r ); 
    }

    
    SECTION( "no_match_without_indexes" )
    {
        auto r = eswitch( 7, std::string{ "Hope" }, true )
        (
            Default { return false; },
            Case( 7, "Hpe", true ) { return true; }
        );

        REQUIRE( !r ); 
    }

    SECTION( "match_with_one_index" )
    {
        auto r = eswitch( 7, std::string{ "Hope" }, true )
        (
            Default { return false; },
            Case( 7, "Hope", _3 == true ) { return true; }
        );

        REQUIRE( r ); 
    }

    SECTION( "no_match_with_one_index" )
    {
        auto r = eswitch( 7, std::string{ "Hope" }, true )
        (
            Default { return false; },
            Case( 7, _2 != "Hpe", true ) { return true; }
        );

        REQUIRE( r ); 
    }
}

TEST_CASE( "eswitch_v5::wildcard", "" ) 
{
    using namespace eswitch_v4;
    using namespace std;

    SECTION( "match_2nd_wildcard" )
    {
        auto r = eswitch( 7, std::string{ "Hope" }, true )
        (
            Default { return false; },
            Case( 7, _, true ) { return true; }
        );

        REQUIRE( r ); 
    }
    SECTION( "match_1st_wildcard" )
    {
        auto r = eswitch( 7, std::string{ "Hope" }, true )
        (
            Default { return false; },
            Case( _, "Hope", true ) { return true; }
        );

        REQUIRE( r ); 
    }
    SECTION( "match_3rd_wildcard" )
    {
        auto r = eswitch( 7, std::string{ "Hope" }, true )
        (
            Default { return false; },
            Case( 7, "Hope", _ ) { return true; }
        );

        REQUIRE( r ); 
    }

    SECTION( "2_match_1st_and_2nd_wildcard" )
    {
        auto r = eswitch( 7, std::string{ "Hope" }, true )
        (
            Default { return false; },
            Case( _, _, true ) { return true; }
        );

        REQUIRE( r ); 
    }

    SECTION( "2_match_1st_and_3rd_wildcard" )
    {
        auto r = eswitch( 7, std::string{ "Hope" }, true )
        (
            Default { return false; },
            Case( _, "Hope", _ ) { return true; }
        );

        REQUIRE( r ); 
    }

    SECTION( "2_match_2nd_and_3rd_wildcard" )
    {
        auto r = eswitch( 7, std::string{ "Hope" }, true )
        (
            Default { return false; },
            Case( 7, _, _ ) { return true; }
        );

        REQUIRE( r ); 
    }


    SECTION( "3_match_1st_2nd_and_3rd_wildcard" )
    {
        auto r = eswitch( 7, std::string{ "Hope" }, true )
        (
            Default { return false; },
            Case( _, _, _ ) { return true; }
        );

        REQUIRE( r ); 
    }
}

TEST_CASE( "eswitch_v5::std_regex", "" ) 
{
    using namespace eswitch_v4;
    using namespace std;

    SECTION( "match_std_regex_without_group" )
    {
        auto r = eswitch( std::string{ "Hope" } )
        (
            Case( R"(\w*)"_r ) { return true; }
        );

        REQUIRE( r );
    }

    
    SECTION( "match_std_regex_with_group" )
    {        
        auto r = eswitch( std::string{ "Hope" } )
        (
            Case( R"((\w*))"_r )( const std::smatch & match ) 
            { 
                return match.size() == 1 && match[1].str() == "Hope"; 
            }
        );

        REQUIRE( r ); 
    }

    SECTION( "match_std_regex_with_group_2" )
    {        
        auto r = eswitch( std::string{ "Key: value" } )
        (
            Case( R"((\w*): (\w*))"_r )( const std::smatch & match ) 
            { 
                return match.size() == 2 && match[1].str() == "Key" && match[2].str() == "value"; 
            }
        );

        REQUIRE( r ); 
    }

    SECTION( "match_std_regex_with_group_3" )
    {        
        auto r = eswitch( std::string{ "Key: 123" } )
        (
            Case( R"((\w*): (\d*))"_r )( const std::smatch & match ) 
            { 
                return match.size() == 2 && match[1].str() == "Key" && match[2].str() == "123"; 
            }
        );

        REQUIRE( r ); 
    }

    SECTION( "match_std_regex_with_group_4" )
    {        
        auto r = eswitch( std::string{ "Key: Hi123" } )
        (
            Case( R"((\w*): (.*))"_r )( const std::smatch & match ) 
            { 
                return match.size() == 2 && match[1].str() == "Key" && match[2].str() == "Hi123"; 
            }
        );

        REQUIRE( r ); 
    }

}

TEST_CASE( "eswitch_v5::not_compiled", "" ) 
{
    using namespace eswitch_v4;
 
    SECTION( "match_2nd_case" )
    {
        // eswitch( std::string{ "Hope" } )
        // (
        //     Case( true ) { return true; }
        // );

        //std::any a = std::string{ "S" };

        // auto r = eswitch( true, std::string{ "Hello" } )
        // (
        //     Case( _2 == true, "Hello" ) { return true; }
        // );

        // eswitch( a )
        // (
        //     Case( _1 == my_type< std::string >{} )( const auto & val ) { return val; }
        // );
        
        // eswitch( a )
        // (
        //     Case( _1 == my_type< std::string >{} )( const std::string & val ) { return val; },
        //     Case( _1 == my_type< int >{} ) { return true; },
        //     Default { return 1; }
        // );

        // eswitch( a )
        // (
        //     Case( _1 == my_type< int >{} && _2 == my_type< int >{} ) { return ""; },
        // );
    }
}