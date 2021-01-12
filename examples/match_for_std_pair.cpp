//  Copyright (c) 2019-present Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[match_for_std_pair]
#include <cassert>
#include <string>
#include <eswitch_v5.hpp>

int main()
{
    using namespace eswitch_v5;
    using namespace std::string_literals;

    auto pr = std::make_pair( 10, "H"s );

    { // match

        auto result = eswitch( pr )
        (
            Case( 10, "H" ) { return true; },        
            Default { return false; }
        );

        assert( result );
    }

    { // partial-match

        auto result = eswitch( pr )
        (
            Case( _2 == "Hello" ) { return true; },        
            Default { return false; }
        );
        
        assert( !result );
    }

    { // no-match

        auto result = eswitch( pr )
        (
            Case( 10, "Hello" ) { return true; },        
            Default { return false; }
        );

        assert( !result );
    }
}
///[match_for_std_pair]
