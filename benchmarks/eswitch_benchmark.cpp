//  Copyright (c) 2019 Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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

// int _native_switch( const estatus3 es3, const eprogress3 ep3, const std::string& str)
// {
//     switch (es3) {
//     case SUCCESS:

//         switch (ep3) {
//         case RUNNING: break;
//         case IN_PROGRES: break;
//         case FINISHED: break;
//         default: return 3;
//         }
//         return 0;
//     case FAILED:

//         switch (ep3) {
//         case RUNNING:
//             if (str == "Hello") return 1;
//             else 
//                 return 2;
//         case IN_PROGRES: break;
//         case FINISHED: break;
//         default: return 3;
//         }

//         break;
//     default:
//         return 3;
//     }
// }

// int _eswitch( const estatus3 es3, const eprogress3 ep3, const std::string& str )
// {
//     using namespace eswitch_v5;

//     return eswitch(es3, ep3, str)
//     ( 
//         Case( _1 == SUCCESS) { return 0; },
//         Case( _1 == FAILED && _2 == RUNNING && _3 == "Hello") { return 1; },
//         Case( _1 == FAILED && _2 == RUNNING) { return 2; }, 
//         Default { return 3; }
//     );
// }

// BENCHMARK_IT( check_E_SWITCH, _eswitch, FAILED, RUNNING, std::string{"Hello"} );
// BENCHMARK_IT( check_native_switch, _native_switch, FAILED, RUNNING, std::string{"Hello"} );

// // =====================================================

// int native_switch_forloop()
// {
//     int result = 0;
//     for (int i = 0; i < 10; ++i) {
//         switch (i) {
//         case 1:
//         case 2:
//         case 3:
//         case 4:
//         case 5:
//             result = 1;
//             break;
//         default:
//             result = 3;
//             break;
//         }
//     }

//     return result;
// }

// int E_SWITCH_forloop()
// {
//     using namespace eswitch_v5;

//     int result = 0;

//     for (int i = 0; i < 10; ++i) 
//     {
//         result = eswitch(i)
//         (
//             //Case( _1 == 1 || _1 == 2 || _1 == 3 || _1 == 4 || _1 == 5 ) >> to_return(1) >> // not optimal
//             Case( _1 == any_from( 1, 2, 3, 4, 5 ) ) { return 1; }, 
//             Default { return 3; }
//         );
//     }

//     return result;
// }

// BENCHMARK_IT_WITHOUT_ARGS( check_E_SWITCH_forloop, E_SWITCH_forloop );
// BENCHMARK_IT_WITHOUT_ARGS( check_native_switch_forloop, native_switch_forloop );

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

// BENCHMARK_IT( check_E_SWITCH_str_cmp, E_SWITCH_str_cmp, std::string("Hello") );
// BENCHMARK_IT( check_native_switch_str_cmp, native_switch_str_cmp, std::string("Hello") );

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

// BENCHMARK_IT( check_E_SWITCH_several_str_cmp, E_SWITCH_several_str_cmp, std::string("Hello") );
// BENCHMARK_IT( check_native_switch_several_str_cmp, native_switch_several_str_cmp, std::string("Hello") );

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
    
    //auto r = eswitch(str);
    //return case_( "unknown" )( std::make_tuple( str ) );

    return eswitch(str)
    (
        Case( "unknown" )     { return unknown; },
        Case( "new_york" )    { return new_york; }, 
        // Case( "washington" )  { return washington; },
        // Case( "new_jersey" )  { return new_jersey; }, 
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


BENCHMARK_IT( check_E_SWITCH_from_string_to_enum_1st_match, E_SWITCH_from_string_to_enum, std::string("unknown") );
//BENCHMARK_IT( check_IF_from_string_to_enum_1st_match, IF_from_string_to_enum, std::string("unknown") );
//BENCHMARK_IT( check_MAP_from_string_to_enum_1st_match, MAP_from_string_to_enum, std::string("unknown") );

// =====================================================

BENCHMARK_IT( check_E_SWITCH_from_string_to_enum_2nd_match, E_SWITCH_from_string_to_enum, std::string("new_york") );
// BENCHMARK_IT( check_IF_from_string_to_enum_2nd_match, IF_from_string_to_enum, std::string("new_york") );
// BENCHMARK_IT( check_MAP_from_string_to_enum_2nd_match, MAP_from_string_to_enum, std::string("new_york") );

// =====================================================

BENCHMARK_IT( check_E_SWITCH_from_string_to_enum_3rd_match, E_SWITCH_from_string_to_enum, std::string("washington") );
// BENCHMARK_IT( check_IF_from_string_to_enum_3rd_match, IF_from_string_to_enum, std::string("washington") );
// BENCHMARK_IT( check_MAP_from_string_to_enum_3rd_match, MAP_from_string_to_enum, std::string("washington") );

// =====================================================

BENCHMARK_IT( check_E_SWITCH_from_string_to_enum_4th_match, E_SWITCH_from_string_to_enum, std::string("new_jersey") );
// BENCHMARK_IT( check_IF_from_string_to_enum_4th_match, IF_from_string_to_enum, std::string("new_jersey") );
// BENCHMARK_IT( check_MAP_from_string_to_enum_4th_match, MAP_from_string_to_enum, std::string("new_jersey") );

// =====================================================

BENCHMARK_IT( check_E_SWITCH_from_string_to_enum_no_match, E_SWITCH_from_string_to_enum, std::string("junk") );
//BENCHMARK_IT( check_IF_from_string_to_enum_no_match, IF_from_string_to_enum, std::string("junk") );
//BENCHMARK_IT( check_MAP_from_string_to_enum_no_match, MAP_from_string_to_enum, std::string("junk") );

// =====================================================
BENCHMARK_IT( check_IF_from_string_to_enum_1st_match, IF_from_string_to_enum, std::string("unknown") );
BENCHMARK_IT( check_IF_from_string_to_enum_2nd_match, IF_from_string_to_enum, std::string("new_york") );
BENCHMARK_IT( check_IF_from_string_to_enum_3nd_match, IF_from_string_to_enum, std::string("washington") );
BENCHMARK_IT( check_IF_from_string_to_enum_4nd_match, IF_from_string_to_enum, std::string("new_jersey") );
BENCHMARK_IT( check_IF_from_string_to_enum_no_match, IF_from_string_to_enum, std::string("junk") );

// 
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
