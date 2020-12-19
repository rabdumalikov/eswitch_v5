//  Copyright (c) 2019-present Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[extension_value_and_type_transferring]
#include <cassert>
#include <cmath>
#include <eswitch_v5.hpp>

template< typename T >
struct Holder{};


// Intermediate structures which is needed for compiler 'name lookup'.
struct Has_value{};
struct Has_type {};

// - eswitch_v5::NoneIndex is mandatory here, since I have another 
//     overload of 'operator==' internally and because of it compiler
//     choose wrong implementation. Thus using concept 'eswitch_v5::NoneIndex'
//     prevent compiler from choosing wrong implementation.
// - Second argument must be 'const', since internally I keep it 
//     as const reference( because it shouldn't be mutable ) and
//     avoiding using 'const' will lead to the situation where 
//     compiler won't be able to match for this overload.

template< eswitch_v5::NoneIndex T >
auto operator==( T&&, const Has_value )
{
    if constexpr( requires{ T::value; } ) 
        return std::make_optional( T::value );
    else
        return false;
}

template< eswitch_v5::NoneIndex T >
auto operator==( T&&, const Has_type )
{
    if constexpr( requires{ typename T::type; } ) 
        return std::make_optional( Holder< typename T::type >{} );
    else
        return false;
}

int main()
{
    using namespace eswitch_v5;

    auto r = eswitch( std::decay< const int & >{} )
    (
        Case( Has_value{} )( int transferred_value ) 
        { 
            return transferred_value; 
        },
        Case( Has_type{} )( Holder< int > transferred_type )
        { 
            return sizeof( int ); 
        },
        Default { return 0; }
    );

    assert( r == sizeof( int ) );
}
///[extension_value_and_type_transferring]
