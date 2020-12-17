User Manual       {#mainpage}
===========

\tableofcontents

\section tutorial-preface Preface

--------------------------------------------
**Eswitch** it is header-only library which was written on c++20. This code it is my attempt to _enhance_ **switch statement** capability with similar syntax.

\subsection tutorial-installation Installation

--------------------------------------------
Since this library is header-only and on top of that whole library was implemented within single file,
thus you can get that file from [eswitch_v5 repository](https://github.com/rabdumalikov/eswitch_v5/tree/main/include/eswitch) on github. And in order to compile with eswitch_v5, just `#include <eswitch_v5.hpp>`.

\subsection tutorial-license License

--------------------------------------------
This code is distributed under the Boost Software License, Version 1.0. (See
accompanying file [LICENSE.txt](https://github.com/rabdumalikov/eswitch_v5/blob/main/LICENSE.txt) or copy at http://www.boost.org/LICENSE_1_0.txt)


\subsection tutorial-compilers Supported Compilers

--------------------------------------------------------------------------------
Should work on all major compilers which support C++20. I personally tested on following:

- clang++-11 (or later)
- g++-10.2 (or later)
- Visual Studio 2019 - isn't supported for now, just because not all the features from c++20 were implemented by Microsoft compiler.

\section tutorial-quick-start Quick Start

--------------------------------------------------------------------------------
**eswitch_v5** is a library which emulate _C++_ functionality i.e. **switch statement** and through this emulation I was able to overcome  **native switch** limitations:

- one parameter per **native switch**
- the parameter restricted to only _integral_ types( **int**, **char**, **enum** ... ).

**eswitch_v5** supports any number of _parameters_ and almost without restriction on their _type_
 as long as the _type_ is **comparable**( i.e. has **operator==**, **operator!=** and so on ).
 Additionally, my library allow to match for **std::any, std::variant<...>, polymorphic type match** and
 **std::regex**. Also it allows to withdraw values from those types, of course if match was successful.

### Motivation

I don't see any good reason why **switch statement**  in **C++** is so limited, whereas other 
statements such as **if, while, for** have no such limitations and they allow to compose and
test complex condition. Moreover in terms of _**assembler**_ output, **if** and **switch** 
statements give the same output, thus the performance also the same. If there isn't any
benefits to keep **switch statement** limited, so why the things the way they are? I also
don't think that **switch statement** is limited because of compatability with **C**, because
syntax of **if statement** is still compatible with **C** even though it was extended in **C++17**, this extention allows to declare variable 
within **if statement** like this: 
```cpp
if( std::smatch mt; std::regex( text, mt, rgx ) ) {...}
```
On top of that the internet is full of questions:
- [Why strings cannot be used in **switch statement**](https://stackoverflow.com/questions/650162/why-the-switch-statement-cannot-be-applied-on-strings),
- [How to compose complex condition in **switch statement**](https://stackoverflow.com/questions/68578/multiple-cases-in-switch-statement)
- etc.
 
And there was even the [proposal](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3627.html) for **C++ standard committee** to allow to use _complex types_( like **string**, **complex numbers**, etc ) within **switch statement** and **committee** <span style="color:green"> *agreed* </span> about **importance of this topic**. 
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

### Conventions used in this document:

In all code examples, I omit the namespace prefixes for names in the **eswitch_v5** and **std** namespaces. Also I'm going to use following name conventions:
- **native switch** => _native C++_ **switch statement**
- **eswitch** => my custom implementation of **enhanced switch statement**

### Similarities

As I have mentioned that I've tried within my implementation to resemble _C++_ **switch statement** syntax. And I think I was able to achieve this, except the places where I was either limited by language
or intentionally tried to avoid certain behavior of **switch statement** in _C++_ like default **fallthrough**. Compare:  

```cpp
switch( num )
{
    case 1:  {...} break;
    case 2:  {...} break;
    default: {...} break;
};
```

with

```cpp
eswitch( num )
(
    Case( 1 ) {...},
    Case( 2 ) {...},
    Default   {...}
);
```
**Note That:** _Native_ **switch statement** in _C++_ has _explicit_ **break** and _implicit_ **fallthrough** 
which proven to be error-prone approach. Since developers sometimes forget to use 
**break** and because of this their code doesn't work the way it was intended. 
Thus in my implementation I reversed this concept i.e. **eswitch** has _implicit_ **break** 
and _explicit_ **fallthrough**. Compare:

```cpp
switch( num )                    
{                                
    case 1:  {...}           
    case 2:  {...} break;        
    default: {...} break;        
};                               
```

with 

```cpp
eswitch( num )
(
	Case( 1 ){...} ^ fallthrough_,
	Case( 2 ){...},
	Default  {...}
);
```

### Keywords

| Name | Description |
| :---: | :---: |
| ***eswitch*** | _accepts list of input parameters_ |
| ***Case*** | _accepts **condition** to check and body next to it will be executed if **condition** matched_ |
| ***Default*** | _body next to it will be executed if nothing else matched_ |
| ***fallthrough_*** | _next body will be executed without checking its **condition**_ |
| ***any_from*** | _accepts values to choose from_ |
| ***is<sometype>*** | _used within **Case** for matching types like **std::any**, **std::variant<...>** and **polymorphic type match**_ |
| ***_r*** | _user defined literal for **std::regex**_ |



### Syntax

``` cpp
eswitch( __params__ )
( 
    Case( __conditions__ )( __value__ ) { ... } ^ __options__,
    Default { ... }
);
```

Where

| Name | Details |
| :---: | :---: |
| <span style="color:blue">__params__</span> | list of parameters( i.e. param1, param2, ..., param_n ) |
| <span style="color:blue">__conditions__</span> | match **in order**( i.e. \_1 == smth1 \|\| \_2 == smth2 \|\| ... )<br>or match via **any_from**( i.e. \_1 == **any_from**( smth1, smth1, ... )<br>or match via **predicate**( i.e. ( pred1, \_1 ) && ( pred2, \_2 ) && ... ) |
| _**Optional:**_<br><span style="color:blue">__value__</span> | withdrawn values from: **std::any**, **std::variant<...>**,<br> **polymorphic match** or **std::regex match**. |                
| _**Optional:**_<br><span style="color:blue">__options__</span> | [**left empty** => _break_] or <br> [**fallthrough_** => _execute body of the following case_] or <br> [**likely_** which will be introduced in future] |

### Features

- **implicit break** - you don't have to specify it 

- **fallthrough** - is *explicit*

- **composing condition** with: **&&** , **\|\|** , **==** , **!=** , **>** , **<** , **<=** , **>=** 
- **param matching via indexing and without**
- **comparison made easy**
- **match for**: _std::any, std::variant, std::regex_
- **match and withdraw value from**: _std::any, std::variant, std::regex_
 
#### Params referencing via indexes

```cpp
eswitch( p1, p2 )
(
    Case( _1 == true && _2 == true ) {...},
    Case( _2 != true || _1 == true ) {...}
);   
// i.e.
     if( p1 == true && p2 == true ) {...}
else if( p1 == true || p2 == true ) {...}
```


#### If indexes aren't provided, then it is assummed that they're sequential

```cpp
eswitch( p1, p2 )
(
    Case( true, false ) {...}, // same as "Case( _1 == true && _2 == false )" 
    Case( false ){...}
);
```

#### Matching via predicates:(it could be either a lambda or a free function)

```cpp
eswitch( p1, p2 )
(
    Case( ( is_odd, _1 ) && ( is_odd, _2 ) ) {...},
    Case( ( is_negative( _1, _2 ) ){...}
);
    
// i.e.
     if( is_odd( p1 ) && is_odd( p2) ) {...}
else if( is_negative( p1, p2 ) ) {...}
```

#### Check in range via predicate

```cpp
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
     if( p1 >  1  && p1 < 10 )  {...}
else if( p1 >= 11 && p1 <= 20 ) {...}
```

#### Any_from helper function

```cpp
eswitch( file_extention )
(
    Case( any_from( "cpp", "cc", "c++", "cxx", "C" ) ) {...},
    Case( any_from( "h", "hpp", "hh", "h++", "hxx", "H" ) ) {...},
    Default {...}
);
```

#### Default case

```cpp
eswitch( p1 )
(
    Case( false ) {...},
    Default {...}
);
```

#### Return value from Case

```cpp
bool val = eswitch( p1 ) 
(
    Case( 1 ) { return true; }
);
```

#### Match for std::any or std::variant

```cpp
eswitch( any_ ) 
(
    Case( is< int >{} ) {...}, // will be executed in 'std::any' contain 'int'
    Case( is< string >{} ) {...} // will be executed in 'std::any' contain 'std::string'
);
```

#### Match and withdraw value from std::any or std::variant

```cpp
eswitch( any ) 
(
    Case( is< int >{} )( const int value ) {...}, // Note that keyword 'auto' isn't allowed( i.e. code won't compile )
    Case( is< string >{} )( const string & value ) {...}
);
```

#### Match for polymorphic types

```cpp
eswitch( base_ptr ) 
(
    Case( is< circle >{} )( circle * c ) { c->draw(); },
    Case( is< square >{} )( square * s ) { s->draw(); }
);
// OR
eswitch( base_ref ) 
(
    Case( is< circle >{} )( circle & c ) { c.draw(); },
    Case( is< square >{} )( square & s ) { s.draw(); }
);
```

#### Match for std::regex

```cpp
eswitch( text ) 
(
    Case( R"(\w*)"_r ) { return "message"; },
    Case( R"(\d*)"_r ) { return "number"; }
);
```

#### Match and withdraw values from std::regex

```cpp
eswitch( text ) 
(
    /// NOTE that "vector<string>"" and "std::smatch" contain same values
    Case( R"((\w*))"_r )( vector< string > && match ){ return match[1]; } 
    Case( R"((\d*))"_r )( vector< string > && match ){ return match[1]; } 
);
```
