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

std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}
 
std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}
 
std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    return ltrim(rtrim(str, chars), chars);
}

std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(trim(token));
   }
   return tokens;
}

int main( /* EXAMPLE NAME: CUSTOM EXTENTION( FOR REGEX ) */ )
{
    using namespace eswitch_v4;

    std::map< std::string, std::string > fields;

    for( const auto & line : get_tokenized_http_response() )
    {       
        const bool to_continue = 
            eswitch( line ) >>
            case_( "^.+ 200 .+$"_r ) >> to_return( true ) >>
            case_( "^.+: .+$"_r )    >> [&]{ 
                auto splitted = split( line, ':' );                     
                fields[ splitted[ 0 ] ] = splitted[ 1 ];
                return true; 
            } >>
            default_ >> to_return( false ) >>
            in_place_return_;

        if( !to_continue ) break;
    }

    assert( fields.size() == 2 );
    assert( fields[ std::string{"Content-Lenght"} ] == std::string{"88"} );
    assert( fields[ std::string{"Content-Type"} ] == std::string{"text/html"} );
}
