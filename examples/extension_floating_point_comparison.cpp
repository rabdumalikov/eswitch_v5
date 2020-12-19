//  Copyright (c) 2019-present Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[extension_floating_point_comparison]
#include <cassert>
#include <cmath>
#include <eswitch_v5.hpp>

struct double_value
{
    double value;
};

bool operator==( const double d, const double_value f )
{
    return fabs( d - f.value ) < __FLT_EPSILON__;
}

double_value operator""_dbl( const long double f )
{
    return { static_cast< double >( f ) };
}

int main()
{
    using namespace eswitch_v5;

    eswitch( 2 - 0.7000000001 )
    (
        Case( 1.3_dbl ) {},
        Default { assert( false ); }
    );
}
///[extension_floating_point_comparison]
