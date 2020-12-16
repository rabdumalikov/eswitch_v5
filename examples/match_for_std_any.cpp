//  Copyright (c) 2019-2021 Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[match_for_std_any]
#include <cassert>
#include <any>
#include <eswitch_v5.hpp>

int main()
{
    using namespace eswitch_v5;

    {   /// match
    std::any a = 888;

    auto result = eswitch( a )
    (
        Case( is<float>{} ) ( float f ) 
        { 
            return f;
        },
        Case( is<int>{} ) ( int i )
        { 
            return i;
        },
        Default { return -1; }
    );

    assert( result == 888 );
    }

    { /// no-match
    std::any a = std::string{"Hello"};

    auto result = eswitch( a )
    (
        Case( is<float>{} ) ( float f ) 
        { 
            return f; 
        },
        Case( is<int>{} ) ( int i )   
        { 
            return i; 
        },
        Default { return -1; }
    );

    assert( result == -1 );
    }
}
///[match_for_std_any]
