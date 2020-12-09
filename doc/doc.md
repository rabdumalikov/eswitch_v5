__________
## Keywords:
| Name | Description |
| :---: | :---: |
| ***eswitch*** | *accepts list of input parameters* |
| ***Case*** | *accepts condition to check* |
| ***Default*** | *body next to it will be executed if nothing else matched* |
| ***fallthrough_*** | *next body will be executed without condition checking* |
| ***any_from*** | *accepts values to choose from* |

## Syntax:

    ``` cpp
    eswitch( __params__ )
    ( 
        Case( __condition__ ) { ... } ^ __falling_options__,
        Default { ... }
    );
    // __param__            - list of parameters( i.e. param1, param2, ..., param_n )
    // __condition__        - it could be specific order match( i.e. _1 == smth1 || _2 == smth2 || ... )
    //			  or it could be match via any_from( i.e. _1 == any_from( smth1, smth1, ... ) )
    //		          or it could be a predicate match( i.e. ( pred1, _1 ) && ( pred2, _2 ) && ... )
    // __falling_options__  - [ left empty => break ] or [ fallthrough_ => execute body of the following case]
    ```

## Features:

- **implicit break** - you don't have to specify it 

- **fallthrough** - is *explicit*

- **composing condition** with: **&&** , **||** , **==** , **!=** , **>** , **<** , **<=** , **>=** 
 - **param matching via indexing and without**
 - **comparison made easy**
 - **match for**: _std::any, std::variant, std::regex_
 - **match and withdraw value from**: _std::any_, _std::variant_ and _std::regex_
 

 ## Details:


- **referencing to params in _eswitch_**:
    ``` cpp
    eswitch( p1, p2 )
    (
        Case( _1 == true && _2 == true ) {...},
        Case( _2 != true || _1 == true ) {...}
    );   
    // i.e.
            if( p1 == true && p2 == true ) {...}
    else if( p1 == true || p2 == true ) {...}
    ```

- **if indexes aren't provided, then it is assummed that they're sequential**:
    ``` cpp
    eswitch( p1, p2 )
    (
        Case( true, false ) {...}, // same as "Case( _1 == true && _2 == false )" 
        Case( false ) {...}
    );
    ```
- **explicit fallthrough**:
    ``` cpp
    eswitch( p1, p2 )
    (
        Case( 25 ) {...} ^ fallthrough_,
        Case( 15 ) {...} // If 1st "Case" will be matched, then body of 2nd "Case" will be executed as well
    );
    ```

- **matching via predicates**: it could be either _lambda_ or _free_function_:
    ``` cpp
    eswitch( p1, p2 )
    (
        Case( ( is_odd, _1 ) && ( is_odd, _2 ) ) {...},
        Case( ( is_negative( _1, _2 ) ) {...}
    );
        
    // i.e.
            if( is_odd( p1 ) && is_odd( p2) ) {...}
    else if( is_negative( p1, p2 ) ) {...}
    ```
- **check in range via predicate**:
    ``` cpp
    eswitch( p1 )
    (
        Case( _1.in( 1, 10 ) ) {...},
        Case( _1.within( 11, 20 ) ) {...}
    );
    // OR
    eswitch( p1 )
    (
        Case( _1 > 1 && _1 < 10 ) {...},
        Case( _1 >= 11 && _1 <= 20 ) ) {...}
    );

    // i.e.
            if( p1 >  1  && p1 < 10 ) {...}
        else if( p1 >= 11 && p1 <= 20 ) {...}
    ```

- **any_from helper function**:
    ``` cpp
    eswitch( file_extention )
    (
        Case( any_from( "cpp", "cc", "c++", "cxx", "C" ) ) {...},
        Case( any_from( "h", "hpp", "hh", "h++", "hxx", "H" ) ) {...},
        Default {...}
    );
    ```

- **default case**:
    ``` cpp
    eswitch( p1 )
    (
        Case( false ) {...},
        Default {...}
    );
    ```
- **return value from _Case_**:
    ``` cpp
    bool val = eswitch( p1 ) 
    (
        Case( 1 ) { return true; }
    );
    ```
- **match for _std::any_ or _std::variant_**:
    ``` cpp
    eswitch( any ) 
    (
        Case( is< int >{} ) {...}, // will be executed in "std::any/std::variant" contain "int"
        Case( is< std::string >{} ) {...} // will be executed in "std::any/std::variant" contain "std::string"
    );
    ```
- **match and withdraw value from _std::any_ or _std::variant_**:
    ``` cpp
    eswitch( any ) 
    (
        /// NOTE THAT: keyword "auto"( in 2nd parentheses ) isn't allowed( i.e. code won't compile )
        Case( is< int >{} )( const int value ) {...},
        Case( is< std::string >{} )( const std::string & value ) {...}
    );
    ```
- **_std::regex_ match**:
    ``` cpp
    eswitch( text ) 
    (
        Case( R"(\w*)"_r ) { return "message"; },
        Case( R"(\d*)"_r ) { return "number"; }
    );
    ```
- **match and withdraw value/values from _std::regex_**:
    ``` cpp
    eswitch( text ) 
    (
        /// NOTE THAT: "vector<string>"" and "std::smatch" are identical in terms of values
        Case( R"((\w*))"_r )( const vector< string > & match ) { return match[1]; } 
        Case( R"((\d*))"_r )( const vector< string > & match ) { return match[1]; } 
    );
    ```