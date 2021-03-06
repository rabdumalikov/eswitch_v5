//  Copyright (c) 2019-present Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[match_via_regex]
#include <cassert>
#include <string>
#include <eswitch_v5.hpp>

int main()
{
    using namespace eswitch_v5;
    using namespace std::string_literals;

    const auto phone_number = "^[+]*[(]{0,1}[0-9]{1,4}[)]{0,1}[-\\s\\./0-9]*$"_r;
    const auto email = "[\\w-]+@([\\w-]+\\.)+[\\w-]+"_r;

    {
        bool executed = false;

        eswitch( "joe@aol.com"s ) 
        (
            Case( phone_number ) { assert( false ); },
            Case( email )        { executed = true; }
        );
    
        assert( executed );
    }

    {
        bool executed = false;

        eswitch( "+(123)-456-78-90"s ) 
        (
            Case( phone_number ) { executed = true; },
            Case( email )        { assert( false ); }
        );

        assert( executed );
    }
}
///[match_via_regex]
