//  Copyright (c) 2019 Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
#include <assert.h>

#include <iostream>
#include <regex>


namespace eswitch_v5
{
    bool operator==( const std::string & v, const std::regex & rm )
    {
        return std::regex_match( v, rm );
    }
}

#include "eswitch_v5.hpp"


template< eswitch_v5::Comparison_operators Cmp, int I, typename T2 >
eswitch_v5::condition< eswitch_v5::Index_< I >, T2 > make_condition( const eswitch_v5::Index_< I > & t1, T2 && t2 )
{
    return eswitch_v5::condition< eswitch_v5::Index_< I >, T2 >( Cmp, std::forward< T2 >( t2 ) );
}

namespace eswitch_v5
{
/////////////////////////////////////////////////////////////////////////////////////////////

    template< int I >
    auto operator==( const eswitch_v5::Index_< I > & val, const std::regex & rgx )
    {
        return make_condition< eswitch_v5::Comparison_operators::equal_ >( val, rgx );
    }

/////////////////////////////////////////////////////////////////////////////////////////////

    template< int I >
    auto operator==( const eswitch_v5::Index_< I > & val, std::regex & rgx )
    {
        return make_condition< eswitch_v5::Comparison_operators::equal_ >( val, rgx );
    }

/////////////////////////////////////////////////////////////////////////////////////////////

    template< int I >
    auto operator==( const eswitch_v5::Index_< I > & val, std::regex && rgx )
    {
        return make_condition< eswitch_v5::Comparison_operators::equal_ >( val, rgx );
    }
/////////////////////////////////////////////////////////////////////////////////////////////
}

std::regex operator "" _r( const char* rgx, size_t )
{
  return std::regex( rgx );
}

int main( /* EXAMPLE NAME: CUSTOM EXTENTION( FOR REGEX ) */ )
{
    using namespace eswitch_v5;

    const auto phone_number = "^[+]*[(]{0,1}[0-9]{1,4}[)]{0,1}[-\\s\\./0-9]*$"_r;
    const auto email = "[\\w-]+@([\\w-]+\\.)+[\\w-]+"_r;

    {
        bool executed = false;

        std::string text{ "joe@aol.com" };

        eswitch( text ) >> 
            case_( _1 == phone_number ) >> []{ assert( false ); } >>
            case_( _1 == email )        >> [&]{ executed = true; };

        assert( executed );
    }

    {
        bool executed = false;

        std::string text{ "joe@aol.com" };

        eswitch( text ) >> 
            case_( phone_number ) >> []{ assert( false ); } >>
            case_( email )        >> [&]{ executed = true; };

        assert( executed );
    }

    {
        bool executed = false;

        std::string text{ "+(123)-456-78-90" };

        eswitch( text ) >> 
            case_( _1 == phone_number ) >> [&]{ executed = true; } >>
            case_( _1 == email )        >> [&]{ assert( false ); };

        assert( executed );
    }

    {
        bool executed = false;

        std::string text{ "+(123)-456-78-90" };

        eswitch( text ) >> 
            case_( phone_number ) >> [&]{ executed = true; } >>
            case_( email )        >> [&]{ assert( false ); };

        assert( executed );
    }

}
