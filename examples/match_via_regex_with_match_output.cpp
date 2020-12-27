//  Copyright (c) 2019-present Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#undef NDEBUG
///[match_via_regex_with_match_output]
#include <cassert>
#include <map>
#include <eswitch_v5.hpp>

std::vector< std::string > get_tokenized_http_response()
{
    std::vector< std::string > v = 
    {
        { "HTTP/1.1 200 OK" },
        { "Content-Lenght: 88" },
        { "Content-Type: text/html" }
    };

    return v;
}

int main()
{
    using namespace eswitch_v5;
    using namespace std::string_literals;

    std::map< std::string, std::string > fields;

    for( const auto & line : get_tokenized_http_response() )
    {       
        const bool to_continue = 
            eswitch( line )
            (
                Case( "^.+ 200 .+$"_r ) { return true; },
                Case( "^(.+): (.+)$"_r )( const std::vector< std::string > & match ){ 
                    fields[ match[ 1 ] ] = match[ 2 ];
                    return true; 
                },
                Default { return false; } 
            );

        if( !to_continue ) break;
    }

    assert( fields.size() == 2 );
    assert( fields[ "Content-Lenght"s ] == "88"s );
    assert( fields[ "Content-Type"s ] == "text/html"s );
}
///[match_via_regex_with_match_output]
