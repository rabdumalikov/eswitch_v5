__________
## Keywords:
| Name | Description |
| :---: | :---: |
| ***eswitch*** | *accepts list of input parameters* |
| ***case_*** | *accepts condition to check* |
| ***default_*** | *body next to it will be executed if nothing else matched* |
| ***fallthrough_*** | *next body will be executed without condition checking* |
| ***in\_place\_return_*** | *returns value if it exists, otherwise throws exception* |
| ***handle_return*** | *accepts lambda to be called if return value exists* |
| ***to_return*** | *accepts value to return* |
| ***any_from*** | *accepts values to choose from* |
## Syntax:

``` cpp
eswitch( __params__ ) >>
    case_( __condition__ ) >> __body__ >> __falling_options__ >>
    default_ >> __body__ >> 
    __return_options__;

// __param__            - list of parameters( i.e. param1, param2, ..., param_n )
// __condition__        - it could be specific order match( i.e. _1 == smth1 || _2 == smth2 || ... ),
//		          or it could be a predicate match( i.e. ( pred1, _1 ) && ( pred2, _2 ) && ... )
// __body__             - lambda or free_function to execute in case of match or it could be EMPTY.
// __falling_options__  - [left empty => break ] or [ fallthrough_ => execute body of the following case]
// __return_options__   - "in_place_return_" returns value in place or throws exception, 
//                        "handle_return" returns value via lambda call.

```

## Features:

- **implicit break** - you don't have to specify it 

- **fallthrough** - is *explicit*

- **composing condition** with '&&','||','==','!=' :
-
     - **index param matching**:
``` cpp
        eswitch( p1, p2 ) >> 
            case_( _1 == true && _2 == true ) >> []{...} >>
            case_( _1 == true || _2 == true ) >> []{...};
            
        // i.e.
                if( p1 == true && p2 == true ) {...}
   	   else if( p1 == true || p2 == true ) {...}
```
-
     - **index is not required if CASE contains just one param**:
``` cpp
        eswitch( p1, p2 )  >> 
            case_( true )  >> []{...} >> // same as "case_( _1 == true )" 
            case_( false ) >> []{...};
```
-
     - **predicate param matching**, it could be lambda or free_function:
``` cpp
        eswitch( p1, p2 ) >> 
            case_( ( is_odd, _1 ) && ( is_odd, _2 ) ) >> []{...} >> 
            case_( ( is_negative( _1, _2 ) ) >> []{...};
            
        // i.e.
             if( is_odd( p1 ) && is_odd( p2) ) {...}
        else if( is_negative( p1, p2 ) ) {...}
```
- **ranges**:
-
     - **check in range via predicate**:
``` cpp
        eswitch( p1, p2 ) >> 
            case_( ( in_range<  1, 10 >, _1 ) ) >> []{...} >>
            case_( ( in_range< 11, 20 >, _2 ) ) >> []{...};
            
        // i.e.
                if( p1 >= 1  || p1 <= 10 ) {...}
   	    else if( p1 >= 11 || p1 <= 20 ) {...}
```
-
     - **check in range via special index construction**:
``` cpp
        eswitch( p1, p2 ) >> 
            case_( _1.in(  1, 10 ) ) >> []{...} >>
            case_( _2.in( 11, 20 ) ) >> []{...};
            
        // i.e.
                if( p1 >= 1  || p1 <= 10 ) {...}
   	    else if( p1 >= 11 || p1 <= 20 ) {...}```
- **any_from helper function**:
``` cpp
    eswitch( file_extention ) >>
        case_( any_from( "cpp", "cc", "c++", "cxx", "C" ) ) >> [&]{...} >>
        case_( any_from( "h", "hpp", "hh", "h++", "hxx", "H" ) ) >> [&]{...} >>
        default_ >> []{...};
```
- **CASE omit body**, in case you have nothing to do on match:
``` cpp
    eswitch( p1, p2 ) >>
        case_( _1 == true && _2 == true ) >>
        case_( _1 == true || _2 == true ) >> []{...};
```
- **default case**:
``` cpp
    eswitch( p1 ) >>
        case_( false ) >> []{...} >>
        default_ >> []{...};
```
- **return value**:
-
    - in **lambda** or **free function**:
``` cpp
    eswitch( p1 ) >> case_( 1 ) >> []{ return true; };
```
``` cpp
	bool foo(){ return true; }
        eswitch( p1 ) >> case_( 1 ) >> foo;
```
-
	- via helper function **to_return**:
``` cpp
        eswitch( p1 ) >> case_( 1 ) >> to_return( true );
```
- **retrieve return value**:
-
     - use **in\_place\_return_**, if nothing to return => **THROW** exception:
``` cpp
        bool result = eswitch( p1 ) >> case_( 1 ) >> []{ return true; } >> 
            in_place_return_;
```
-
     - **handle_return**, executes if there is return value:
``` cpp
        eswitch( p1 ) >> case_( 1 ) >> []{ return true; } >> 
	    handle_return( []( const bool val ){ ... } );
```
- **customize CASE** - you could specify custom logic for some types:
-
 	- **CASE\_OVERLOAD( type\_to\_overload, accept_1st_arg_and_make_custom_cmp )**:
``` cpp
	struct double_cmp;
	CASE_OVERLOAD( double, double_cmp );
        
        struct double_cmp
        {
            double value;
            
            explicit double_cmp( const double input ) : value( input ) {}
            friend bool operator==( const double v, const double_cmp dcmp )
            {
                return properly_compare_doubles( v, dcmp.value );
            }
            friend bool operator!=( const double v, const double_cmp dcmp )
            {
                return !( v == dcmp );
            }
        };
        
        double d = 2.0001;
        
        eswitch( d ) >>
	    case_( 2.0001 ) >> []{...}
            case_( 2.0002 ) >> []{...};
```
____________________________________________________
# Examples:
## _String as a param_
``` cpp
    ...
    std::string value{ "gt" };
    
    eswitch( value ) >>
        case_( "amp" )  >> []{ Print('&'); } >>
        case_( "lt" )   >> []{ Print('<'); } >>
        case_( "gt" )   >> []{ Print('>'); } >>
        case_( "quot" ) >> []{ Print('\"'); } >>
        case_( "apos" ) >> []{ Print('\''); } >>
        default_            >> []{ Print('?'); };
```
-  #### Output:
    ```
    >
	```

## _Case Ranges_
``` cpp
    ...
    int value = 15;
    
    eswitch( value ) >>
        case_( _1.in( 1, 10 ) )  >> []{ Print("Value in a range[1,10]"); } >>
        case_( _1.in( 11, 20 ) ) >> []{ Print("Value in a range[11,20]"); } >>
        default_                 >> []{ Print('?'); };
```
-  #### Output:
    ```
    Value in a range[11,20]
	```
    
## _Case Ranges via predicate_
``` cpp
    ...
    int value = 9;
    
    eswitch( value ) >>
        case_( ( in_range< 1,  10 >, _1 ) ) >> []{ Print("Value in a range[1,10]"); } >>
        case_( ( in_range< 11, 20 >, _1 ) ) >> []{ Print("Value in a range[11,20]"); } >>
        default_                 >> []{ Print('?'); };
```
-  #### Output:
    ```
    Value in a range[1,10]
	```
    
## _implicit break_
``` cpp
    ...
    enum Place { washington, california, ... };
    Place place = washington;

    eswitch( place ) >>
        case_( washington ) >> []{ Print('w'); } >>
        case_( california ) >> []{ Print('c'); } >>
        default_            >> []{ Print('?'); };
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

    eswitch( place ) >>
        case_( washington ) >> []{ Print('w'); } >> fallthrough_ >>
        case_( california ) >> []{ Print('c'); } >>
        default_            >> []{ Print('?'); };
```
-  #### Output:
```
    w
    c
```

## _stringify enum_
``` cpp
    ...
    enum Place { washington, california, new_york, ... };
    Place place = new_york;

    const char * enum_to_str = 
        eswitch( place ) >>
            case_( washington ) >> to_return( "washington" ) >>
            case_( california ) >> to_return( "california" ) >>
            case_( new_york )   >> to_return( "new_york" ) >>
            default_            >> to_return( "???" ) >>
            in_place_return_;

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

    eswitch( payload, parser ) >>
        case_( _1 == XML && _2 != nullptr ) >> [&] { 
            auto result = parser->parse_xml( ... );
            ...
        } >>
        case_( _1 == JSON && _2 != nullptr ) >> [&] {
            auto result = parser->parse_json( ... );
            ...
        } >>
        case_( _2 == nullptr ) >> [&] { 
            PrintError(...); 
        } >>
        default_ >> [] { 
            unreachable(); 
        };
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
        eswitch( ch ) >>
            case_( ( isalpha, _1 ) ) >> [&]{ ++amountAlphas; } >>
            case_( ( isdigit, _1 ) ) >> [&]{ ++amountDigits; } >>
            default_                 >> [&]{ ++amountOthers; };
    }

    printf( "amountAlphas=%d, amountDigits=%d, amountOthers=%d", 
        amountAlphas, amountDigits, amountOthers );

```
-  #### Output:
```
    amountAlphas=6, amountDigits=4, amountOthers=2
```

## _Customized cases_
``` cpp

    regex operator "" _r( const char* rgx, size_t ){...}

    // "RegexMatcher" defined in example/example17.cpp
    CASE_OVERLOAD( regex, RegexMatcher );
    ...
    string response =
        "HTTP/1.1 200 OK" "\r\n"
        "Content-Lenght: 88" "\r\n"
        "Content-Type: text/html" "\r\n";

    map< string, string > fields;

    for( const auto & line : tokenize( response, "\r\n" ) )
    {       
        eswitch( line ) >>
            case_( "^.+ 200 .+$"_r ) >>     // match for "HTTP/1.1 200 OK"
            case_( "^.+: .+$"_r )    >> [&] // match for "key: value"
            { 
                auto splitted = split( line, ':' );                     
                fields[ splitted[ 0 ] ] = splitted[ 1 ];
            } >>
            default_ >> []{ terminate(); };
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



