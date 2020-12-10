## In a nutshell:
This is my attempt to enhance **switch statement** capability with resembling his syntax.

**For example:**
```cpp
    /* C++ Native switch */      |         /* eswitch */
=================================================================
switch( num )                    |        eswitch( num )
{                                |        (
    case 1:  {...} break;        VS           Case( 1 ){...},
    case 2:  {...} break;        |            Case( 2 ){...},
    Default: {...} break;        |            Default  {...}
};                               |        );
=================================================================
```
_Native_ **switch statement** in _C++_ has _explicit_ **break** and _implicit_ **fallthrough** 
which proven to be error-prone approach. Since developers sometimes forget to use 
**break** and because of this their code doesn't work the way it was intended. 
Thus in my implementation I reversed this concept i.e. **eswitch** has _implicit_ **break** 
and _explicit_ **fallthrough**. 

**For example:**
```cpp
    /* C++ Native switch */      |         /* eswitch */
=================================================================
switch( num )                    |        eswitch( num )
{                                |        (
    case 1:  {...}               VS           Case( 1 ){...} ^ fallthrough_
    case 2:  {...} break;        |            Case( 2 ){...},
    Default: {...} break;        |            Default  {...}
};                               |        );
=================================================================
```

## Motivation:
I don't see any good reason why **switch statement**  in **C++** is so limited, whereas other 
statements such as **if, while, for** have no such limitations and they allow to compose and
test complex condition. Moreover in terms of **_assembler_** output, **if** and **switch** 
statements give the same output, thus the performance also the same. If there isn't any
benefits to keep **switch statement** limited, so the things the way they are? I also
don't think that **switch statement** is limited because of compatability with **C**, because
syntax of **if statement** was extended in **C++17**, this extention allows to declare variable 
within **if statement** like this: 
```cpp
if( std::smatch mt; std::regex( text, mt, rgx ) ) {...}
```
On top of that the internet is full of questions:
- [Why strings cannot be used in **switch statement**](https://stackoverflow.com/questions/650162/why-the-switch-statement-cannot-be-applied-on-strings),
- [How to compose complex condition in **switch statement**](https://stackoverflow.com/questions/68578/multiple-cases-in-switch-statement)
- etc.
 
And there was even the [proposal](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3627.html) for **C++ standard committee** to allow to use _complex types_( like **string**, **complex numbers**, etc ) within **switch statement** and **committee** agreed about importance of this topic. 
But somehow the author stopped his work toward this direction.

There is more, compilers such as **clang** and **gcc** have non-standard extention for matching [**case ranges**](https://gcc.gnu.org/onlinedocs/gcc/Case-Ranges.html).
```cpp
switch( num )                              switch( ch )
{                                          {
    case 1 ... 5: break;                       case 'A' ... 'Z': break;          
    case 10 ... 99: break;                     case 'a' ... 'z': break;      
};                                         };
```
The evidence above tell us that people don't like limitations of **switch statement**, and they're trying to overcome 
them with different approaches( by implementing **non-standard extension** or writing  **proposals** or just **wondering** ).

For modern language like **swift** it is not an issue, because they have pretty advanced **switch statement**.

Based on those factors I decided to write my own implementation of **enhanced switch** or just **eswitch**.
In my implementation I tried to aim all the **limitations** and leave the syntax of **eswitch** as close as 
possible to _native C++_ **switch statement**. And last but not least, another my priority was the performance 
of **eswitch**, which shouldn't differ by much from **native switch**. 

## Conventions used in this document:
In all code examples, I omit the namespace prefixes for names in the **eswitch_v5** and **std** namespaces. 
Along with following name conventions:
- **native switch** => _native C++_ **switch statement**
- **eswitch** => my custom implementation of **enhanced switch statement**

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
    Default { ... } ;
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
 - **match and withdraw value from**: _std::any, std::variant, std::regex_
 
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
            Case( false ){...}
        );
        ```

    - **matching via predicates**: it could be either _lambda_ or _free_function_:
        ``` cpp
        eswitch( p1, p2 )
        (
            Case( ( is_odd, _1 ) && ( is_odd, _2 ) ) {...},
            Case( ( is_negative( _1, _2 ) ){...}
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
        eswitch( any_ ) 
        (
            Case( is< int >{} ) {...}, // will be executed in 'std::any' contain 'int'
            Case( is< string >{} ) {...} // will be executed in 'std::any' contain 'std::string'
        );
        ```
    - **match and withdraw value from _std::any_ or _std::variant_**:
        ``` cpp
        eswitch( any ) 
        (
            Case( is< int >{} )( const int value ) {...}, // Note that keyword 'auto' isn't allowed( i.e. code won't compile )
            Case( is< string >{} )( const string & value ) {...}
        );
        ```
    - **match for _std::regex_**:
        ``` cpp
        eswitch( text ) 
        (
            Case( R"(\w*)"_r ) { return "message"; },
            Case( R"(\d*)"_r ) { return "number"; }
        );
        ```
    - **match and withdraw values from _std::regex_**:
        ``` cpp
        eswitch( text ) 
        (
            /// NOTE that "vector<string>"" and "std::smatch" contain same values
            Case( R"((\w*))"_r )( const vector< string > & match ){ return match[1]; } 
            Case( R"((\d*))"_r )( const vector< string > & match ){ return match[1]; } 
        );
        ```