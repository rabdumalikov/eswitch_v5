//  Copyright (c) 2019-present Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[match_by_multiple_parameters]
#include <cassert>
#include <memory>
#include <eswitch_v5.hpp>

enum Place{ california, washington, new_york, new_jersey, las_vegas };

class Parser{/*code*/};

int main()
{
    using namespace eswitch_v5;

    {
        std::unique_ptr< Parser > parser( new Parser{} );
        
        const Place p = california;

        bool executed = false;

        eswitch( p, parser )
        (
            Case( _1 == california && _2 == nullptr ) { assert( false ); },
            Case( _1 == california && _2 != nullptr ) { executed = true; }
        );

        assert( executed );
    }    

    {
        std::unique_ptr< Parser > parser( new Parser{} );
        
        const Place p = california;

        bool executed = false;

        eswitch( p, parser )
        ( 
            Case( _1 == california || _2 == nullptr ) { executed = true; },
            Case( _1 == california && _2 != nullptr ) { assert( false ); }
        );

        assert( executed );
    }      
}
///[match_by_multiple_parameters]
