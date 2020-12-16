//  Copyright (c) 2019-2021 Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[match_for_std_variant]
#include <cassert>
#include <variant>
#include <eswitch_v5.hpp>

int main()
{
    using namespace eswitch_v5;

    std::variant< int, std::string > var = std::string{"Hello"};

    auto result = eswitch( var )
    (
        Case( is<std::string>{} ) ( const std::string & str ) 
        { 
            return str.size(); 
        },
        Case( is<int>{} ) ( int i ) 
        { 
            return i; 
        },
        Default { return -1; }
    );

    assert( result == 5 );
}
///[match_for_std_variant]
