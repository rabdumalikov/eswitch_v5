//  Copyright (c) 2019-present Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[match_for_polymorphic_types]
#include <cassert>
#include <eswitch_v5.hpp>

struct base
{ 
    virtual ~base() = default; 
    virtual int area() = 0; 
};
struct circle : base{ int area() { return 10; } };
struct square : base{ int area() { return 20; } };

int main()
{
    using namespace eswitch_v5;

    circle c;
    base & b = c;

    auto result = eswitch( b )
    (
        Case( is<circle>{} )( circle & c ) 
        { 
            return c.area(); 
        },
        Case( is<square>{} )( square & s ) 
        { 
            return s.area(); 
        },
        Default { return -1; }
    );

    assert( result == 10 );
}
///[match_for_polymorphic_types]
