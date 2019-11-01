//  Copyright (c) 2019 Rustam Abdumalikov
//
//  "eswitch_v4" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "benchmark/benchmark.h"
#include "eswitch_v4.hpp"
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

#define BENCHMARK_IT( name, args, function_to_benchmark )\
void name(benchmark::State& state)\
{\
    auto arg = args;\
\
    while (state.KeepRunning()) {\
        benchmark::DoNotOptimize(\
            function_to_benchmark(arg));\
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

int _native_switch(const std::string& str)
{
    estatus3 es3 = FAILED;
    eprogress3 ep3 = RUNNING;

    switch (es3) {
    case SUCCESS:

        switch (ep3) {
        case RUNNING: break;
        case IN_PROGRES: break;
        case FINISHED: break;
        default: return 3;
        }
        break;
    case FAILED:

        switch (ep3) {
        case RUNNING:
            if (str == "Hell") return 1;
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

int _eswitch(const std::string& str)
{
    using namespace eswitch_v4;

    estatus3 es3 = FAILED;
    eprogress3 ep3 = RUNNING;

    return eswitch(es3, ep3, str) >>
        case_(_1 == SUCCESS) >> to_return(0) >>
        case_( _1 == FAILED && _2 == RUNNING && _3 == "Hello") >> to_return(1) >>
        case_( _1 == FAILED && _2 == RUNNING) >> to_return(2) >>     
        default_ >> to_return(3) >> in_place_return_;
}

BENCHMARK_IT( check_E_SWITCH, std::string{"Hello"}, _eswitch );
BENCHMARK_IT( check_native_switch, std::string{"Hello"}, _native_switch );

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
    using namespace eswitch_v4;

    int result = 0;

    for (int i = 0; i < 10; ++i) 
    {
        result = eswitch(i) >>
            //case_( _1 == 1 || _1 == 2 || _1 == 3 || _1 == 4 || _1 == 5 ) >> to_return(1) >> // not optimal
            case_( _1 == any_from( 1, 2, 3, 4, 5 ) ) >> to_return(1) >>
            default_ >> to_return(3) >> in_place_return_;
    }

    return result;
}

BENCHMARK_IT_WITHOUT_ARGS( check_E_SWITCH_forloop, E_SWITCH_forloop );
BENCHMARK_IT_WITHOUT_ARGS( check_native_switch_forloop, native_switch_forloop );

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
    using namespace eswitch_v4;

    return eswitch(5, str) >>
        case_( _1 == 5 && _2 == "Hello") >> to_return(1) >>
        default_ >> to_return(3) >>
        in_place_return_;
}

BENCHMARK_IT( check_E_SWITCH_str_cmp, std::string("Hello"), E_SWITCH_str_cmp );
BENCHMARK_IT( check_native_switch_str_cmp, std::string("Hello"), native_switch_str_cmp );

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
    using namespace eswitch_v4;

    return eswitch(5, str) >>
        case_( _1 == 5 && _2 == "is") >> to_return(-10) >>
        case_( _1 == 5 && _2 == "my") >> to_return(-20) >>     
        case_( _1 == 5 && _2 == "name" ) >> to_return(-30) >>
        case_( _1 == 5 && _2 == "Hell") >> to_return(-40) >>
        case_( _1 == 5 && _2 == "Hello") >> to_return(1) >>
        default_ >> to_return(3) >>
        in_place_return_;
}

BENCHMARK_IT( check_E_SWITCH_several_str_cmp, std::string("Hello"), E_SWITCH_several_str_cmp );
BENCHMARK_IT( check_native_switch_several_str_cmp, std::string("Hello"), native_switch_several_str_cmp );

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
    using namespace eswitch_v4;
    
    return eswitch(str) >>
        case_( "unknown" )    >> to_return(unknown) >>
        case_( "new_york" )   >> to_return(new_york) >> 
        case_( "washington" ) >> to_return(washington) >> 
        case_( "new_jersey" ) >> to_return(new_jersey) >> 
        default_              >> to_return( -1 ) >>
        in_place_return_;
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


BENCHMARK_IT( check_E_SWITCH_from_string_to_enum_1st_match, std::string("unknown"), E_SWITCH_from_string_to_enum );
BENCHMARK_IT( check_IF_from_string_to_enum_1st_match, std::string("unknown"), IF_from_string_to_enum );
BENCHMARK_IT( check_MAP_from_string_to_enum_1st_match, std::string("unknown"), MAP_from_string_to_enum );

// =====================================================

BENCHMARK_IT( check_E_SWITCH_from_string_to_enum_2nd_match, std::string("new_york"), E_SWITCH_from_string_to_enum );
BENCHMARK_IT( check_IF_from_string_to_enum_2nd_match, std::string("new_york"), IF_from_string_to_enum );
BENCHMARK_IT( check_MAP_from_string_to_enum_2nd_match, std::string("new_york"), MAP_from_string_to_enum );

// =====================================================

BENCHMARK_IT( check_E_SWITCH_from_string_to_enum_3rd_match, std::string("washington"), E_SWITCH_from_string_to_enum );
BENCHMARK_IT( check_IF_from_string_to_enum_3rd_match, std::string("washington"), IF_from_string_to_enum );
BENCHMARK_IT( check_MAP_from_string_to_enum_3rd_match, std::string("washington"), MAP_from_string_to_enum );

// =====================================================

BENCHMARK_IT( check_E_SWITCH_from_string_to_enum_4th_match, std::string("new_jersey"), E_SWITCH_from_string_to_enum );
BENCHMARK_IT( check_IF_from_string_to_enum_4th_match, std::string("new_jersey"), IF_from_string_to_enum );
BENCHMARK_IT( check_MAP_from_string_to_enum_4th_match, std::string("new_jersey"), MAP_from_string_to_enum );

// =====================================================

BENCHMARK_IT( check_E_SWITCH_from_string_to_enum_no_match, std::string("junk"), E_SWITCH_from_string_to_enum );
BENCHMARK_IT( check_IF_from_string_to_enum_no_match, std::string("junk"), IF_from_string_to_enum );
BENCHMARK_IT( check_MAP_from_string_to_enum_no_match, std::string("junk"), MAP_from_string_to_enum );

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
