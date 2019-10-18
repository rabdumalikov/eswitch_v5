//  Copyright 2019 Rustam Abdumalikov.
//
//  "eswitch_v4" library
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include "benchmark/benchmark.h"
#include "eswitch_v4.hpp"
#include <iostream>

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

    int i = 0;

    switch (es3) {
    case SUCCESS:

        switch (ep3) {
        case RUNNING:
            break;
        case IN_PROGRES:
            break;
        case FINISHED:
            break;
        default:
            i = 3;
            break;
        }
        break;
    case FAILED:

        switch (ep3) {
        case RUNNING:
            i = 2;
            if (str == "Hell") {
                i = 1;
            }
            break;
        case IN_PROGRES:
            break;
        case FINISHED:
            break;
        default:
            i = 3;
            break;
        }

        break;
    default:
        i = 3;
        break;
    }

    return i;
}

int _eswitch(const std::string& str)
{
    using namespace eswitch_v4;

    estatus3 es3 = FAILED;
    eprogress3 ep3 = RUNNING;

    int i = 0;

    eswitch(es3, ep3, str) >>
    case_(_1 == SUCCESS) >> [&]
    {
        i = 0;
    } >>
    case_( _1 == FAILED && _2 == RUNNING && _3 == "Hello") >> [&]
    {
        i = 1;
    } >>
    case_( _1 == FAILED && _2 == RUNNING) >> [&]
    {
        i = 2;
    }
    >> 
    default_ >> [&]
    {
        i = 3;
    };

    return i;
}

void check_E_SWITCH(benchmark::State& state)
{
    std::string str("Hello");

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
            _eswitch(str));
    }
}

BENCHMARK(check_E_SWITCH);

void check_native_switch(benchmark::State& state)
{
    std::string str("Hello");

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
            _native_switch(str));
    }
}

BENCHMARK(check_native_switch);

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

    for (int i = 0; i < 10; ++i) {
        eswitch(i) >>
        //case_( _1 == 1 || _1 == 2 || _1 == 3 || _1 == 4 || _1 == 5 ) >> [&] // not optimal
        case_( _1 == any_from( 1, 2, 3, 4, 5 ) ) >> [&]
        {
            result = 1;
        }  >>       
        default_ >> [&]
        {
            result = 3;
        };
    }

    return result;
}

void check_E_SWITCH_forloop(benchmark::State& state)
{
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
            E_SWITCH_forloop());
    }
}

BENCHMARK(check_E_SWITCH_forloop);

void check_native_switch_forloop(benchmark::State& state)
{
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
            native_switch_forloop());
    }
}

BENCHMARK(check_native_switch_forloop);

// =====================================================

int native_switch_str_cmp(const std::string& str)
{
    int result = 0;

    switch (5) {
    case 5:
        if (str == "Hello") {
            result = 1;
        }
        break;
    default:
        result = 3;
        break;
    }

    return result;
}

int E_SWITCH_str_cmp(const std::string& str)
{
    using namespace eswitch_v4;

    int result = 0;

    eswitch(5, str) >>
    case_( _1 == 5 && _2 == "Hello") >> [&]
    {
        result = 1;
    } >>     
    default_ >> [&]
    {
        result = 3;
    };

    return result;
}

void check_E_SWITCH_str_cmp(benchmark::State& state)
{
    static const std::string str("Hello");

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
            E_SWITCH_str_cmp(str));
    }
}

BENCHMARK(check_E_SWITCH_str_cmp);

void check_native_switch_str_cmp(benchmark::State& state)
{
    static const std::string str("Hello");

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
            native_switch_str_cmp(str));
    }
}

BENCHMARK(check_native_switch_str_cmp);

// =====================================================

int native_switch_several_str_cmp(const std::string& str)
{
    int result = 0;

    switch (5) {
    case 5:
        if (str == "is") {
            result = -10;
        } else if (str == "my") {
            result = -20;
        } else if (str == "name") {
            result = -30;
        } else if (str == "Hell") {
            result = -40;
        } else if (str == "Hello") {
            result = 1;
        }
        break;
    default:
        result = 3;
        break;
    }

    return result;
}

int E_SWITCH_several_str_cmp(const std::string& str)
{
    using namespace eswitch_v4;

    int result = 0;

    eswitch(5, str) >>
    case_( _1 == 5 && _2 == "is") >> [&]
    {
        result = -10;
    } >>    
    case_( _1 == 5 && _2 == "my") >> [&]
    {
        result = -20;
    } >>     
    case_( _1 == 5 && _2 == "name" ) >> [&]
    {
        result = -30;
    } >>    
    case_( _1 == 5 && _2 == "Hell") >> [&]
    {
        result = -40;
    } >>    
    case_( _1 == 5 && _2 == "Hello") >> [&]
    {
        result = 1;
    } >>    
    default_ >> [&]
    {
        result = 3;
    };

    return result;
}

void check_E_SWITCH_several_str_cmp(benchmark::State& state)
{
    static const std::string str("Hello");

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
            E_SWITCH_several_str_cmp(str));
    }
}

BENCHMARK(check_E_SWITCH_several_str_cmp);

void check_native_switch_several_str_cmp(benchmark::State& state)
{
    static const std::string str("Hello");

    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
            native_switch_several_str_cmp(str));
    }
}

BENCHMARK(check_native_switch_several_str_cmp);

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
