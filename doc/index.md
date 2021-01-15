User Manual       {#mainpage}
===========

\tableofcontents

\section tutorial-overview Overview

--------------------------------------------
**eswitch_v5** is a library, which is an improved version of <span class="keywordflow">`switch`</span> statement in C++. The main idea behind this library is to be able to overcome <span class="keywordflow">`switch`</span> statement limitations, such as:
- one argument per <span class="keywordflow">`switch`</span> statement
- the argument restricted to only _integral_ types( <span class="keyword">`int`</span>, <span class="keyword">`char`</span>, <span class="keyword">`enum`</span> ... )
- impossibility to _compose complex conditions_, as opposed to other _statements_ and _loops_ in C++

**eswitch_v5** supports any number of arguments and almost without restriction on their _type_ as long as the _type_ is **comparable**( i.e. has `operator==`, `operator!=` and so on ). Additionally, my library went beyond overcoming limitations of <span class="keywordflow">`switch`</span> statement e.g. I introduced more expressive way for matching and withdrawing values from `std::any`, `std::variant`, `polymorphic types` and
`std::regex`.

### Motivation

--------------------------------------------

I don't see any good reason why <span class="keywordflow">`switch`</span> statement in C++ is so limited, whereas other statements such as <span class="keywordflow">`if`</span>, <span class="keywordflow">`else if`</span> including loops <span class="keywordflow">`while`</span> and <span class="keywordflow">`for`</span> have no such limitations and they allow to compose and test complex condition. And here is why:
- In terms of **assembler** output, <span class="keywordflow">`if`</span> and <span class="keywordflow">`switch`</span> 
statements give the same [output](https://godbolt.org/z/G3Woj6), thus the performance also the same.
<table>
<tr>
<td colspan="2">
<div align="left">
<b>compiler: clang, flags: -O3</b>
</div>
<tr>
    <td>
```cpp
int foo( int num )
{
         if( num == 10 ) return 1;
    else if( num == 15 ) return 2;
    else                 return 3;
}
```
<td>
```
foo(int):            # @foo(int)
    xor     ecx, ecx            
    cmp     edi, 15             
    sete    cl                  
    xor     ecx, 3              
    cmp     edi, 10             
    mov     eax, 1              
    cmovne  eax, ecx            
    ret                         
```
<tr>
    <td>
```cpp
int bar( int num )
{
    switch( num )
    {
        case 10: return 1;
        case 15: return 2;
        default: return 3;
    } 
}
```
<td>
```asm
bar(int):            # @bar(int)
    xor     ecx, ecx            
    cmp     edi, 15             
    sete    cl                  
    xor     ecx, 3              
    cmp     edi, 10             
    mov     eax, 1              
    cmovne  eax, ecx            
    ret                         
```
</table>

- Also I don't think that limitations related to compatibility with **C**, since syntax of <span class="keywordflow">`if`</span> statement is still compatible with **C** even though it was extended in **C++17**, this extension allows to declare variable  within <span class="keywordflow">`if`</span> statement like this: 
```cpp
if( std::smatch mt; std::regex_match( text, mt, rgx ) ) {...}
```
Moreover this situation seem unfortunate for other people as well:

- There was the [proposal](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3627.html) for C++ standard committee: ''To allow to use _complex types_( like **strings**, **complex numbers**, etc ) within <span class="keywordflow">`switch`</span> statement''. And committee agreed about importance of this topic. 
But somehow the author stopped his work toward this direction.


- Compilers such as **clang** and **gcc** have non-standard extension for matching [case ranges](https://gcc.gnu.org/onlinedocs/gcc/Case-Ranges.html).\n
**For example**:
<table cellspacing="0" cellpadding="0">
<tr>
    <td>
```cpp
switch( num )
{
    case  1 ... 9:  break;
    case 10 ... 99: break;
};                         
```
<td>
```cpp
switch( ch )
{
    case 'A' ... 'Z': break;
    case 'a' ... 'z': break;
};                         
```
</table>

- On top of that the internet is full of questions with over million views:
    - [Why strings cannot be used in switch statement?](https://stackoverflow.com/questions/650162/why-the-switch-statement-cannot-be-applied-on-strings)
    - [How to compose complex condition in switch statement?](https://stackoverflow.com/questions/68578/multiple-cases-in-switch-statement)

- Programming language **swift** has pretty advanced <span class="keywordflow">`switch`</span> statement.

The evidence above tell us that people don't like inconsistency and limitations of <span class="keywordflow">`switch`</span> statement, and they were trying to overcome 
them with different approaches( by implementing non-standard extension, writing proposals or just searching for the solution/workaround in the internet ).

Based on those factors I decided to write my own implementation of **enhanced** <span class="keywordflow">`switch`</span> statement( or just **eswitch** ).
In my implementation I tried:
1. to address all the decribed limitations
2. leave the syntax of **eswitch** as close as possible to <span class="keywordflow">`switch`</span> statement. Compare:
<table cellspacing="0" cellpadding="0" >
<tr>
    <td>
```cpp
switch( num )
{ 
    case 1:  {...} break;
    case 2:  {...} break;
    default: {...} break;
};          
```
<td>
```cpp
eswitch( num )
(              
    Case( 1 ) {...},
    Case( 2 ) {...},
    Default   {...}
);  
```
</table>
Pretty close, huh? Except the places where I was either limited by language or intentionally tried to avoid certain behavior of <span class="keywordflow">`switch`</span> statement like default **fallthrough**.\n
\n
@note In C++ <span class="keywordflow">`switch`</span> statement has explicit <span class="keywordflow">`break`</span> and implicit **fallthrough**.

\n
This behavior is considered to be error-prone. Since developers sometimes forget to use 
<span class="keywordflow">`break`</span> and because of this their code doesn't work the way it was intended. Thus in my implementation I reversed this concept i.e. **eswitch** has implicit <span class="keywordflow">`break`</span> and explicit **fallthrough**. Compare:
<table cellspacing="0" cellpadding="0">
<tr>
    <td>
```cpp
switch( num )            
{                        
    case 1:  {...}       
    case 2:  {...} break;
    default: {...} break;
};                               
```
<td>
```cpp
eswitch( num )     
(     
    Case( 1 ){...} ^ fallthrough_,
    Case( 2 ){...},     
    Default  {...}     
);      
```
</table>

3. And last but not least, another my priority was the performance 
of **eswitch**, which shouldn't differ by much from <span class="keywordflow">`switch`</span> statement.

### Comparison

--------------------------------------------
Since full comparison of **eswitch** vs <span class="keywordflow">`switch`</span> statement isn't possible due to the limitations of
the latter. Thus I'm going to do comparison with <span class="keywordflow">`if`</span> statement instead.

<table>
<tr><th>After<th>Before
<tr>
<td colspan="2">
<div align="left">
<b>any_from helper function</b>
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
```
<tr>
<td colspan="2">
<div align="left">
<b>Match and withdraw value from `std::any` and `std::variant`</b> - in terms of this  library the usage for matching type in either of those objects is unified, whereas in raw C++ usage is different.
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
std::any any_obj = ...;

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
std::variant< int, string > variant_obj = ...;

if( auto * value = get_if< int >( &variant_obj ) )
{
    ...
}
else if( auto * value = get_if< string >( &variant_obj ) )
{
    ...
}
``` 
<tr>
<td colspan="2">
<div align="left">
<b>Match for polymorphic types</b>
</div>
<tr>
<td>
```cpp
eswitch( base_ptr )
(
    Case( is< circle >{} )( circle * c )
    {
        c->draw();
    },
    Case( is< square >{} )( square * s )
    {
        s->draw();
    }
);
```
<td>
```cpp
if( auto * c = dynamic_cast< circle* >( base_ptr ) )
{
    c->draw();
}
else if( auto * s = dynamic_cast< square* >( base_ptr ) )
{
    s->draw();
}
```
<tr>
<td>
```cpp
eswitch( base_ref )
(
    Case( is< circle >{} )( circle & c )
    {
        c.draw();
    },
    Case( is< square >{} )( square & s )
    {
        s.draw();
    }
);
```
<td>
```cpp
try
{
    auto & c = dynamic_cast< circle& >( base_ptr );
    c.draw();
    return;
}
catch( const bad_cast & ) { ... }

try
{
    auto & s = dynamic_cast< square& >( base_ptr );
    s.draw();
    return;
}
catch( const bad_cast & ) { ... }
```
<tr>
<td colspan="2">
<div align="left">
<b>Match for std::regex</b>
</div>
<tr>
<td>
```cpp
eswitch( text )
(
    Case( R"(\w*)"_r ) { return "message"; },
    Case( R"(\d*)"_r ) { return "number"; }
);
```
<td>
```cpp
smatch match;
if( regex_match( text, match, regex( R"(\w*)" ) ) )
{  
    return "message";
}
else if( regex_match( text, match, regex( R"(\d*)" ) ) )
{
    return "number";
}
```
<tr>
<td colspan="2">
<div align="left">
<b>Match and withdraw values from std::regex</b>
</div>
<tr>
<td>
```cpp
eswitch( text )
(
    Case( R"((\w*))"_r )( vector< string > && match )
    {
        return match[1];
    }
    Case( R"((\d*))"_r )( vector< string > && match )
    { 
        return match[1];
    }
);
```
<td>
```cpp
smatch match;
if( regex_match( text, match, regex( R"((\w*))" ) ) )
{
    return match[1];
}
else if( regex_match( text, match, regex( R"((\d*))" ) ) )
{
    return match[1];
}
```
<tr>
<td colspan="2">
<div align="left">
<b>Match for individual entries in std::pair</b>
</div>
<tr>
<td>
```cpp
eswitch( make_pair( 10, string{"Text"} ) )
(
    Case( 10, "Text" ){ ... }
);
```
<td>
```cpp
auto pr = make_pair( 10, string{"Text"} );

if( pr.first == 10 && pr.second == "Text" ) { ... }
``` 
<tr>
<td colspan="2">
<div align="left">
<b>Match for individual entries in std::tuple</b>
</div>
<tr>
<td>
```cpp
eswitch( make_tuple( 1, 0, 0, 1 ) )
(
    Case( 1, 0, 0, 1 ) { return 9; },
    Case( 1, 1, 1, 1 ) { return 15; }
);
```
<td>
```cpp
auto tup = make_tuple( 1, 0, 0, 1 );

auto [ x, y, z, k ] = tup;

if( x == 1 && y == 0 && z == 0 && k == 1 )
{
    return 9;
}
else if( x == 1 && y == 1 && z == 1 && k == 1 )
{
   return 15;
}
```
<tr>
<td colspan="2">
<div align="left">
<b>Check in range</b>
</div>
<tr>
<td>
```cpp
eswitch( p1 )
(
    Case( _1.between( 1, 10 ) ) {...},
    Case( _1.within( 11, 20 ) ) {...}
);
```
<td>
```cpp
if     ( num >   1 && num <  10 ) { ... }
else if( num >= 11 && num <= 20 ) { ... }
```
</table>

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
| ***Case*** | _accepts condition to check and body next to it will be executed if condition matched_ |
| ***Default*** | _body next to it will be executed if nothing else matched_ |
| ***fallthrough_*** | _next body will be executed without checking its condition_ |
| ***any_from*** | _accepts values to choose from_ |
| ***is<sometype>*** | _used within **Case** for matching types like `std::any`, `std::variant` and `polymorphic types match`_ |
| ***_r*** | _user defined literal for `std::regex`_ |



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
**2. Omitted parameter**: same as lambda without parameter
```cpp
eswitch( __arguments__ )
( 
    Case( __conditions__ ) { __body__ } ^ __options__,
    Default { __body__ }
);
```
**3. Omitted setting options**: default option( <span class="keywordflow">`break`</span> ) will be used
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
        </div>
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

<tr>
    <td>
        <div align="left">
        <ol>
            <li>Match in order<br> 
            <li>Same as the 1st one, but less verbose<br> 
            <li>Match via `any_from`<br>
            <li>Same as the 3rd one, but less verbose<br> 
            <li>Match via predicate
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
        <div align="left">Correspond to withdrawn value from: `std::any`, `std::variant`,<br> 
            `polymorphic match` or `std::regex` match. But also it correspond to<br>
            returned value wrapped into `std::optional` from custom extensions.<br>
            @note
            Keyword <span class="keyword">`auto`</span> here is forbidden,<br>
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

## How to write Custom extensions?

--------------------------------------------

This guide is about how to <u>use</u> _custom types_ in **Case** or <u>define</u> _custom behavior_ for some types. We will implement custom extension and I walk you through all the steps and details.

**Thing to know:**

The only things which we can be customized are all the comparison `operators` such as '==', '!=', '>', '<' and so on.

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
So standard way of comparing **floating points** doesn't work properly. On top of that, simple overloading of `operator==` for primitives is [forbidden](https://eel.is/c++draft/over.oper#general-7).

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
Then we can use **double_value** in our overloaded `operator==` :
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

**Example:** [Value and Type transferring](@ref example-value-and-type-transferring)

## Rationalities

--------------------------------------------

- Using macroses for **Case** and **Default** allowed me to be as close as possible to <span class="keywordflow">`switch`</span> statement regarding syntax, otherwise there was no way to hide **lambda** declaration. 
- Setting properties via `operator^` - It is the only `operator` which is used to set 
certain properties for **values/matrices**( in **matlab** ), which I find reasonable. Thus I used this notation in my library, like this:
```cpp 
eswitch( some_var )
(
    Case( some_value_1 ) {} ^ likely_,
    Case( some_value_2 ) {} ^ fallthrough_
);
```
- I can use hash in order to use string within <span class="keywordflow">`switch`</span> statement:

- Why do we need another statement? That's is reasonable questions to ask.