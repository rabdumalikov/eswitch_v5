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
#include <iostream>


#define BENCHMARK_IT_WITHOUT_ARGS( name, function_to_benchmark )\
void name(benchmark::State& state)\
{\
\
    while (state.KeepRunning()) {\
        benchmark::DoNotOptimize(\
            function_to_benchmark());\
    }\
}\
BENCHMARK(name);

#define BENCHMARK_IT( name, function_to_benchmark, ... )\
void name(benchmark::State& state)\
{\    
\
    while (state.KeepRunning()) {\
        const auto res = function_to_benchmark(__VA_ARGS__);\
        benchmark::DoNotOptimize(res);\
    }\
}\
BENCHMARK(name);


#define BENCHMARK_AND_COMPARE( name, function_to_benchmark, actual_result, ... )\
void name(benchmark::State& state)\
{\    
\
    while (state.KeepRunning()) {\
        const auto res = function_to_benchmark(__VA_ARGS__);\
        assert( res == actual_result );\
        benchmark::DoNotOptimize(res);\
    }\
}\
BENCHMARK(name);


const int AMOUNT_ITERATION = 1000000;

enum estatus3 {
    SUCCESS,
    FAILED
};

enum eprogress3 {
    RUNNING,
    IN_PROGRES,
    FINISHED
};

int _native_switch( const estatus3 es3, const eprogress3 ep3, const std::string& str)
{
    switch (es3) {
    case SUCCESS:

        switch (ep3) {
        case RUNNING: break;
        case IN_PROGRES: break;
        case FINISHED: break;
        default: return 3;
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
        default: return 3;
        }

        break;
    default:
        return 3;
    }
}

int _eswitch( const estatus3 es3, const eprogress3 ep3, const std::string& str )
{
    using namespace eswitch_v5;

    return eswitch(es3, ep3, str)
    ( 
        Case( _1 == SUCCESS) { return 0; },
        Case( _1 == FAILED && _2 == RUNNING && _3 == "Hello") { return 1; },
        Case( _1 == FAILED && _2 == RUNNING) { return 2; }, 
        Default { return 3; }
    );
}

BENCHMARK_IT( check_multiple_matches_E_SWITCH, _eswitch, FAILED, RUNNING, std::string{"Hello"} );
BENCHMARK_IT( check_multiple_matches_NativeSwitch, _native_switch, FAILED, RUNNING, std::string{"Hello"} );

// =====================================================

int native_switch_forloop()
{
    int result = 0;
    for (int i = 0; i < 10; ++i) {
        switch (i) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            result = 1;
            break;
        default:
            result = 3;
            break;
        }
    }

    return result;
}

int E_SWITCH_forloop()
{
    using namespace eswitch_v5;

    int result = 0;

    for (int i = 0; i < 10; ++i) 
    {
        result = eswitch(i)
        (
            //Case( _1 == 1 || _1 == 2 || _1 == 3 || _1 == 4 || _1 == 5 ) >> to_return(1) >> // not optimal
            Case( _1 == any_from( 1, 2, 3, 4, 5 ) ) { return 1; }, 
            Default { return 3; }
        );
    }

    return result;
}

BENCHMARK_IT_WITHOUT_ARGS( check_forloop_ESWITCH, E_SWITCH_forloop );
BENCHMARK_IT_WITHOUT_ARGS( check_forloop_NativeSwitch, native_switch_forloop );

// =====================================================

int native_switch_range_check( const int value )
{
    if( value > 0 && value < 10 ) return 0;
    else if( value >= 10 && value <= 20 ) return 11;
    else if( value > 30 && value <= 50 ) return 40;
}

int E_SWITCH_range_check( const int value )
{
    using namespace eswitch_v5;

    return eswitch( value )
    (
        Case( _1 > 0 && _1 < 10 ) { return 0; },
        Case( _1 >= 10 && _1 <= 20 ) { return 11; },
        Case( _1 > 30 && _1 <= 50 ) { return 40; }
    );
}

BENCHMARK_IT( check_range_check_1st_match_ESWITCH, E_SWITCH_range_check, 5 );
BENCHMARK_IT( check_range_check_1st_match_NativeSwitch, native_switch_range_check, 5 );
BENCHMARK_IT( check_range_check_2nd_match_ESWITCH, E_SWITCH_range_check, 10 );
BENCHMARK_IT( check_range_check_2nd_match_NativeSwitch, native_switch_range_check, 10 );
BENCHMARK_IT( check_range_check_3rd_match_ESWITCH, E_SWITCH_range_check, 31 );
BENCHMARK_IT( check_range_check_3rd_match_NativeSwitch, native_switch_range_check, 31 );


int native_switch_std_any( const std::any & value )
{
    if( auto * v = std::any_cast< int >( &value ) ) return *v;
    else if( auto * v = std::any_cast< std::string >( &value ) ) return v->size();
    else
    {
        return -1;
    }   
}

int E_SWITCH_std_any( const std::any & value )
{
    using namespace eswitch_v5;

    return eswitch( value )
    (
        Case( _1 == is< int >{} )( const int v ) { return v; },
        Case( _1 == is< std::string >{} )( const std::string & v ) { return v.size(); },
        Default { return -1; }
    );
}

BENCHMARK_AND_COMPARE( check_std_any_1st_match_ESWITCH, E_SWITCH_std_any, 10, std::any{ 10 } );
BENCHMARK_AND_COMPARE( check_std_any_1st_match_NativeSwitch, native_switch_std_any, 10, std::any{ 10 } );
BENCHMARK_AND_COMPARE( check_std_any_2nd_match_ESWITCH, E_SWITCH_std_any, 4, std::any{ std::string{ "Hell" } } );
BENCHMARK_AND_COMPARE( check_std_any_2nd_match_NativeSwitch, native_switch_std_any, 4, std::any{ std::string{ "Hell" } } );
BENCHMARK_AND_COMPARE( check_std_any_3rd_match_ESWITCH, E_SWITCH_std_any, -1, std::any{ double{ 0.0 } } );
BENCHMARK_AND_COMPARE( check_std_any_3rd_match_NativeSwitch, native_switch_std_any, -1, std::any{ double{ 0.0 } } );

int native_switch_std_variant( const std::variant< int, std::string > & value )
{
    if( auto * v = std::get_if< int >( &value ) ) return *v;
    else if( auto * v = std::get_if< std::string >( &value ) ) return v->size();
    else
    {
        return -1;
    }   
}

int E_SWITCH_std_variant( const std::variant< int, std::string > & value )
{
    using namespace eswitch_v5;

    return eswitch( value )
    (
        Case( is< int >{} )( const int v ) { return v; },
        Case( is< std::string >{} )( const std::string & v ) { return v.size(); },
        Default { return -1; }
    );
}

BENCHMARK_AND_COMPARE( check_std_variant_1st_match_ESWITCH, E_SWITCH_std_variant, 10, std::variant< int, std::string >{ 10 } );
BENCHMARK_AND_COMPARE( check_std_variant_1st_match_NativeSwitch, native_switch_std_variant, 10, std::variant< int, std::string >{ 10 } );

static auto rgx_for_match = std::regex( ".*: .*" );

auto native_switch_std_regex_match( const std::string & text )
{
    if( std::smatch match; std::regex_match( text, match, rgx_for_match ) )
    {
        return true;
    }
    return false;
}

int E_SWITCH_std_regex_match( const std::string & value )
{
    using namespace eswitch_v5;

    return eswitch( value )
    (
        Case( rgx_for_match ) { return true; },
        Default { return false; }
    );
}

BENCHMARK_IT( check_std_regex_match_ESWITCH, E_SWITCH_std_regex_match, std::string{ "Key: value" } );
BENCHMARK_IT( check_std_regex_match_NativeSwitch, native_switch_std_regex_match, std::string{ "Key: value" } );

static auto rgx_for_match_with_withdrawal = std::regex( "(.*): (.*)" );

auto native_switch_std_regex_match_with_withdrawal( const std::string & text )
{
    if( std::smatch match; std::regex_match( text, match, rgx_for_match_with_withdrawal ) )
    {
        std::vector< std::string > vs;
        vs.reserve( match.size() );

        for( const auto & v : match )  
            vs.push_back( v );

        return vs;
    }
    return std::vector< std::string >{};
}

auto E_SWITCH_std_regex_match_with_withdrawal( const std::string & value )
{
    using namespace eswitch_v5;

    return eswitch( value )
    (
        Case( rgx_for_match_with_withdrawal )( std::vector< std::string > && vs ) { return std::move( vs ); },
        Default { return std::vector< std::string >{}; }
    );
}

BENCHMARK_IT( check_std_regex_match_with_withdrawal_ESWITCH, E_SWITCH_std_regex_match_with_withdrawal, std::string{ "Key: value" } );
BENCHMARK_IT( check_std_regex_match_with_withdrawal_NativeSwitch, native_switch_std_regex_match_with_withdrawal, std::string{ "Key: value" } );


// =====================================================

int native_switch_str_cmp(const std::string& str)
{
    switch (5) {
    case 5:
        if (str == "Hello") return 1;
        break;
    default:
        return 3;
    }
}

int E_SWITCH_str_cmp(const std::string& str)
{
    using namespace eswitch_v5;

    return eswitch(5, str)
    (
        Case( _1 == 5 && _2 == "Hello") { return 1; },
        Default { return 3; }
    );
}

BENCHMARK_IT( check_str_cmp_ESWITCH, E_SWITCH_str_cmp, std::string("Hello") );
BENCHMARK_IT( check_str_cmp_NativeSwitch, native_switch_str_cmp, std::string("Hello") );

// =====================================================

int native_switch_several_str_cmp(const std::string& str)
{
    switch (5) {
    case 5:
        if (str == "is") return -10;
        else if (str == "my") return -20;
        else if (str == "name") return -30;
        else if (str == "Hell") return -40;
        else if (str == "Hello") return 1;        
    default:
        return 3;
    }
}

int E_SWITCH_several_str_cmp(const std::string& str)
{
    using namespace eswitch_v5;

    return eswitch(5, str)
    (
        Case( _1 == 5 && _2 == "is") { return -10; },
        Case( _1 == 5 && _2 == "my") { return -20; },     
        Case( _1 == 5 && _2 == "name" ) { return -30; },
        Case( _1 == 5 && _2 == "Hell") { return -40; },
        Case( _1 == 5 && _2 == "Hello") { return 1; },
        Default { return 3; }
    );
}

BENCHMARK_IT( check_several_str_cmp_ESWITCH, E_SWITCH_several_str_cmp, std::string("Hello") );
BENCHMARK_IT( check_several_str_cmp_NativeSwitch, native_switch_several_str_cmp, std::string("Hello") );

// =====================================================

enum Place { unknown = 0, new_york=5, washington=129, new_jersey=501 };

int IF_from_string_to_enum(const std::string& str)
{
    if( str == "unknown" ) return unknown;
    else if( str == "new_york" ) return new_york;
    else if( str == "washington" ) return washington;
    else if( str == "new_jersey" ) return new_jersey;
    else
    {
        return -1;
    }    
}

int E_SWITCH_from_string_to_enum(const std::string& str)
{
    using namespace eswitch_v5;
    
    return eswitch(str)
    (
        Case( "unknown" )     { return unknown; },
        Case( "new_york" )    { return new_york; }, 
        Case( "washington" )  { return washington; },
        Case( "new_jersey" )  { return new_jersey; }, 
        Default               { return -1; }
    );
}

int MAP_from_string_to_enum(const std::string& str)
{
    static std::map< std::string, Place > cache{
        { "unknown", unknown },
        { "new_york", new_york },
        { "washington", washington },
        { "new_jersey", new_jersey },
    };

    auto iter = cache.find( str );
    if( iter != std::end( cache ) ) return iter->second;

    return -1;
}


BENCHMARK_IT( check_from_string_to_enum_1st_match_ESWITCH, E_SWITCH_from_string_to_enum, std::string("unknown") );
BENCHMARK_IT( check_from_string_to_enum_1st_match_IF, IF_from_string_to_enum, std::string("unknown") );
BENCHMARK_IT( check_from_string_to_enum_1st_match_MAP, MAP_from_string_to_enum, std::string("unknown") );

// =====================================================

BENCHMARK_IT( check_from_string_to_enum_2nd_match_ESWITCH, E_SWITCH_from_string_to_enum, std::string("new_york") );
BENCHMARK_IT( check_from_string_to_enum_2nd_match_IF, IF_from_string_to_enum, std::string("new_york") );
BENCHMARK_IT( check_from_string_to_enum_2nd_match_MAP, MAP_from_string_to_enum, std::string("new_york") );

// =====================================================

BENCHMARK_IT( check_from_string_to_enum_3rd_match_ESWITCH, E_SWITCH_from_string_to_enum, std::string("washington") );
BENCHMARK_IT( check_from_string_to_enum_3rd_match_IF, IF_from_string_to_enum, std::string("washington") );
BENCHMARK_IT( check_from_string_to_enum_3rd_match_MAP, MAP_from_string_to_enum, std::string("washington") );

// =====================================================

BENCHMARK_IT( check_from_string_to_enum_4th_match_ESWITCH, E_SWITCH_from_string_to_enum, std::string("new_jersey") );
BENCHMARK_IT( check_from_string_to_enum_4th_match_IF, IF_from_string_to_enum, std::string("new_jersey") );
BENCHMARK_IT( check_from_string_to_enum_4th_match_MAP, MAP_from_string_to_enum, std::string("new_jersey") );

// =====================================================

BENCHMARK_IT( check_from_string_to_enum_no_match_ESWITCH, E_SWITCH_from_string_to_enum, std::string("junk") );
BENCHMARK_IT( check_from_string_to_enum_no_match_IF, IF_from_string_to_enum, std::string("junk") );
BENCHMARK_IT( check_from_string_to_enum_no_match_MAP, MAP_from_string_to_enum, std::string("junk") );


static auto unknown_str = std::string{ "unknown" };
static auto new_york_str = std::string{ "new_york" };
static auto washington_str = std::string{ "washington" };
static auto new_jersey_str = std::string{ "new_jersey" };


int IF_from_string_to_enum_2(const std::string& str)
{
    using namespace std::string_literals;

    if( str == unknown_str ) return unknown;
    else if( str == new_york_str ) return new_york;
    else if( str == washington_str ) return washington;
    else if( str == new_jersey_str ) return new_jersey;
    else
    {
        return -1;
    }    
}

int E_SWITCH_from_string_to_enum_2(const std::string& str)
{
    using namespace eswitch_v5;
    using namespace std::string_literals;

    return eswitch(str)
    (
        Case( unknown_str )     { return unknown; },
        Case( new_york_str )    { return new_york; }, 
        Case( washington_str )  { return washington; },
        Case( new_jersey_str )  { return new_jersey; }, 
        Default               { return -1; }
    );
}

int MAP_from_string_to_enum_2(const std::string& str)
{
    using namespace std::string_literals;

    static std::map< std::string, Place > cache{
        { unknown_str, unknown },
        { new_york_str, new_york },
        { washington_str, washington },
        { new_jersey_str, new_jersey },
    };

    auto iter = cache.find( str );
    if( iter != std::end( cache ) ) return iter->second;

    return -1;
}

// =====================================================

BENCHMARK_IT( another_check_from_string_to_enum_1st_match_ESWITCH, E_SWITCH_from_string_to_enum_2, std::string("unknown") );
BENCHMARK_IT( another_check_from_string_to_enum_1st_match_IF, IF_from_string_to_enum_2, std::string("unknown") );
BENCHMARK_IT( another_check_from_string_to_enum_1st_match_MAP, MAP_from_string_to_enum_2, std::string("unknown") );

// =====================================================

BENCHMARK_IT( another_check_from_string_to_enum_2nd_match_ESWITCH, E_SWITCH_from_string_to_enum_2, std::string("new_york") );
BENCHMARK_IT( another_check_from_string_to_enum_2nd_match_IF, IF_from_string_to_enum_2, std::string("new_york") );
BENCHMARK_IT( another_check_from_string_to_enum_2nd_match_MAP, MAP_from_string_to_enum_2, std::string("new_york") );

// =====================================================

BENCHMARK_IT( another_check_from_string_to_enum_3rd_match_ESWITCH, E_SWITCH_from_string_to_enum_2, std::string("washington") );
BENCHMARK_IT( another_check_from_string_to_enum_3rd_match_IF, IF_from_string_to_enum_2, std::string("washington") );
BENCHMARK_IT( another_check_from_string_to_enum_3rd_match_MAP, MAP_from_string_to_enum_2, std::string("washington") );

// =====================================================

BENCHMARK_IT( another_check_from_string_to_enum_4th_match_ESWITCH, E_SWITCH_from_string_to_enum_2, std::string("new_jersey") );
BENCHMARK_IT( another_check_from_string_to_enum_4th_match_IF, IF_from_string_to_enum_2, std::string("new_jersey") );
BENCHMARK_IT( another_check_from_string_to_enum_4th_match_MAP, MAP_from_string_to_enum_2, std::string("new_jersey") );

// =====================================================

BENCHMARK_IT( another_check_from_string_to_enum_no_match_ESWITCH,  E_SWITCH_from_string_to_enum_2, std::string("junk") );
BENCHMARK_IT( another_check_from_string_to_enum_no_match_IF,  IF_from_string_to_enum_2, std::string("junk") );
BENCHMARK_IT( another_check_from_string_to_enum_no_match_MAP,  MAP_from_string_to_enum_2, std::string("junk") );

// =====================================================

int main( int, char ** )
{
    int argc = 2;
    char* argv[] = { "benchs", "--benchmark_out=bench.txt", nullptr };

	::benchmark::Initialize(&argc, argv); 
	::benchmark::ConsoleReporter cr;
	::benchmark::JSONReporter jsnr;
	::benchmark::RunSpecifiedBenchmarks( &cr, &jsnr); 

	return 1;
}
