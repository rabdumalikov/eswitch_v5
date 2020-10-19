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

## _String as a param_
``` cpp
    ...
    std::string value{ "gt" };
    
    eswitch( value )
    (
        Case( "amp" )  { Print('&'); },
        Case( "lt" )   { Print('<'); },
        Case( "gt" )   { Print('>'); },
        Case( "quot" ) { Print('\"'); },
        Case( "apos" ) { Print('\''); },
        Default        { Print('?'); }
    );
```
-  #### Output:
    ```
    >
	```

## _Case Ranges_
``` cpp
    ...
    int value = 15;
    
    eswitch( value )
    (
        Case( _1.in( 1, 10 ) )  { Print("Value in a range[1,10]"); },
        Case( _1.in( 11, 20 ) ) { Print("Value in a range[11,20]"); },
        Default                 { Print('?'); }
    );
```
-  #### Output:
    ```
    Value in a range[11,20]
	```

## _Comparison_
``` cpp
    ...
    int value = 15;
    
    eswitch( value )
    (
        Case( _1 >  0  && _1 <  16 ) { Print("Value in a range(0,16)"); },
        Case( _1 >= 16 && _1 <= 20 ) { Print("Value between range[16,20]"); },
        Default                      { Print('?'); }
    );
```
-  #### Output:
    ```
    Value in a range(0,16)
	```
    
    
## _implicit break_
``` cpp
    ...
    enum Place { washington, california, ... };
    Place place = washington;

    eswitch( place )
    (
        Case( washington ) { Print('w'); },
        Case( california ) { Print('c'); },
        Default            { Print('?'); }
    );
```
-  #### Output:
    ```
    w
    ```
    
## _explicit fallthrough_
``` cpp
    ...
    enum Place { washington, california, ... };
    Place place = washington;

    eswitch( place )
    (
        Case( washington ) { Print('w'); } ^ fallthrough_,
        Case( california ) { Print('c'); },
        Default            { Print('?'); }
    );
```
-  #### Output:
```
    w
    c
```

## _std::any_
``` cpp
    ...
    std::any any = std::string{ "Hello" };
    
    eswitch( any )
    (
        Case( is< std::string >{} )( const std::string & str ) {
            Print("Str=%s", str ); 
        },
        Case( is< int >{} ) { 
            Print("int"); 
        },
        Default { 
            Print("Some other type"); 
        }
    );
```
-  #### Output:
```
    Str=Hello
```

## _std::variant_
``` cpp
    ...
    std::variant< int, double, char > var = 5.5;
    
    eswitch( var )
    (
        Case( is<double>{} )( double d ) {
            Print( "Double=%f", d ); 
        },
        Case( is<char>{} )( char c ) { 
            Print( "Char=%d", c ); 
        },
        Case( is<int>{} )( int i ) { 
            Print( "Integer=%d", i ); 
        },
        Default { 
            Print( "Some other type" );
        }
    );
```
-  #### Output:
```
    Dbl=5.5
```

## _std::pair_
``` cpp
    ...
    std::pair pr{ 10, std::string{"Hi!"} };
    
    auto result = eswitch( pr )
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
    Print("Result=%s", result ? "true" : "false");
```
-  #### Output:
```
    Result=true
```

## _std::tuple_
``` cpp
    ...
    std::tuple tup{ 1, 1, 1, 1 };
    
    auto result = eswitch( tup )
    (
        Case( 1, 0, 0, 0 ) { return 8; },
        Case( 1, 0, 0, 1 ) { return 9; },
        Case( 1, 0, 1, 1 ) { return 11; },
        Case( 1, 1, 1, 1 ) { return 15; },
    );
    Print("Result=%d", result);
```
-  #### Output:
```
    Result=15
```

## _stringify enum_
``` cpp
    ...
    enum Place { washington, california, new_york, ... };
    Place place = new_york;

    const char * enum_to_str = 
        eswitch( place )
        (
            Case( washington ) { return "washington"; },
            Case( california ) { return "california"; },
            Case( new_york )   { return "new_york"; },
            Default            { return "???"; }
        );

    printf( "State=%s" "\n", enum_to_str );
```
-  #### Output:
```
    State=new_york
```

## _multiple conditions and params_
``` cpp
    ...
    enum payload_type { XML, JSON, ... };
    ...
    payload_type payload = ...;
    XmlJsonParser * parser = ...;

    eswitch( payload, parser )
    (
        Case( _2 != nullptr && _1 == XML ) { 
            auto result = parser->parse_xml( ... );
            ...
        },
        Case( _2 != nullptr && _1 == JSON ) {
            auto result = parser->parse_json( ... );
            ...
        },
        Case( _2 == nullptr ) { 
            PrintError(...); 
        },
        Default { 
            unreachable(); 
        }
    );
```

## _Params match by predicate_
``` cpp
    int isalpha( int ){...}
    int isdigit( int ){...}
    ...
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
    ...
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
        ...
    }
    ...
    Print( "ContentLength=%s, ContentType=%s", 
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

- **clang++-11**  (or later)
- **g++-6.3.0** (or later)
- **Visual Studio** **2019**.

_______________
## TODO:
implement support for _bitwise_ operators( **\&, \|, \^, \~** )
_______________


## How to build:

**release:** *./build.sh < compiler_name >* // where compiler_name=**clang\+\+ | g\+\+ | ...**, but *not* **clang | gcc**.

**debug:**  *./build_dev.sh < compiler_name >*
_______________
## License:
Boost Software License( Version 1.0 )
_______________

