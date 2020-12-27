//  Copyright (c) 2019-present Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[extension_float_comparison]
#include <cassert>
#include <cmath>
#include <eswitch_v5.hpp>

namespace eswitch_v5 
{
    struct float_type
    {
        double value;
    };

    float_type operator""_flt( const long double f )
    {
        return { static_cast< double >( f ) };
    }

    bool operator==( const double d, float_type f )
    {
        return fabs( d - f.value ) < __FLT_EPSILON__;
    }

}

int main()
{
    using namespace eswitch_v5;

    auto r = eswitch( 1.300000000123 )
    (
        Case( 1.3_flt ) { return true; },
        Default { return false; }
    );

    assert( r );
}
///[extension_float_comparison]
