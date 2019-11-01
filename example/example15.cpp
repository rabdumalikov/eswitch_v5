//  Copyright (c) 2019 Rustam Abdumalikov
//
//  "eswitch_v4" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
#include <assert.h>

#include "eswitch_v4.hpp"
#include <iostream>
#include <regex>

struct RegexMatcher
{
    std::regex rgx_;

    RegexMatcher( const std::regex & rgx ) : rgx_( rgx ){}

    friend bool operator==( const std::string & v, const RegexMatcher & rm )
    {
        return std::regex_match( v, rm.rgx_ );
    }

    friend bool operator!=( const std::string & v, const RegexMatcher & rm )
    {
        return !( v == rm );
    }
};

CASE_OVERLOAD( std::regex, RegexMatcher );

std::regex operator "" _r( const char* rgx, size_t )
{
  return std::regex( rgx );
}

int main( /* EXAMPLE NAME: CUSTOM EXTENTION( FOR REGEX ) */ )
{
    using namespace eswitch_v4;

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
