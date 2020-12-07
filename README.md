__________
## Build status
![eswitch_v5](https://github.com/rabdumalikov/eswitch_v5/workflows/eswitch_v5%20CI/badge.svg?branch=main)
__________
## Motivation:

To overcome **native switch** limitations:

- one parameter per **native switch**
- the parameter restricted to only _integral_ types( **int**, **char**, **enum** ... ).

**eswitch** supports any number of _parameters_ and almost without restriction on their _type_
 as long as the _type_ is **comparable**( i.e. has **operator==** ).
____________________________________________________
## Feature comparison:

| Feature | eswitch | native switch |
| :---: | :---: | :---: |
| *fallthrough* | yes | yes |
| *break* | yes | yes |
| *default*| yes | yes |
| *return* | yes | yes |
| _**or** cmp_ | yes| yes |
| _**and** cmp_ | yes | no |
| _(params > 1) per **switch**_ | yes | no |
| _(conditions > 1) per **case**_ | yes | no |
____________________________________________________
# Examples:


## _implicit break_
``` cpp
enum Place { washington, california, ... };

void foo( const Place place )
{
    eswitch( place )
    (
        Case( washington ) { Print('w'); },
        Case( california ) { Print('c'); },
        Default            { Print('?'); }
    );
}
```
-  #### Output:
    ```
    foo( washington ); // w
    foo( california ); // c
    foo( new_jersey ); // ?
    ```

## _explicit fallthrough_
``` cpp
void foo( const Place place )
{
    eswitch( place )
    (
        Case( washington ) { Print('w'); } ^ fallthrough_,
        Case( california ) { Print('c'); },
        Default            { Print('?'); }
    );
}
```
-  #### Output:
    ```
    foo( washington ); // wc
    foo( california ); // c
    foo( new_jersey ); // ?
    ```

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

## _Comparison_
``` cpp
void foo( const int value ) 
{
    eswitch( value )
    (
        Case( _1 >  0  && _1 <  16 ) { Print("Value inside range(0,16)"); },
        Case( _1 >= 16 && _1 <= 20 ) { Print("Value within range[16,20]"); },
        Default                      { Print('?'); }
    );
}
```
-  #### Output:
    ```
    foo( 15 ); // Value inside range(0,16)
    foo( 16 ); // Value within range[16,20]
	```
    
## _Comparison made easy_
``` cpp
void foo( const int value ) 
{
    eswitch( value ) 
    (
        Case( _1.in( 0, 16 ) )      { Print("Value inside range(0,16)"); },
        Case( _1.within( 16, 20 ) ) { Print("Value within range[16,20]"); },
        Default                     { Print('?'); }
    );
}
```
-  #### Output:
    ```
    foo( 15 ); // Value inside range(0,16)
    foo( 16 ); // Value within range[16,20]
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
            Print("Some other type"); 
        }
    );
}
```
-  #### Output:
    ```
    foo( std::string{"Hello"} ); // Str=Hello
    foo( 15 ); // Integer
    foo( true ); // Some other type
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
            Print( "Char=%c", ch ); 
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
    foo( 'c' ); // Char=c
    foo( 223 ); // Integer=223
    foo( {} );  // Varient is empty
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
        Case( 10, _2 != "Hi!" ) { 
            return false; 
        },
        Case( _1 != 10, "Hi!" ) { 
            return false; 
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
    auto result3 = foo( std::make_pair( 11, "Hi!"_s );    // result3 == false
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

## _Params match by predicate_
``` cpp
int isalpha( int ){...}
int isdigit( int ){...}

int amountAlphas = 0;
int amountDigits = 0;
int amountOthers = 0;

for( const char ch : "Nimbus 2000!"_s ) 
{    
    eswitch( ch )
    (
        Case( ( isalpha, _1 ) ) { ++amountAlphas; },
        Case( ( isdigit, _1 ) ) { ++amountDigits; },
        Default                 { ++amountOthers; }
    );
}

printf( "amountAlphas=%d, amountDigits=%d, amountOthers=%d", 
    amountAlphas, amountDigits, amountOthers );
```
-  #### Output:
    ```
    amountAlphas=6, amountDigits=4, amountOthers=2
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
- **g++\-9.3.0** (or later)
- **Visual Studio** **2019**.

## How to build:

**release:** *./build.sh < compiler_name >* // where compiler_name=**clang\+\+ | g\+\+ | ...**, but *not* **clang | gcc**.

**debug:**  *./build_dev.sh < compiler_name >*
_______________
## License:
Boost Software License( Version 1.0 )
_______________

