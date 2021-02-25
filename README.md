## Overview:

`eswitch_v5` is a library, which is an improved version of <span class="keywordflow">`switch`</span> statement in C++. The main idea behind this library is to be able to overcome <span class="keywordflow">`switch`</span> statement limitations, such as:
- one argument per <span class="keywordflow">`switch`</span> statement
- the argument restricted to only _integral_ types( <span class="keyword">`int`</span>, <span class="keyword">`char`</span>, <span class="keyword">`enum`</span> ... )
- impossibility to compose complex conditions, as opposed to other _statements_ and _loops_ in C++

`eswitch_v5` supports any number of arguments and almost without restriction on their type as long as the type is comparable( i.e. has `operator==`, `operator!=` and so on ). Additionally, my library went beyond overcoming limitations of <span class="keywordflow">`switch`</span> statement e.g. I introduced more expressive way for matching and withdrawing values from `std::any`, `std::variant`, `polymorphic types` and
`std::regex`.

## Build status:

[![eswitch_v5](https://github.com/rabdumalikov/eswitch_v5/workflows/gcc%20CI/badge.svg?branch=main)](https://github.com/rabdumalikov/eswitch_v5/actions)
[![eswitch_v5](https://github.com/rabdumalikov/eswitch_v5/workflows/gcc%20TESTS/badge.svg?branch=main)](https://github.com/rabdumalikov/eswitch_v5/actions)
[![eswitch_v5](https://github.com/rabdumalikov/eswitch_v5/workflows/Docs%20generation/badge.svg?branch=main)](https://github.com/rabdumalikov/eswitch_v5/actions)

[![eswitch_v5](https://github.com/rabdumalikov/eswitch_v5/workflows/clang%20CI/badge.svg?branch=main)](https://github.com/rabdumalikov/eswitch_v5/actions)
[![eswitch_v5](https://github.com/rabdumalikov/eswitch_v5/workflows/clang%20TESTS/badge.svg?branch=main)](https://github.com/rabdumalikov/eswitch_v5/actions)
[![eswitch_v5](https://github.com/rabdumalikov/eswitch_v5/workflows/clang%20Examples/badge.svg?branch=main)](https://github.com/rabdumalikov/eswitch_v5/actions)


## Documentation:

https://rabdumalikov.github.io/eswitch_v5

# Examples:

## _String as a param_
``` cpp
void foo( std::string_view str )
{
    eswitch( str )
    (
        Case( "amp" )  { Print('&'); },
        Case( "lt" )   { Print('<'); },
        Case( "gt" )   { Print('>'); },
        Case( "quot" ) { Print('\"'); },
        Case( "apos" ) { Print('\''); },
        Default        { Print('?'); }
    );
}
```
-  #### Output:
    ```
    foo( "amp" ); // &
    foo( "gt" );  // >
	```

## _stringify enum_
``` cpp
std::string foo( Place place )
{
    return eswitch( place )
    (
        Case( washington ) { return "washington"; },
        Case( california ) { return "california"; },
        Case( new_york )   { return "new_york"; },
        Default            { return "???"; }
    );
}
```
-  #### Output:
    ```
    auto result1 = foo( washington ); // result1 == "washington"
    auto result2 = foo( new_york );   // result2 == "new_york"
    ```

## _std::any_
``` cpp
void foo( const std::any a )
{
    eswitch( any )
    (
        Case( is< std::string >{} )( const std::string & str ) {
            Print("Str=%s", str ); 
        },
        Case( is< int >{} ) { 
            Print("Integer"); 
        },
        Default { 
            Print("Something else"); 
        }
    );
}
```
-  #### Output:
    ```
    foo( std::string{"Hello"} ); // Str=Hello
    foo( 15 ); // Integer
    foo( true ); // Something else
    ```

## _std::variant_
``` cpp
void foo( const std::variant< int, double, char > & var )
{
    eswitch( var )
    (
        Case( is<double>{} )( double d ) {
            Print( "Double=%f", d ); 
        },
        Case( is<char>{} )( char ch ) { 
            Print( "Character=%c", ch ); 
        },
        Case( is<int>{} )( int i ) { 
            Print( "Integer=%d", i ); 
        },
        Default { 
            Print( "Variant is empty" );
        }
    );
}
```
-  #### Output:
    ```
    foo( 5.5 ); // Dbl=5.5
    foo( 'c' ); // Character=c
    foo( 223 ); // Integer=223
    foo( {} );  // Varient is empty
    ```

## _polymorphic match_
``` cpp
struct base {...};
struct circle : base {...};
struct square : base {...};
struct rectangle : base {...};

void foo( base * b )
{
    eswitch( b )
    (
        Case( is< circle >{} )( circle * c ) {
            Print("Name=%s", c->name() ); 
        },
        Case( is< square >{} )( square * s ) { 
            Print("Name=%s", s->name() ); 
        },
        Default { 
            Print("Name=Unknown" ); 
        }
    );
}
```
-  #### Output:
    ```
    foo( new circle{} );    // Name=Circle
    foo( new square{} );    // Name=Square
    foo( new rectangle{} ); // Name=Unknown
    foo( nullptr );         // Name=Unknown
    ```

## _std::pair_
``` cpp
auto foo( std::pair< int, std::string > pr )
{    
    return eswitch( pr )
    (
        Case( 10, "Hi!" ) { 
            return true; 
        },
        Default { 
            return false; 
        }
    );
}
```
-  #### Output:
    ```
    auto result1 = foo( std::make_pair( 10, "Hi!"_s );    // result1 == true
    auto result2 = foo( std::make_pair( 10, "Hello!"_s ); // result2 == false
    ```

## _std::tuple_
``` cpp
auto foo( std::tuple< int, int, int, int > tup )
{
    return eswitch( tup )
    (
        Case( 1, 0, 0, 0 ) { return 8; },
        Case( 1, 0, 0, 1 ) { return 9; },
        Case( 1, 0, 1, 1 ) { return 11; },
        Case( 1, 1, 1, 1 ) { return 15; },
    );
}
```
-  #### Output:
    ```
    foo( std::make_tuple( 1, 0, 0, 0 ) ); // Result=8
    foo( std::make_tuple( 1, 1, 1, 1 ) ); // Result=15
    ```

## _Regex cases_
``` cpp
string response =
    "HTTP/1.1 200 OK" "\r\n"
    "Content-Lenght: 88" "\r\n"
    "Content-Type: text/html" "\r\n";

map< string, string > fields;

for( const auto & line : tokenize( response, "\r\n" ) )
{       
    eswitch( line )
    (
        /// rgxmatch has same values as std::smatch
        Case( "^(.+): (.+)$"_r )( std::vector< std::string > && rgxmatch ) {
            fields[ std::move( rgxmatch[ 1 ] ) ] = std::move( rgxmatch[ 2 ] );
        }
    );
}

printf( "ContentLength=%s, ContentType=%s", 
    fields[ "Content-Lenght"_s ].c_str(),
    fields[ "Content-Type"_s ].c_str() );
```
-  #### Output:
    ```
    ContentLength=88, ContentType=text/html
    ```
_______________

## Minimum C++ standard:
C++20
_______________

## Supported Compilers:

Should work on all major compilers which support **C++20**.
I personally tested on following:

- **clang++\-11**  (or later)
- **g++\-10.2.0** (or later)
- **Visual Studio** **2019**.

## How to build:

**release:** *./build.sh < compiler_name >* // where compiler_name=**clang\+\+ | g\+\+ | ...**, but *not* **clang | gcc**.

**debug:**  *./build_dev.sh < compiler_name >*
_______________
## License:
Boost Software License( Version 1.0 )
_______________

