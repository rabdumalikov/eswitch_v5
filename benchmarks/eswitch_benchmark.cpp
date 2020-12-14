//  Copyright (c) 2019 Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#undef NDEBUG
#include <assert.h>

#include "benchmark/benchmark.h"
#include "eswitch_v5.hpp"

#define BENCHMARK_IT( name, function_to_benchmark, ... )\
void name(benchmark::State& state)\
{\
\
    while (state.KeepRunning()) {\
        auto res = function_to_benchmark(__VA_ARGS__);\
        benchmark::DoNotOptimize(res);\
    }\
}\
BENCHMARK(name)


#define BENCHMARK_AND_COMPARE( name, function_to_benchmark, actual_result, ... )\
void name(benchmark::State& state)\
{\
    while (state.KeepRunning()) {\
        auto var = function_to_benchmark(__VA_ARGS__);\
        benchmark::DoNotOptimize(var == actual_result);\
        benchmark::ClobberMemory();\
    }\
}\
BENCHMARK(name)


enum estatus3 {
    SUCCESS,
    FAILED
};

enum eprogress3 {
    RUNNING,
    IN_PROGRES,
    FINISHED
};

int __attribute__ ((noinline)) Native_Switch( const estatus3 es3, const eprogress3 ep3, const std::string& str )
{
    switch (es3) {
    case SUCCESS:

        switch (ep3) {
        case RUNNING: break;
        case IN_PROGRES: break;
        case FINISHED: break;
        default: return 31;
        }
        return 0;
    case FAILED:
        switch (ep3) {
        case RUNNING:
            if (str == "Hello") return 1;
            else 
                return 2;
        case IN_PROGRES: break;
        case FINISHED: break;
        default: return 32;
        }

        break;
    }

    return 809;
}

int __attribute__ ((noinline)) _eswitch( const estatus3 es3, const eprogress3 ep3, const std::string& str )
{
    using namespace eswitch_v5;

    return eswitch(es3, ep3, str)
    ( 
        Case( _1 == SUCCESS) { return 0; },
        Case( _1 == FAILED && _2 == RUNNING && _3 == "Hello") { return 1; },
        Case( _1 == FAILED && _2 == RUNNING) { return 2; }, 
        Default { return 33; }
    );
}

BENCHMARK_AND_COMPARE( check_multiple_matches_E_SWITCH, _eswitch, 1, FAILED, RUNNING, std::string{"Hello"} );
BENCHMARK_AND_COMPARE( check_multiple_matches_NativeSwitch, Native_Switch, 1, FAILED, RUNNING, std::string{"Hello"} );


// // =====================================================

int __attribute__ ((noinline)) native_switch_forloop( int amount_iters )
{
    int result = 0;
    for (int i = 0; i < amount_iters; ++i) {
        switch (i) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            result += 1;
            break;
        default:
            result += 3;
            break;
        }
    }

    return result;
}

int __attribute__ ((noinline)) E_SWITCH_forloop( int amount_iters )
{
    using namespace eswitch_v5;

    int result = 0;

    for (int i = 0; i < amount_iters; ++i) 
    {
        result += eswitch(i)
        (
            //Case( _1 == 1 || _1 == 2 || _1 == 3 || _1 == 4 || _1 == 5 ) { return 1; }, // also possible
            Case( _1 == any_from( 1, 2, 3, 4, 5 ) ) { return 1; }, 
            Default { return 3; }
        );
    }

    return result;
}

int i = 10;
BENCHMARK_AND_COMPARE( check_forloop_ESWITCH, E_SWITCH_forloop, 999, i );
BENCHMARK_AND_COMPARE( check_forloop_NativeSwitch, native_switch_forloop, 999, i );

// // =====================================================

// int IF_range_check( const int value )
// {
//     if( value > 0 && value < 10 ) return 0;
//     else if( value >= 10 && value <= 20 ) return 11;
//     else if( value > 30 && value <= 50 ) return 40;
//     else return 101;
// }

// int E_SWITCH_range_check( const int value )
// {
//     using namespace eswitch_v5;

//     return eswitch( value )
//     (
//         Case( _1 > 0 && _1 < 10 ) { return 0; },
//         Case( _1 >= 10 && _1 <= 20 ) { return 11; },
//         Case( _1 > 30 && _1 <= 50 ) { return 40; },
//         Default { return 101; }
//     );
// }

// int E_SWITCH_range_check_made_easy( const int value )
// {
//     using namespace eswitch_v5;

//     return eswitch( value )
//     (
//         Case( _1.in( 1, 10 ) ) { return 0; },
//         Case( _1.within( 10, 20 ) ) { return 11; },
//         Case( _1 > 30 && _1 <= 50 ) { return 40; }
//     );
// }

// BENCHMARK_AND_COMPARE( check_range_check_1st_match_ESWITCH, E_SWITCH_range_check, 0, 5 );
// BENCHMARK_AND_COMPARE( check_range_check_1st_match_ESWITCH_2, E_SWITCH_range_check_made_easy, 0, 5 );
// BENCHMARK_AND_COMPARE( check_range_check_1st_match_IF, IF_range_check, 0, 5 );

// BENCHMARK_AND_COMPARE( check_range_check_2nd_match_ESWITCH, E_SWITCH_range_check, 11, 10 );
// BENCHMARK_AND_COMPARE( check_range_check_2nd_match_ESWITCH_2, E_SWITCH_range_check_made_easy, 11, 10 );
// BENCHMARK_AND_COMPARE( check_range_check_2nd_match_IF, IF_range_check, 11, 10 );

// BENCHMARK_AND_COMPARE( check_range_check_3rd_match_ESWITCH, E_SWITCH_range_check, 40, 31 );
// BENCHMARK_AND_COMPARE( check_range_check_3rd_match_ESWITCH_2, E_SWITCH_range_check_made_easy, 40, 31 );
// BENCHMARK_AND_COMPARE( check_range_check_3rd_match_IF, IF_range_check, 40, 31 );

// static auto is_positive = []( const int i ) { return i  > 0; };

// bool direct_match_via_predicate( const int value )
// {
//     return is_positive( value );
// }

// bool E_SWITCH_match_via_predicate( const int value )
// {
//     using namespace eswitch_v5;

//     return eswitch( value )
//     (
//         Case( ( is_positive, _1 ) ) { return true; },
//         Default { return false; }
//     );
// }

// BENCHMARK_AND_COMPARE( check_match_via_predicate_ESWITCH, E_SWITCH_match_via_predicate, true, 5 );
// BENCHMARK_AND_COMPARE( check_match_via_predicate_DIRECT, direct_match_via_predicate, true, 5 );
// BENCHMARK_AND_COMPARE( check_no_match_via_predicate_ESWITCH, E_SWITCH_match_via_predicate, false, -1 );
// BENCHMARK_AND_COMPARE( check_no_match_via_predicate_DIRECT, direct_match_via_predicate, false, -1 );


// int IF_std_any( const std::any & value )
// {
//     if( auto * v = std::any_cast< int >( &value ) ) return *v;
//     else if( auto * v = std::any_cast< std::string >( &value ) ) return v->size();
//     else
//     {
//         return -1;
//     }   
// }

// int E_SWITCH_std_any( const std::any & value )
// {
//     using namespace eswitch_v5;

//     return eswitch( value )
//     (
//         Case( _1 == is< int >{} )( const int v ) { return v; },
//         Case( _1 == is< std::string >{} )( const std::string & v ) { return v.size(); },
//         Default { return -1; }
//     );
// }

// BENCHMARK_AND_COMPARE( check_std_any_1st_match_ESWITCH, E_SWITCH_std_any, 10, std::any{ 10 } );
// BENCHMARK_AND_COMPARE( check_std_any_1st_match_IF, IF_std_any, 10, std::any{ 10 } );
// BENCHMARK_AND_COMPARE( check_std_any_2nd_match_ESWITCH, E_SWITCH_std_any, 4, std::any{ std::string{ "Hell" } } );
// BENCHMARK_AND_COMPARE( check_std_any_2nd_match_IF, IF_std_any, 4, std::any{ std::string{ "Hell" } } );
// BENCHMARK_AND_COMPARE( check_std_any_3rd_match_ESWITCH, E_SWITCH_std_any, -1, std::any{ double{ 0.0 } } );
// BENCHMARK_AND_COMPARE( check_std_any_3rd_match_IF, IF_std_any, -1, std::any{ double{ 0.0 } } );

// int IF_std_variant( const std::variant< int, std::string > & value )
// {
//     if( auto * v = std::get_if< int >( &value ) ) return *v;
//     else if( auto * v = std::get_if< std::string >( &value ) ) return v->size();
//     else
//     {
//         return -1;
//     }   
// }

// int E_SWITCH_std_variant( const std::variant< int, std::string > & value )
// {
//     using namespace eswitch_v5;

//     return eswitch( value )
//     (
//         Case( is< int >{} )( const int v ) { return v; },
//         Case( is< std::string >{} )( const std::string & v ) { return v.size(); },
//         Default { return -1; }
//     );
// }

// BENCHMARK_AND_COMPARE( check_std_variant_1st_match_ESWITCH, E_SWITCH_std_variant, 10, std::variant< int, std::string >{ 10 } );
// BENCHMARK_AND_COMPARE( check_std_variant_1st_match_IF, IF_std_variant, 10, std::variant< int, std::string >{ 10 } );
// BENCHMARK_AND_COMPARE( check_std_variant_2nd_match_ESWITCH, E_SWITCH_std_variant, 5, std::variant< int, std::string >{ std::string{"Hello"} } );
// BENCHMARK_AND_COMPARE( check_std_variant_2nd_match_IF, IF_std_variant, 5, std::variant< int, std::string >{ std::string{"Hello"} } );

// static auto rgx_for_match = std::regex( ".*: .*" );

// auto IF_std_regex_match( const std::string & text )
// {
//     if( std::smatch match; std::regex_match( text, match, rgx_for_match ) )
//     {
//         return true;
//     }
//     return false;
// }

// int E_SWITCH_std_regex_match( const std::string & value )
// {
//     using namespace eswitch_v5;

//     return eswitch( value )
//     (
//         Case( rgx_for_match ) { return true; },
//         Default { return false; }
//     );
// }

// BENCHMARK_AND_COMPARE( check_std_regex_match_ESWITCH, E_SWITCH_std_regex_match,true, std::string{ "Key: value" } );
// BENCHMARK_AND_COMPARE( check_std_regex_match_IF, IF_std_regex_match, true, std::string{ "Key: value" } );

// static auto rgx_for_match_with_withdrawal = std::regex( "(.*): (.*)" );

// int IF_std_regex_match_with_withdrawal( const std::string & text )
// {
//     if( std::smatch match; std::regex_match( text, match, rgx_for_match_with_withdrawal ) )
//     {
//         std::vector< std::string > vs;
//         vs.reserve( match.size() );

//         for( const auto & v : match )  
//             vs.push_back( v );

//         return vs.size();
//     }
//     return 0;
// }

// int E_SWITCH_std_regex_match_with_withdrawal( const std::string & value )
// {
//     using namespace eswitch_v5;

//     auto res = eswitch( value )
//     (
//         Case( rgx_for_match_with_withdrawal )( std::vector< std::string > && vs ) { return std::move( vs ); },
//         Default { return std::vector< std::string >{}; }
//     );

//     return res.size();
// }

// BENCHMARK_AND_COMPARE( check_std_regex_match_with_withdrawal_ESWITCH, E_SWITCH_std_regex_match_with_withdrawal, 3, std::string{ "Key: value" } );
// BENCHMARK_AND_COMPARE( check_std_regex_match_with_withdrawal_IF, IF_std_regex_match_with_withdrawal, 3, std::string{ "Key: value" } );
// BENCHMARK_AND_COMPARE( check_std_regex_no_match_with_withdrawal_ESWITCH, E_SWITCH_std_regex_match_with_withdrawal, 0, std::string{ "HTTP/1.1 200 OK" } );
// BENCHMARK_AND_COMPARE( check_std_regex_no_match_with_withdrawal_IF, IF_std_regex_match_with_withdrawal, 0, std::string{ "HTTP/1.1 200 OK" } );


// // =====================================================

// int native_switch_str_cmp(const std::string& str)
// {
//     switch (5) {
//     case 5:
//         if (str == "Hello") return 1;
//         break;
//     default:
//         return 3;
//     }
// }

// int E_SWITCH_str_cmp(const std::string& str)
// {
//     using namespace eswitch_v5;

//     return eswitch(5, str)
//     (
//         Case( _1 == 5 && _2 == "Hello") { return 1; },
//         Default { return 3; }
//     );
// }

// BENCHMARK_AND_COMPARE( check_str_cmp_ESWITCH, E_SWITCH_str_cmp, 1, std::string("Hello") );
// BENCHMARK_AND_COMPARE( check_str_cmp_NativeSwitch, native_switch_str_cmp, 1, std::string("Hello") );

// // =====================================================

// int native_switch_several_str_cmp(const std::string& str)
// {
//     switch (5) {
//     case 5:
//         if (str == "is") return -10;
//         else if (str == "my") return -20;
//         else if (str == "name") return -30;
//         else if (str == "Hell") return -40;
//         else if (str == "Hello") return 1;        
//     default:
//         return 3;
//     }
// }

// int E_SWITCH_several_str_cmp(const std::string& str)
// {
//     using namespace eswitch_v5;

//     return eswitch(5, str)
//     (
//         Case( _1 == 5 && _2 == "is") { return -10; },
//         Case( _1 == 5 && _2 == "my") { return -20; },     
//         Case( _1 == 5 && _2 == "name" ) { return -30; },
//         Case( _1 == 5 && _2 == "Hell") { return -40; },
//         Case( _1 == 5 && _2 == "Hello") { return 1; },
//         Default { return 3; }
//     );
// }

// BENCHMARK_AND_COMPARE( check_several_str_cmp_ESWITCH, E_SWITCH_several_str_cmp, 1, std::string("Hello") );
// BENCHMARK_AND_COMPARE( check_several_str_cmp_NativeSwitch, native_switch_several_str_cmp, 1, std::string("Hello") );

// // =====================================================

// enum Place { unknown = 0, new_york=5, washington=129, new_jersey=501 };

// int IF_from_string_to_enum(const std::string& str)
// {
//     if( str == "unknown" ) return unknown;
//     else if( str == "new_york" ) return new_york;
//     else if( str == "washington" ) return washington;
//     else if( str == "new_jersey" ) return new_jersey;
//     else
//     {
//         return -1;
//     }    
// }

// int E_SWITCH_from_string_to_enum(const std::string& str)
// {
//     using namespace eswitch_v5;
    
//     return eswitch(str)
//     (
//         Case( "unknown" )     { return unknown; },
//         Case( "new_york" )    { return new_york; }, 
//         Case( "washington" )  { return washington; },
//         Case( "new_jersey" )  { return new_jersey; }, 
//         Default               { return -1; }
//     );
// }

// int MAP_from_string_to_enum(const std::string& str)
// {
//     static std::map< std::string, Place > cache{
//         { "unknown", unknown },
//         { "new_york", new_york },
//         { "washington", washington },
//         { "new_jersey", new_jersey },
//     };

//     auto iter = cache.find( str );
//     if( iter != std::end( cache ) ) return iter->second;

//     return -1;
// }


// BENCHMARK_AND_COMPARE( check_from_string_to_enum_1st_match_ESWITCH, E_SWITCH_from_string_to_enum, unknown, std::string("unknown") );
// BENCHMARK_AND_COMPARE( check_from_string_to_enum_1st_match_IF, IF_from_string_to_enum, unknown, std::string("unknown") );
// BENCHMARK_AND_COMPARE( check_from_string_to_enum_1st_match_MAP, MAP_from_string_to_enum, unknown, std::string("unknown") );

// // =====================================================

// BENCHMARK_AND_COMPARE( check_from_string_to_enum_2nd_match_ESWITCH, E_SWITCH_from_string_to_enum, new_york, std::string("new_york") );
// BENCHMARK_AND_COMPARE( check_from_string_to_enum_2nd_match_IF, IF_from_string_to_enum, new_york, std::string("new_york") );
// BENCHMARK_AND_COMPARE( check_from_string_to_enum_2nd_match_MAP, MAP_from_string_to_enum, new_york, std::string("new_york") );

// // =====================================================

// BENCHMARK_AND_COMPARE( check_from_string_to_enum_3rd_match_ESWITCH, E_SWITCH_from_string_to_enum, washington, std::string("washington") );
// BENCHMARK_AND_COMPARE( check_from_string_to_enum_3rd_match_IF, IF_from_string_to_enum, washington, std::string("washington") );
// BENCHMARK_AND_COMPARE( check_from_string_to_enum_3rd_match_MAP, MAP_from_string_to_enum, washington, std::string("washington") );

// // =====================================================

// BENCHMARK_AND_COMPARE( check_from_string_to_enum_4th_match_ESWITCH, E_SWITCH_from_string_to_enum, new_jersey, std::string("new_jersey") );
// BENCHMARK_AND_COMPARE( check_from_string_to_enum_4th_match_IF, IF_from_string_to_enum, new_jersey, std::string("new_jersey") );
// BENCHMARK_AND_COMPARE( check_from_string_to_enum_4th_match_MAP, MAP_from_string_to_enum, new_jersey, std::string("new_jersey") );

// // =====================================================

// BENCHMARK_AND_COMPARE( check_from_string_to_enum_no_match_ESWITCH, E_SWITCH_from_string_to_enum, -1, std::string("junk") );
// BENCHMARK_AND_COMPARE( check_from_string_to_enum_no_match_IF, IF_from_string_to_enum, -1, std::string("junk") );
// BENCHMARK_AND_COMPARE( check_from_string_to_enum_no_match_MAP, MAP_from_string_to_enum, -1, std::string("junk") );


// static auto unknown_str = std::string{ "unknown" };
// static auto new_york_str = std::string{ "new_york" };
// static auto washington_str = std::string{ "washington" };
// static auto new_jersey_str = std::string{ "new_jersey" };


// int IF_from_string_to_enum_2(const std::string& str)
// {
//     using namespace std::string_literals;

//     if( str == unknown_str ) return unknown;
//     else if( str == new_york_str ) return new_york;
//     else if( str == washington_str ) return washington;
//     else if( str == new_jersey_str ) return new_jersey;
//     else
//     {
//         return -1;
//     }    
// }

// int E_SWITCH_from_string_to_enum_2(const std::string& str)
// {
//     using namespace eswitch_v5;
//     using namespace std::string_literals;

//     return eswitch(str)
//     (
//         Case( unknown_str )     { return unknown; },
//         Case( new_york_str )    { return new_york; }, 
//         Case( washington_str )  { return washington; },
//         Case( new_jersey_str )  { return new_jersey; }, 
//         Default               { return -1; }
//     );
// }

// int MAP_from_string_to_enum_2(const std::string& str)
// {
//     using namespace std::string_literals;

//     static std::map< std::string, Place > cache{
//         { unknown_str, unknown },
//         { new_york_str, new_york },
//         { washington_str, washington },
//         { new_jersey_str, new_jersey },
//     };

//     auto iter = cache.find( str );
//     if( iter != std::end( cache ) ) return iter->second;

//     return -1;
// }

// // =====================================================

// BENCHMARK_AND_COMPARE( another_check_from_string_to_enum_1st_match_ESWITCH, E_SWITCH_from_string_to_enum_2, unknown, std::string("unknown") );
// BENCHMARK_AND_COMPARE( another_check_from_string_to_enum_1st_match_IF, IF_from_string_to_enum_2, unknown, std::string("unknown") );
// BENCHMARK_AND_COMPARE( another_check_from_string_to_enum_1st_match_MAP, MAP_from_string_to_enum_2, unknown, std::string("unknown") );

// // =====================================================

// BENCHMARK_AND_COMPARE( another_check_from_string_to_enum_2nd_match_ESWITCH, E_SWITCH_from_string_to_enum_2, new_york, std::string("new_york") );
// BENCHMARK_AND_COMPARE( another_check_from_string_to_enum_2nd_match_IF, IF_from_string_to_enum_2, new_york, std::string("new_york") );
// BENCHMARK_AND_COMPARE( another_check_from_string_to_enum_2nd_match_MAP, MAP_from_string_to_enum_2, new_york, std::string("new_york") );

// // =====================================================

// BENCHMARK_AND_COMPARE( another_check_from_string_to_enum_3rd_match_ESWITCH, E_SWITCH_from_string_to_enum_2, washington, std::string("washington") );
// BENCHMARK_AND_COMPARE( another_check_from_string_to_enum_3rd_match_IF, IF_from_string_to_enum_2, washington, std::string("washington") );
// BENCHMARK_AND_COMPARE( another_check_from_string_to_enum_3rd_match_MAP, MAP_from_string_to_enum_2, washington, std::string("washington") );

// // =====================================================

// BENCHMARK_AND_COMPARE( another_check_from_string_to_enum_4th_match_ESWITCH, E_SWITCH_from_string_to_enum_2, new_jersey, std::string("new_jersey") );
// BENCHMARK_AND_COMPARE( another_check_from_string_to_enum_4th_match_IF, IF_from_string_to_enum_2, new_jersey, std::string("new_jersey") );
// BENCHMARK_AND_COMPARE( another_check_from_string_to_enum_4th_match_MAP, MAP_from_string_to_enum_2, new_jersey, std::string("new_jersey") );

// // =====================================================

// BENCHMARK_AND_COMPARE( another_check_from_string_to_enum_no_match_ESWITCH,  E_SWITCH_from_string_to_enum_2, -1, std::string("junk") );
// BENCHMARK_AND_COMPARE( another_check_from_string_to_enum_no_match_IF,  IF_from_string_to_enum_2, -1, std::string("junk") );
// BENCHMARK_AND_COMPARE( another_check_from_string_to_enum_no_match_MAP,  MAP_from_string_to_enum_2, -1, std::string("junk") );

// // =====================================================

int main( int, char ** )
{
    int argc = 2;
    const char * argv[] = { "benchs", "--benchmark_out=bench.txt", nullptr };

	::benchmark::Initialize( &argc, const_cast< char** >( argv) ); 
	::benchmark::ConsoleReporter cr;
	::benchmark::JSONReporter jsnr;
	::benchmark::RunSpecifiedBenchmarks( &cr, &jsnr); 

	return 1;
}
