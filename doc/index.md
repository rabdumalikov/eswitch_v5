User Manual       {#mainpage}
===========

\tableofcontents

\section tutorial-overview Overview

--------------------------------------------
**eswitch_v5** is a library, which is an improved version of **switch statement** in C++(or just **native switch**). The main idea behind this library is to be able to overcome **native switch** limitations, such as:
- one _argument_ per **native switch**
- the _argument_ restricted to only _integral_ types( <span style="color:orange">int</span>, <span style="color:orange">char</span>, <span style="color:orange">enum</span> ... )
- impossibility to _compose complex conditions_, as opposed to other **statements** like <span style="color:orange">if</span>, <span style="color:orange">else if</span>, along with *loops* such as <span style="color:orange">while</span> and <span style="color:orange">for</span>.

**eswitch_v5** supports any number of _arguments_ and almost without restriction on their _type_
 as long as the _type_ is **comparable**( i.e. has <span style="color:orange">operator==</span>, <span style="color:orange">operator!=</span> and so on ).
 Additionally, my library allows to match for **std::any, std::variant<...>, polymorphic type match** and
 **std::regex**. Also it allows to withdraw values from those types, of course if the match was successful.

### Motivation

--------------------------------------------

I don't see any good reason why **switch statement** in C++ is so limited, whereas other 
statements such as **if, else if** including _loops_ **while** and **for** have no such limitations and they allow to compose and
test complex condition. And here is why:
- In terms of _**assembler**_ output, **if** and **switch** 
statements give the same [output](https://godbolt.org/z/G3Woj6), thus the performance also the same.
```cpp
 ============================================================================                          
|                                       |   foo(int):            # @foo(int) |                           
|  int foo(int num)                     |       xor     ecx, ecx             |               
|  {                                    |       cmp     edi, 15              |              
|      if     ( num == 10 ) return 1;   |       sete    cl                   |         
|      else if( num == 15 ) return 2;   |       xor     ecx, 3               |             
|      else                 return 3;   |       cmp     edi, 10              |              
|  }                                    |       mov     eax, 1               |             
|                                       |       cmovne  eax, ecx             |               
|                                       |       ret                          |                            
|=======================================+====================================|                                
|  int bar(int num)                     |    bar(int):           # @bar(int) |                                                                    
|  {                                    |       xor     ecx, ecx             |                   
|      switch( num )                    |       cmp     edi, 15              |                                  
|      {                                |       sete    cl                   |                 
|          case 10: return 1;           |       xor     ecx, 3               |                                          
|          case 15: return 2;           |       cmp     edi, 10              |                                           
|          default: return 3;           |       mov     eax, 1               |                                          
|      };                               |       cmovne  eax, ecx             |                        
|  }                                    |       ret                          |      
 ============================================================================                                
```

- Also I don't think that limitations related to compatibility with **C**, since syntax of **if statement** is still compatible with **C** even though it was extended in **C++17**, this extension allows to declare variable  within **if statement** like this: 
```cpp
if( std::smatch mt; std::regex( text, mt, rgx ) ) {...}
```
Moreover this situation seem unfortunate for other people as well:

- There was the [proposal](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3627.html) for **C++ standard committee**: ''To allow to use _complex types_( like **strings**, **complex numbers**, etc ) within **switch statement**''. And **committee** <span style="color:green"> *agreed* </span> about **importance of this topic**. 
But somehow the author stopped his work toward this direction.


- Compilers such as **clang** and **gcc** have non-standard extension for matching [**case ranges**](https://gcc.gnu.org/onlinedocs/gcc/Case-Ranges.html).\n
**For example**:
```cpp
switch( num )               |    switch( ch )
{                           |    {
    case 1 ... 5:   break;  |        case 'A' ... 'Z': break;          
    case 10 ... 99: break;  |        case 'a' ... 'z': break;      
};                          |    };
```

- On top of that the internet is full of questions with over million views:
    - [Why strings cannot be used in **switch statement**?](https://stackoverflow.com/questions/650162/why-the-switch-statement-cannot-be-applied-on-strings)
    - [How to compose complex condition in **switch statement**?](https://stackoverflow.com/questions/68578/multiple-cases-in-switch-statement)

- Programming language **swift** has pretty advanced **switch statement**.

The evidence above tell us that people don't like inconsistency and limitations of **switch statement**, and they were trying to overcome 
them with different approaches( by implementing **non-standard extension**, writing **proposals** or just **searching for the solution/workaround in the internet** ).

Based on those factors I decided to write my own implementation of **enhanced switch**( or just **eswitch** ).
In my implementation I tried:
1. to address all the decribed **limitations**
2. leave the syntax of **eswitch** as close as possible to C++ **switch statement**. Compare:\n 
@code{cpp}
    switch( num )                |    eswitch( num )                                                        
    {                            |    (                       
        case 1:  {...} break;    |        Case( 1 ) {...},                                               
        case 2:  {...} break;    |        Case( 2 ) {...},                                               
        default: {...} break;    |        Default   {...}                                               
    };                           |    );                        
@endcode\n
Pretty close, huh? Except the places where I was either limited by language or intentionally tried to avoid certain behavior of **switch statement** in C++ like default **fallthrough**.\n
\n
@note **Switch statement** in C++ has _explicit_ **break** and _implicit_ **fallthrough**.

\n
This behavior is considered to be error-prone. Since developers sometimes forget to use 
**break** and because of this their code doesn't work the way it was intended.
Thus in my implementation I reversed this concept i.e. **eswitch** has _implicit_ **break** 
and _explicit_ **fallthrough**. Compare:\n
@code{cpp}
switch( num )                |    eswitch( num )     
{                            |    (     
    case 1:  {...}           |        Case( 1 ){...} ^ fallthrough_, 
    case 2:  {...} break;    |        Case( 2 ){...},     
    default: {...} break;    |        Default  {...}     
};                           |    );     
@endcode

3. And last but not least, another my priority was the performance 
of **eswitch**, which shouldn't differ by much from **native switch**.

\subsection tutorial-installation Installation

--------------------------------------------
Since this library is header-only and on top of that whole library was implemented within single file,
thus you can get that file from **eswitch_v5** repository on [github](https://github.com/rabdumalikov/eswitch_v5/blob/main/include/eswitch/eswitch_v5.hpp). And in order to compile with eswitch_v5, just `#include <eswitch_v5.hpp>`.

\subsection tutorial-license License

--------------------------------------------
This code is distributed under the Boost Software License, Version 1.0. (See
accompanying file [LICENSE.txt](https://github.com/rabdumalikov/eswitch_v5/blob/main/LICENSE.txt) or copy at http://www.boost.org/LICENSE_1_0.txt)


\subsection tutorial-compilers Supported Compilers

--------------------------------------------------------------------------------
Should work on all major compilers which support **C++20**. I personally tested on following:

- **clang++-11** (or later)
- **g++-10.2** (or later)
- **Visual Studio 2019** - isn't supported for now, just because [familiar template syntax for generic lambdas](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0428r2.pdf) from **C++20** wasn't implemented by _Microsoft_ compiler.

### Conventions used in this document

--------------------------------------------
In all code examples, I omit the namespace prefixes for names in the **eswitch_v5** and **std** namespaces.

\section tutorial-implementation-details Implementation Details

--------------------------------------------------------------------------------
This section contains all the details, which user need to know in order to use this library successfully.

### Keywords

--------------------------------------------

| Name | Description |
| :---: | :---: |
| ***eswitch*** | _accepts list of arguments_ |
| ***Case*** | _accepts **condition** to check and body next to it will be executed if **condition** matched_ |
| ***Default*** | _body next to it will be executed if nothing else matched_ |
| ***fallthrough_*** | _next body will be executed without checking its **condition**_ |
| ***any_from*** | _accepts values to choose from_ |
| ***is<sometype>*** | _used within **Case** for matching types like **std::any**, **std::variant<...>** and **polymorphic type match**_ |
| ***_r*** | _user defined literal for **std::regex**_ |



### Syntax {#syntax}

--------------------------------------------

**1. Full declaration**
```cpp
eswitch( __arguments__ )
( 
    Case( __conditions__ )( __param__ ) { __body__ } ^ __options__,
    Default { __body__ }
);
```
**2. Omitted parameter**: same as lambda without **parameter**.
```cpp
eswitch( __arguments__ )
( 
    Case( __conditions__ ) { __body__ } ^ __options__,
    Default { __body__ }
);
```
**3. Omitted setting options**: default option( **break** ) will be used.
```cpp
eswitch( __arguments__ )
( 
    Case( __conditions__ ) { __body__ },
    Default { __body__ }
);
```
**4. Omitted 'Default'**: no fallback
```cpp
eswitch( __arguments__ )
( 
    Case( __conditions__ ) { __body__ }
);
```

**Explanation**

<table>
<tr><th>Name<th>Details
<tr>
    <td>
        <span style="color:blue">__arguments__</span>
    <td>
        <div align="left">
            The list of arguments( i.e. <b>arg_1</b>, <b>arg_2</b>, ..., <b>arg_n</b> )
        </div>
<tr>
    <td rowspan="2">
        <span style="color:blue">__conditions__</span>
    <td>
        <div align="left">
        It is a _lazy expression_, where indexes <b>_1</b>, <b>_2</b>, ... represent one-to-one<br> correspondance with arguments in **eswitch**. Consider following code:
```cpp 
eswitch(arg_1, arg_2, ..., arg_n)
(
    Case( _1 == smth_1 || _2 == smth_2 || ... ) {...}
);
// _1 refer to arg_1
// _2 refer to arg_2
// ...
```

**Possible usages:**
```cpp
Case( _1 == smth1 && _2 == smth2 && ... )                        (1)

Case( smth1, smth2, ... )                                        (2)

Case( _1 == any_from( smth1, smth2, ... ) )                      (3)

Case( any_from( smth1, smth2, ... ) )                            (4)

Case( ( pred1, _1 ) && ( pred2, _2 ) && ... )                    (5)
```      
</div>
<tr>
    <td>
        <div align="left">
        <ol>
            <li>Match **in order**<br> 
            <li>Same as the <b>1st</b> one, but less verbose<br> 
            <li>Match via **any_from**<br>
            <li>Same as the <b>3rd</b> one, but less verbose<br> 
            <li>Match via **predicate**
        </ol>
        </div>
<tr>
    <td>
        <div align="center">
            <span style="color:blue">__body__</span>
        </div>
    <td>
        <div align="left">
            Function body
        </div>
<tr>
    <td>
        <div align="center">
            <i><b>Optional:</b></i><br>
            <span style="color:blue">__param__</span>
        </div>
    <td>
        <div align="left">Correspond to withdrawn value from: **std::any**, **std::variant<...>**,<br> 
            **polymorphic match** or **std::regex match**. But also it correspond to<br>
            **returned value** wrapped into `std::optional<...>` from custom extensions.<br>
            @note
            Keyword [_auto_](https://en.cppreference.com/w/cpp/language/auto) here is forbidden,<br>
            i.e. type of <b>__param__</b> should be specified explicitly.
        </div>
<tr>
    <td>
        <div align="center">
            <i><b>Optional:</b></i><br>
            <span style="color:blue">__options__</span>
        </div>
    <td>
        <div align="left">
            [**left empty** => _break_] or<br>
            [**fallthrough_** => _execute body of the following case_] or<br>
            [**likely_** which will be introduced in future]
        </div>
</table>

### Features
   
--------------------------------------------
<table>
<tr><th>After<th>Before
<tr>
<td colspan="2">
<div align="left">
<b>any_from helper function</b> - This helper function allows to match for one of its entry.
</div>
<tr>
    <td>
```cpp                                                                                                                                                        
eswitch( file_extension )                                   
(                                                           
    Case( any_from( "cpp", "c++", "cxx", "cc", "C" ) )                  
    {                                                       
        return "source";                                    
    },                                                      
    Case( any_from( "hpp", "h++", "hxx", "hh", "h", "H" ) )             
    {                                                       
        return "header";                                    
    },                                                      
    Default                                                 
    {                                                       
        return "unknown";                                   
    }                                                       
);                                                          
``` 
<td>
```cpp                                                                   
if( file_extension == "cpp" ||                                                                                                      
    file_extension == "c++" ||                                          
    file_extension == "cxx" ||                                                                                                    
    file_extension == "cc"  ||                                             
    file_extension == "C" )                                                                  
{                                              
    return "source";                                                                                                    
}                                              
else if(                                                                  
    file_extension == "hpp" ||                                               
    file_extension == "h++" ||                                                     
    file_extension == "hxx" ||                                              
    file_extension == "hh"  ||                                                                  
    file_extension == "h"   ||                                              
    file_extension == "H" )                                                         
{                                                                 
    return "header";                                                 
}                                                                                     
else                                                                   
    return "unknown";                                                                                                                      
```
<tr>
<td colspan="2">
<div align="left">
<b>Match and withdraw value from std::any and std::variant<></b> - In term of my implementation the usage for matching type in either of those objects is unified, whereas in raw C++ usage is different.
</div>
<tr>
<td rowspan="2" >
```cpp                                                                                                                                                        
eswitch( any_or_variant_obj )
(
    Case( is< int >{} )( const int value )
    {
        ...
    },
    Case( is< string >{} )( const string & value )
    {
        ...
    }
);                                                                                                                                                                                                                                                                                                  
``` 
<td>
```cpp                                                                   
// std::any type match                                  
if( auto * value = any_cast< int >( &any_obj ) )        
{                                                       
    ...                                                 
}                                                       
else if( auto * value = any_cast< string >( &any_obj ) )
{                                                       
    ...                                                 
}                                                                                                                                                                          
```
<tr>
<td>
```cpp                                                                   
// std::variant<> type match                              
if( auto * value = get_if< int >( &variant_obj ) )        
{                                                         
    ...                                                   
}                                                         
else if( auto * value = get_if< string >( &variant_obj ) )
{                                                         
    ...                                                   
}                                                                                                                                                                                                                              
``` 
</table>

#### Match for polymorphic types where base object is pointer

```cpp
eswitch( base_ptr )                                            |                                                                                                                                                                                                                                                                                                                                                      
(                                                              |    if( auto * c = dynamic_cast< circle* >( base_ptr ) )                                                                                                                                                                                                                       
    Case( is< circle >{} )( circle * c )                       |    {                                                                                                                                                            
    {                                                          |        c->draw();                                                                                                            
        c->draw();                                             |    }                                                                                                                                      
    },                                                         |    else if( auto * s = dynamic_cast< square* >( base_ptr ) )                                                                                                                                                                                                                                    
    Case( is< square >{} )( square * s )                       |    {                                                                                                                                                            
    {                                                          |        s->draw();                                                                                                            
        s->draw();                                             |    }                                                                                                                    
    }                                                          |                                                                                                                                                                                                        
);                                                             |  
```                                                           
                                                                     
#### Match for polymorphic types where base object is reference
```cpp                                                         
                                                               |    try                                                                                         
eswitch( base_ref )                                            |    {                           
(                                                              |        auto & c = dynamic_cast< circle& >( base_ptr );                                                                                 
    Case( is< circle >{} )( circle & c )                       |        c.draw();                                                                                
    {                                                          |        return;                            
        c.draw();                                              |    }                          
    },                                                         |    catch( const bad_cast & ) { ... }                   
    Case( is< square >{} )( square & s )                       |                              
    {                                                          |    try                          
        s.draw();                                              |    {                          
    }                                                          |        auto & s = dynamic_cast< square& >( base_ptr );                          
);                                                             |        s.draw();                          
                                                               |        return;                          
                                                               |    }                          
                                                               |    catch( const bad_cast & ) { ... }                                             
```

#### Match for std::regex
                                                       
```cpp
                                                               |    smatch match;                                                                  
eswitch( text )                                                |    if( regex_match( text, match, regex( R"(\w*)" ) ) )                 
(                                                              |    {                                                             
    Case( R"(\w*)"_r ) { return "message"; },                  |        return "message";                                                           
    Case( R"(\d*)"_r ) { return "number"; }                    |    }                  
);                                                             |    else if( regex_match( text, match, regex( R"(\d*)" ) ) )                
                                                               |    {                
                                                               |        return "number";                
                                                               |    }                
```

#### Match and withdraw values from std::regex

```cpp
eswitch( text )                                                |                                                                         
(                                                              |    smatch match;                                                           
    /* NOTE: "vector<string>" same as "std::smatch" */         |    if( regex_match( text, match, regex( R"((\w*))" ) ) )                                                                   
    Case( R"((\w*))"_r )( vector< string > && match )          |    {                                                                                                                            
    {                                                          |        return match[1];                                                            
        return match[1];                                       |    }                                                    
    }                                                          |    else if( regex_match( text, match, regex( R"((\d*))" ) ) )                
    Case( R"((\d*))"_r )( vector< string > && match )          |    {                                                                               
    {                                                          |        return match[1];                                
        return match[1];                                       |    }                                                      
    }                                                          |
);                                                             |
```

#### Match for individual entries in std::pair

```cpp
eswitch( make_pair( 10, string{"Text"} ) )                     |    auto pr = make_pair( 10, string{"Text"} );                                                              
(                                                              |        
    Case( 10, "Text" ){ ... }                                  |    if( pr.first == 10 && pr.second == "Text" ) { ... }
);                                                             |
```

#### Match for individual entries in std::tuple                                         

```cpp
                                                               |    auto tup = make_tuple( 1, 0, 0, 1 );                                                                                          
eswitch( std::make_tuple( 1, 0, 0, 1 ) )                       |                                                                                                                
(                                                              |    auto [x,y,z,k] = tup;                                                                                              
    Case( 1, 0, 0, 1 ) { return 9; },                          |                                                                                                        
    Case( 1, 1, 1, 1 ) { return 15; }                          |    if( x == 1 && y == 0 && z == 0 && k == 1 )                                                                                                                                    
);                                                             |    {                                                                                                        
                                                               |        return 9;                                                                  
                                                               |    }                                                                                                       
                                                               |    else if( x == 1 && y == 1 && z == 1 && k == 1 )                                                                
                                                               |    {                                                                                                                                                          
                                                               |       return 15;          
                                                               |    }                                       
```

#### Check in range

```cpp
eswitch( p1 )                                                  |
(                                                              |    if     ( num >   1 && num <  10 ) { ... }                 
    Case( _1.between( 1, 10 ) ) {...},                         |    else if( num >= 11 && num <= 20 ) { ... }            
    Case( _1.within( 11, 20 ) ) {...}                          |                                          
);                                                             |       
```
```cpp
eswitch( p1 )
(
    Case( _1 >  1  && _1 <  10 ) {...},
    Case( _1 >= 11 && _1 <= 20 ) {...}
);
```

#### Arguments referencing via indexes

```cpp
eswitch( p1, p2 )
(
    Case( _1 == true && _2 == true ) {...},
    Case( _2 != true || _1 == true ) {...}
);   
```

#### If indexes aren't provided, then it is assumed that they're sequential

```cpp
eswitch( p1, p2 )
(
    Case( true, false ) {...}, // same as "Case( _1 == true && _2 == false )" 
    Case( false ) {...}
);
```

#### Matching via predicates( it could be either a lambda or a free function ):

```cpp
eswitch( p1, p2 )
(
    Case( ( is_odd, _1 ) && ( is_odd, _2 ) ) {...},
    Case( ( is_negative( _1, _2 ) ) {...}
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

#### Returning value from Case

```cpp
bool val = eswitch( p1 ) 
(
    Case( 1 ) { return true; }
);
```
<table>
<tr><th>After<th>Before
<tr>
    <td>
```cpp
    bool val = eswitch( p1 ) 
    (
        Case( 1 ) { return true; }
    );
```
<td>
```cpp
    bool val = eswitch( p1 ) 
    (
        Case( 1 ) { return true; }
    );
```
</table>

## How to write Custom extensions?

--------------------------------------------

This guide is about how to <u>use</u> _custom types_ in **Case** or <u>define</u> _custom behavior_ for some types. We will implement **custom extension** and I walk you through all the steps and details.

**Thing to know:**

The only things which we can be customized are all the **comparison** `operators` such as '==', '!=', '>', '<' and so on.

**Let's begin:**

As you may already know that comparing **floating points** is a tricky thing. **For example** following code fails( because of the _precision_ ).
```cpp
eswitch( 2 - 0.7000000001 )
(
    Case( 1.3 )
    {
        ...
    },
    Default 
    { 
        assert( false ); 
    }
);
```
So standard way of comparing **floating points** doesn't work properly. On top of that, simple overloading `operator==` for primitives is [forbidden](https://eel.is/c++draft/over.oper#general-7).

```cpp
bool operator==( const double d1, const double d1 )
{
    ...
}
/// Compilation ERROR: overloaded 'operator==' must have at least one parameter of class or enumeration type
```
To overcome this we need to use an **intermediate class** which will hold our value. Like that: 
```cpp
struct double_value
{
    double value;
};
```
Then we can use **double_value** in our `operator==` :
```cpp
bool operator==( const double d1, const double_value d2 )
{
    return fabs( d1 - d2.value ) < __FLT_EPSILON__;
}
```
And now code below will work as we desired( since using **double_value** allows for compiler through **name lookup** to find our custom `operator==` ).
```cpp
eswitch( 2 - 0.7000000001 )
(
    Case( double_value{ 1.3 } ) 
    {
        ...
    },
    Default 
    { 
        assert( false ); 
    }
);
```
**Full example**: [Floating point comparison](@ref example-floating-point-comparison)

@note
- An **intermediate class** should be used all the time( not only for primitives due to compiler restrictions ), otherwise it won't be possible for compiler to find custom `operator`( unless this `operator` will be defined before `#include <eswitch_v5.hpp>`, only then **intermediate class** won't be needed ).

- Also example below demonstrate if certain value should be returned from custom `operator` and transferred to **Case** as [input parameter](#syntax), then the value should be wrapped into `std::optional`. 

**For example:** [Value and Type transferring](@ref example-value-and-type-transferring)

## Rationalities

--------------------------------------------

- Using macroses for **Case** and **Default** allowed me to be as close as possible to C++ **switch statement** regarding syntax, otherwise there was no way to hide **lambda** declaration. 
- Setting properties via `operator^` - It is the only `operator` which is used to set 
certain properties for **values/matrices**( in **matlab** ), which I find reasonable. Thus I used this notation in my library, like this:
```cpp 
eswitch( some_var )
(
    Case( some_value_1 ) {} ^ likely_,
    Case( some_value_2 ) {} ^ fallthrough_
);
```
- I can use hash in order to use string within **switch statement**:

- Why do we need another statement? That's is reasonable questions to ask.