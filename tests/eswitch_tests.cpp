//  Copyright (c) 2019-2021 Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "eswitch_v5.hpp"
#include <memory>
#include <sstream>
#include <iostream>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2/catch.hpp"

enum Place { unknown = 0, new_york=5, washington=129, new_jersey=501 };

TEST_CASE( "eswitch_v5::range", "" ) 
{
    using namespace eswitch_v5;
    using namespace std;

    SECTION( "inside_range" )
    {
        auto r = 
        eswitch( 6 )
        (
            Case( _1.in( 1, 10 ) ) { return true; },
            Default   { return false; } 
        );

        REQUIRE( r );    
    }

    SECTION( "inside_range" )
    {
        auto r = 
        eswitch( 10 )
        (
            Case( _1.in( 1, 10 ) ) { return true; },
            Default   { return false; } 
        );

        REQUIRE( !r );    
    }

    SECTION( "within_range" )
    {
        auto r = 
        eswitch( 6 )
        (
            Case( _1.within( 1, 10 ) ) { return true; },
            Default   { return false; } 
        );

        REQUIRE( r );    
    }

    SECTION( "within_range" )
    {
        auto r = 
        eswitch( 10 )
        (
            Case( _1.within( 1, 10 ) ) { return true; },
            Default   { return false; } 
        );

        REQUIRE( r );    
    }
}

TEST_CASE( "eswitch_v5::any_from_match", "" ) 
{
    using namespace eswitch_v5;
    using namespace std;

    SECTION( "match_any_from" )
    {
        auto r = 
        eswitch( washington )
        (
            Case( _1 == any_from( washington, new_york ) ) { return 123; },
            Default   { return 'c'; } 
        );

        REQUIRE( r == 123 );    
    }

    SECTION( "match_any_from2" )
    {
        auto r = 
        eswitch( new_york )
        (
            Case( _1 == any_from( washington, new_york ) ) { return 123; },
            Default   { return 'c'; } 
        );

        REQUIRE( r == 123 );    
    }
}

// TEST_CASE( "eswitch_v5::compile_time_evaluation", "" ) 
// {
//     using namespace eswitch_v5;
    
//     constexpr auto result0 = eswitch( 50 )
//     (
//         Case( _1 >= 0 && _1 <= 100 ) { return true; },
//         Default { return false; } 
//     );

//     static_assert( result0 );

//     constexpr auto result1 = eswitch( new_york )
//     (
//         Case( _1 == new_york ) { return 123; },
//         Default   { return 'c'; } 
//     );

//     static_assert( result1 == 123 );

//     constexpr auto result2 = eswitch( new_york )
//     (
//         Case( _1 == any_from( washington, new_york ) ) { return 1234; },
//         Default   { return 'c'; } 
//     );

//     static_assert( result2 == 1234 );

//     constexpr auto result3 = eswitch( new_jersey )
//     (
//         Case( _1 == any_from( washington, new_york ) ) { return 1234; },
//         Default   { return 'c'; } 
//     );

//     static_assert( result3 == 'c' );

//     constexpr auto result4 = eswitch( new_jersey )
//     (
//         Case( _1 == any_from( washington, new_york ) ) { return 1234; },
//         Case( new_jersey ) { return 'H'; },
//         Default   { return 'c'; } 
//     );

//     static_assert( result4 == 'H' );

//     constexpr auto result4_1 = eswitch( new_jersey, washington )
//     (
//         Case( _1 == new_jersey && _2 == washington ) { return 1234; },
//         Case( new_jersey ) { return 'H'; },
//         Default   { return 'c'; } 
//     );

//     static_assert( result4_1 == 1234 );

//     constexpr auto result5 = eswitch( new_jersey )
//     (
//         Case( _1 != any_from( washington, new_york ) ) { return 1234; },
//         Case( new_jersey ) { return 'H'; },
//         Default   { return 'c'; } 
//     );

//     static_assert( result5 == 1234 );

//     auto is_even = []( auto v ) { return v % 2 == 0; };
//     auto is_odd  = []( auto v ) { return !(v % 2 == 0); };

//     constexpr auto result6 = eswitch( 2 )
//     (
//         Case( ( is_even, _1 ) ) { return true; },
//         Case( ( is_odd, _1 ) ) { return false; } 
//     );

//     static_assert( result6 == true );

//     constexpr auto result7 = eswitch( 3 )
//     (
//         Case( ( is_even, _1 ) ) { return true; },
//         Case( ( is_odd, _1 ) ) { return false; } 
//     );

//     static_assert( result7 == false );

//     constexpr std::variant< int, float > var1 = 10;

//     constexpr auto result8 = eswitch( var1 )
//     (
//         Case( _1 == is< float >{} ) { return false; },
//         Case( _1 == is< int >{} ) { return true; }
//     );    

//     static_assert( result8 == true );

//     constexpr std::variant< int, double > var2{ 5.5 };

//     constexpr auto result9 = eswitch( var2 )
//     (
//         Case( _1 == is< double >{} ) { return false; },
//         Case( _1 == is< int >{} ) { return true; }
//     );    

//     static_assert( result9 == false );

//     constexpr auto result9_1 = eswitch( var2 )
//     (
//         Case( _1 == is< double >{} || _1 == is< int >{} ) { return true; },
//         Default { return false; }
//     );    

//     static_assert( result9_1 == true );

//     constexpr auto result9_2 = eswitch( var2 )
//     (
//         Case( _1 == is< double >{} )( const double val ) { return val; },
//         Case( _1 == is< int >{} )( const int val ) { return val; },
//         Default { return 0; }
//     );    

//     static_assert( result9_2 == 5.5 );

//     constexpr std::variant< int, float, const char* > var3 = "H";

//     constexpr auto result10 = eswitch( var3 )
//     (
//         Case( _1 == is< float >{} ) { return true; },
//         Case( _1 == is< int >{} ) { return true; },
//         Default { return false; }
//     );    

//     static_assert( result10 == false );

//     constexpr std::pair pr{ 10, true };

//     constexpr auto result11 = eswitch( pr )
//     (
//         Case( _1 == 10 && _2 == true ) { return true; },
//         Default { return false; }
//     );

//     static_assert( result11 == true );

//     constexpr auto result11_1 = eswitch( pr )
//     (
//         Case( 10, true ) { return true; },
//         Default { return false; }
//     );

//     static_assert( result11_1 == true );

//     constexpr std::tuple tp{ 10, 5.6, true };

//     constexpr auto result12 = eswitch( tp )
//     (
//         Case( _1 == 10 && _2 == 5.6 && _3 == true ) { return true; },
//         Default { return false; }
//     );

//     static_assert( result12 == true );

//     constexpr auto result12_1 = eswitch( tp )
//     (
//         Case( 10, 5.6, true ) { return true; },
//         Default { return false; }
//     );

//     static_assert( result12_1 == true );

//     constexpr auto result12_2 = eswitch( tp )
//     (
//         Case( 10, 5.6, false ) { return true; },
//         Default { return false; }
//     );

//     static_assert( result12_2 == false );

//     constexpr auto result12_3 = eswitch( tp )
//     (
//         Case( _1 == 10 && _2 == 5.6 && _3 != false ) { return true; },
//         Default { return false; }
//     );

//     static_assert( result12_3 == true );

//     constexpr auto result12_4 = eswitch( tp )
//     (
//         Case( 10, 5.6, _3 != false ) { return true; },
//         Default { return false; }
//     );

//     static_assert( result12_4 == true );

// }


TEST_CASE( "eswitch_v5::greater_or_less", "" ) 
{
    using namespace eswitch_v5;
    using namespace std;

    SECTION( "greater" )
    {
        auto r = 
        eswitch( 50 )
        (
            Case( _1 > 2 ) { return true; },
            Default { return false; } 
        );

        REQUIRE( r );    
    }

    SECTION( "greater_2nd_match" )
    {
        auto r = 
        eswitch( 50 )
        (
            Case( _1 > 50 ) { return false; },
            Case( _1 > 49 ) { return true; },
            Default { return false; } 
        );

        REQUIRE( r );    
    }

    
    SECTION( "greater_or_equal" )
    {
        auto r = 
        eswitch( 50 )
        (
            Case( _1 >= 50 ) { return true; },
            Default { return false; } 
        );

        REQUIRE( r );    
    }

    SECTION( "less" )
    {
        auto r = 
        eswitch( 50 )
        (
            Case( _1 < 100 ) { return true; },
            Default { return false; } 
        );

        REQUIRE( r );    
    }

    
    SECTION( "less_or_equal" )
    {
        auto r = 
        eswitch( 50 )
        (
            Case( _1 <= 50 ) { return true; },
            Default { return false; } 
        );

        REQUIRE( r );    
    }

    SECTION( "mix_of_operators" )
    {
        auto r = 
        eswitch( 50 )
        (
            Case( _1 >= 0 && _1 <= 100 ) { return true; },
            Default { return false; } 
        );

        REQUIRE( r );    
    }
}

TEST_CASE( "eswitch_v5::equal_match", "" ) 
{
    using namespace eswitch_v5;
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
    using namespace eswitch_v5;
    using namespace std;

    SECTION( "match_case" )
    {
        int i = 0;
        eswitch( washington )
        (
            Case( washington ) { i += 1; } ^ fallthrough_,
            Case( new_york )   { i += 2; },
            Default            { i += 3; }
        );


        REQUIRE( i == 3 );    
    }

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
    using namespace eswitch_v5;
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
    using namespace eswitch_v5;
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
    using namespace eswitch_v5;
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
    using namespace eswitch_v5;
    using namespace std;

    std::any a = 10;

    REQUIRE_THROWS_AS( eswitch( a )
    (
        Case( _1 == is< float >{} ) { return true; }
    ), std::bad_optional_access );    
}

TEST_CASE( "eswitch_v5::std_any_default", "" ) 
{
    using namespace eswitch_v5;
    using namespace std;

    std::any a = 10;

    REQUIRE_FALSE( 
        eswitch( a )
        (
            Case( is< float >{} ) { return true; },
            Default { return false; }
        ) 
    );    
}

TEST_CASE( "eswitch_v5::std_any_success", "" ) 
{
    using namespace eswitch_v5;
    using namespace std;

    SECTION( "match_2nd_case" )
    {
        std::any a = 10;

        REQUIRE( 
            eswitch( a )
            (
                Case( _1 == is< float >{} ) { return false; },
                Case( _1 == is< int >{} ) { return true; },
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
                Case( _1 == is< float >{} ) { return true; },
                Case( _1 == is< int >{} ) { return false; },
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
                Case( _1 == is< float >{} || _1 == is< int >{} ) { return true; },
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
                Case( _1 == is< float >{} ) { return false; },
                Case( _1 == is< std::string >{} ) { return true; },
                Default { return false; }
            ) 
        );    
    }

}

TEST_CASE( "eswitch_v5::std_any_only", "" ) 
{
    using namespace eswitch_v5;
    using namespace std;

    SECTION( "match_2nd_case" )
    {
        std::any a = 10;

        auto r = eswitch( a )
        (
            Case( _1 == is< float >{} )  { return 0; },
            Case( _1 == is< int >{} )( const int val ) { return val + 11; },
            Default { return 0; }
        );

        REQUIRE( r == 21 ); 
    }

    SECTION( "match_3rd_case" )
    {
        std::any a = std::string{"STR"};

        auto r = eswitch( a )
        (
            Case( _1 == is< float >{} ) { return ""; },
            Case( _1 == is< int >{} )( const int val ) { return ""; },
            Case( _1 == is< std::string >{} )( const std::string & val ) { return val + "hello"; },
            Default { return ""; }
        );

        REQUIRE( r == "STRhello" ); 
    }
}

TEST_CASE( "eswitch_v5::std_variant_only", "" ) 
{
    using namespace eswitch_v5;
    using namespace std;

    SECTION( "match_2nd_case" )
    {
        std::variant< int, double > a = 19.9;

        auto r = eswitch( a )
        (
            Case( _1 == is< double >{} )( const double val ) { return val - 1; },
            Case( _1 == is< int >{} )  { return 0; },
            Default { return 0; }
        );

        REQUIRE( r == 18.9 ); 
    }

    SECTION( "match_3rd_case" )
    {
        std::variant< int, float, std::string > a = std::string{"STR"};

        auto r = eswitch( a )
        (
            Case( _1 == is< float >{} ) { return ""; },
            Case( _1 == is< int >{} )( const int val ) { return ""; },
            Case( _1 == is< std::string >{} )( const std::string & val ) { return val + "hello"; },
            Default { return ""; }
        );

        REQUIRE( r == "STRhello" ); 
    }

    SECTION( "match_Default_case" )
    {
        std::variant< int, double, std::string > a = 16.7;

        auto r = eswitch( a )
        (
            Case( _1 == is< int >{} )( const int val ) { return ""; },
            Case( _1 == is< std::string >{} )( const std::string & val ) { return val + "hello"; },
            Default { return "Default"; }
        );

        REQUIRE( r == "Default" ); 
    }

    SECTION( "unset_variant" )
    {
        std::variant< std::monostate, int, double, std::string > a;

        auto r = eswitch( a )
        (
            Case( _1 == is< int >{} )( const int val ) { return ""; },
            Case( _1 == is< std::string >{} )( const std::string & val ) { return val + "hello"; },
            Default { return "Default"; }
        );

        REQUIRE( r == "Default" ); 
    }
}

TEST_CASE( "eswitch_v5::std_variant_plus_std_any", "" ) 
{
    using namespace eswitch_v5;
    using namespace std;

    SECTION( "match_2nd_case" )
    {
        std::any a = std::string{ "S" };
        std::variant< int, double > v = 19.9;

        auto r = eswitch( a, v )
        (
            Case( _1 == is< std::string >{} && _2 == is< double >{} ) { return true; },
            Case( _1 == is< int >{} && _2 == is< int >{} ) { return false; },
            Default { return false; }
        );

        REQUIRE( r ); 
    }

    SECTION( "match_2nd_case" )
    {
        std::any a = std::string{ "S" };
        
        auto r = eswitch( a )
        (
            Case( _1 == is< std::string >{} )( const std::string & val ) { return val; },
            Case( _1 == is< int >{} ) { return ""; },
            Default { return ""; }
        );
        
        REQUIRE( r == "S" ); 
    }
}

TEST_CASE( "eswitch_v5::default_cast_at_any_position", "" ) 
{
    using namespace eswitch_v5;
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
    using namespace eswitch_v5;
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
    using namespace eswitch_v5;
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
    using namespace eswitch_v5;
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
    using namespace eswitch_v5;
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
    using namespace eswitch_v5;
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
            Case( R"((\w*))"_r )( const std::vector< std::string > & match ) 
            { 
                return match.size() == 2 && match[1] == "Hope"; 
            }
        );

        REQUIRE( r ); 
    }

    SECTION( "match_std_regex_with_group_and_std_move" )
    {        
        auto r = eswitch( std::string{ "Hope" } )
        (
            Case( R"((\w*))"_r )( std::vector< std::string > && match ) 
            { 
                std::string tmp{ std::move( match[1] ) };

                return match.size() == 2 && tmp == "Hope" && match[1].empty(); 
            }
        );

        REQUIRE( r ); 
    }


    SECTION( "match_std_regex_with_group_2" )
    {        
        auto r = eswitch( std::string{ "Key: value" } )
        (
            Case( R"((\w*): (\w*))"_r )( const std::vector< std::string > & match ) 
            { 
                //std::cout << "SSSSIZE=" << match.size() << match[1].str() << match[2].str() << std::endl << std::endl;
                return match.size() == 3 && match[1] == "Key" && match[2] == "value"; 
            }
        );

        REQUIRE( r ); 
    }

    SECTION( "match_std_regex_with_group_3" )
    {        
        auto r = eswitch( std::string{ "Key: 123" } )
        (
            Case( R"((\w*): (\d*))"_r )( const std::vector< std::string > & match ) 
            { 
                return match.size() == 3 && match[1] == "Key" && match[2] == "123"; 
            }
        );

        REQUIRE( r ); 
    }

    SECTION( "match_std_regex_with_group_4" )
    {        
        auto r = eswitch( std::string{ "Key: Hi123" } )
        (
            Case( R"((\w*): (.*))"_r )( const std::vector< std::string > & match ) 
            { 
                return match.size() == 3 && match[1] == "Key" && match[2] == "Hi123"; 
            }
        );

        REQUIRE( r ); 
    }

}

std::string foo()
{
    return "Hello";
}

TEST_CASE( "eswitch_v5::temporary_as_argument", "" )
{
    using namespace eswitch_v5;
 
    const Place p = new_york;

    bool executed = false;

    eswitch( p, foo() )
    ( 
        Case( _1 == new_york || _2 == "Hello" ) { executed = true; },
        Case( _1 == new_york && _2 != "Hello" ) { assert( false ); }
    );

    REQUIRE( executed );
}

TEST_CASE( "eswitch_v5::predicate_with_mutiple_args", "" )
{
    using namespace eswitch_v5;
 
    auto is_positive = []( const int i, const int j, const int k ) { return i  > 0 && j > 0 && k > 0; };

    SECTION( "in_order" )
    {
        bool executed = false;

        eswitch( 1, 2, 3 )
        (
            Case( ( is_positive, _1, _2, _3 ) ) { executed = true; },
            Default { assert( false ); }
        );

        REQUIRE( executed );
    }

    SECTION( "out_of_order" )
    {
        bool executed = false;

        eswitch( 1, 2, 3 )
        (
            Case( ( is_positive, _3, _1, _2 ) ) { executed = true; },
            Default { assert( false ); }
        );

        REQUIRE( executed );
    }
}

TEST_CASE( "eswitch_v5::details", "" )
{
    using namespace eswitch_v5;
 
    std::any a{ 10 };
    REQUIRE( details::is_std_any_v< decltype( a ) > );

    const std::any & aref = a;
    REQUIRE( details::is_std_any_v< decltype( aref ) > );

    std::variant< int, std::string > v{ 10 };
    REQUIRE( details::is_std_variant_v< decltype( v ) > );

    const std::variant< int, std::string > & vref = v;
    REQUIRE( details::is_std_variant_v< decltype( vref ) > );
}

struct base
{
    virtual ~base(){}
    virtual int draw() = 0;
    virtual int cdraw() const = 0;
};

struct circle : base
{
    virtual int draw() { return 10; };
    virtual int cdraw() const { return 11; }
};

struct square : base
{
    virtual int draw() { return 20; };
    virtual int cdraw() const { return 21; }
};

struct rectangle : base
{
    virtual int draw() { return 30; };
    virtual int cdraw() const { return 31; }
};

TEST_CASE( "eswitch_v5::polymorphism_pointer", "" )
{
    using namespace eswitch_v5;

    SECTION( "circle_match" ) 
    {
        circle s;
        base * b = &s;

        auto res = eswitch( b )
        (
            Case( is< circle >{} )( circle * c ){ return c->draw(); },
            Case( is< square >{} )( square * s ){ return s->draw(); },
            Default { return -1; }
        );

        REQUIRE( res == 10 );
    }

    SECTION( "square_match" ) 
    {
        square s;
        base * b = &s;

        auto res = eswitch( b )
        (
            Case( is< circle >{} )( circle * c ){ return c->draw(); },
            Case( is< square >{} )( square * s ){ return s->draw(); },
            Default { return -1; }
        );

        REQUIRE( res == 20 );
    }


    SECTION( "Default_match" ) 
    {
        base * b = nullptr;

        auto res = eswitch( b )
        (
            Case( is< circle >{} )( circle * c ){ return c->draw(); },
            Case( is< square >{} )( square * s ){ return s->draw(); },
            Default { return -1; }
        );

        REQUIRE( res == -1 );
    }
}

TEST_CASE( "eswitch_v5::polymorphism_const_pointer", "" )
{
    using namespace eswitch_v5;

    SECTION( "circle_match" ) 
    {
        circle s;
        const base * b = &s;

        auto res = eswitch( b )
        (
            Case( is< circle >{} )( const circle * c ){ return c->cdraw(); },
            Case( is< square >{} )( const square * s ){ return s->cdraw(); },
            Default { return -1; }
        );

        REQUIRE( res == 11 );
    }

    SECTION( "square_match" ) 
    {
        square s;
        const base * b = &s;

        auto res = eswitch( b )
        (
            Case( is< circle >{} )( const circle * c ){ return c->cdraw(); },
            Case( is< square >{} )( const square * s ){ return s->cdraw(); },
            Default { return -1; }
        );

        REQUIRE( res == 21 );
    }


    SECTION( "Default_match" ) 
    {
        const base * b = nullptr;

        auto res = eswitch( b )
        (
            Case( is< circle >{} )( const circle * c ){ return c->cdraw(); },
            Case( is< square >{} )( const square * s ){ return s->cdraw(); },
            Default { return -1; }
        );

        REQUIRE( res == -1 );
    }
}

TEST_CASE( "eswitch_v5::polymorphism_reference", "" )
{
    using namespace eswitch_v5;

    SECTION( "circle_match" ) 
    {
        circle c;
        base & b = c;

        auto res = eswitch( b )
        (
            Case( is< circle >{} )( circle & c ){ return c.draw(); },
            Case( is< square >{} )( square & s ){ return s.draw(); },
            Default { return -1; }
        );

        REQUIRE( res == 10 );
    }

    SECTION( "square_match" ) 
    {
        square s;
        base & b = s;

        auto res = eswitch( b )
        (
            Case( is< circle >{} )( circle & c ){ return c.draw(); },
            Case( is< square >{} )( square & s ){ return s.draw(); },
            Default { return -1; }
        );

        REQUIRE( res == 20 );
    }


    SECTION( "Default_match" ) 
    {
        rectangle r;
        base & b = r;

        auto res = eswitch( b )
        (
            Case( is< circle >{} )( circle & c ){ return c.draw(); },
            Case( is< square >{} )( square & s ){ return s.draw(); },
            Default { return -1; }
        );

        REQUIRE( res == -1 );
    }
}

TEST_CASE( "eswitch_v5::polymorphism_const reference", "" )
{
    using namespace eswitch_v5;

    SECTION( "circle_match" ) 
    {
        circle c;
        const base & b = c;

        auto res = eswitch( b )
        (
            Case( is< circle >{} )( const circle & c ){ return c.cdraw(); },
            Case( is< square >{} )( const square & s ){ return s.cdraw(); },
            Default { return -1; }
        );

        REQUIRE( res == 11 );
    }

    SECTION( "square_match" ) 
    {
        square s;
        const base & b = s;

        auto res = eswitch( b )
        (
            Case( is< circle >{} )( const circle & c ){ return c.cdraw(); },
            Case( is< square >{} )( const square & s ){ return s.cdraw(); },
            Default { return -1; }
        );

        REQUIRE( res == 21 );
    }


    SECTION( "Default_match" ) 
    {
        rectangle r;
        const base & b = r;

        auto res = eswitch( b )
        (
            Case( is< circle >{} )( const circle & c ){ return c.cdraw(); },
            Case( is< square >{} )( const square & s ){ return s.cdraw(); },
            Default { return -1; }
        );

        REQUIRE( res == -1 );
    }
}

// TEST_CASE( "eswitch_v5::not_compiled", "" ) 
// {
//     using namespace eswitch_v5;
 
//     SECTION( "match_2nd_case" )
//     {
//         // eswitch( std::string{ "Hope" } )
//         // (
//         //     Case( true ) { return true; }
//         // );

//         //std::any a = std::string{ "S" };

//         // auto r = eswitch( true, std::string{ "Hello" } )
//         // (
//         //     Case( _2 == true, "Hello" ) { return true; }
//         // );

//         // eswitch( a )
//         // (
//         //     Case( _1 == is< std::string >{} )( const auto & val ) { return val; }
//         // );
        
//         // eswitch( a )
//         // (
//         //     Case( _1 == is< std::string >{} )( const std::string & val ) { return val; },
//         //     Case( _1 == is< int >{} ) { return true; },
//         //     Default { return 1; }
//         // );

//         // eswitch( a )
//         // (
//         //     Case( _1 == is< int >{} && _2 == is< int >{} ) { return ""; },
//         // );
//     }
// }