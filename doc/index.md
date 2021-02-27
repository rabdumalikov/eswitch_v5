User Manual       {#mainpage}
===========

\tableofcontents

@htmlonly
<!-- Place this tag in your head or just before your close body tag. -->
<script async defer src="https://buttons.github.io/buttons.js"></script>

<a class="github-button" href="https://github.com/rabdumalikov/eswitch_v5" data-color-scheme="no-preference: light; light: light; dark: light;" data-size="large" aria-label="View on github">View on github</a>
@endhtmlonly

\section tutorial-overview Overview

--------------------------------------------
`eswitch_v5` is a library, which is an improved version of <span class="keywordflow">`switch`</span> statement in C++. The main idea behind this library is to be able to overcome <span class="keywordflow">`switch`</span> statement limitations, such as:
- one argument per <span class="keywordflow">`switch`</span> statement
- the argument restricted to only _integral_ types( <span class="keyword">`int`</span>, <span class="keyword">`char`</span>, <span class="keyword">`enum`</span> ... )
- impossibility to compose complex conditions, as opposed to other _statements_ and _loops_ in C++

`eswitch_v5` supports any number of arguments and almost without restriction on their type as long as the type is comparable( i.e. has `operator==`, `operator!=` and so on ). Additionally, my library went beyond overcoming limitations of <span class="keywordflow">`switch`</span> statement e.g. I introduced more expressive way for matching and withdrawing values from `std::any`, `std::variant`, `polymorphic types` and
`std::regex`.

### Motivation

--------------------------------------------
Let's start with a little bit of backstory. As you know in C++, the <span class="keywordflow">`switch`</span> statement is quite limited( compared with other statements ) and there are few reasons for this, which are mainly related to optimization. Compiler can use **jump table** or **binary search** to optimize the <span class="keywordflow">`switch`</span> statement, but nothing comes without a price as such optimizations are possible only for compile-time data. In C++98, only values of primitives like <span class="keyword">`int`</span>, <span class="keyword">`char`</span>, <span class="keyword">`enum`</span> and <span class="keyword">`bool`</span> can be known at compile time. That's why the <span class="keywordflow">`switch`</span> statement was restricted to primitive-only. However, in C++11, the new keyword <span class="keyword">`constexpr`</span> was introduced. Which opened the doors for compile-time custom types. So now it has become possible to overcome the primitive-only limitation. There was also the [proposal](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3627.html) in this regard. Unfortunately, for whatever reason, the author decided to bail out, even though the feedback from the committee was positive. Nonetheless, even if this proposal was accepted in the C++ standard, the <span class="keywordflow">`switch`</span> statement still wouldn't be as powerful as the <span class="keywordflow">`if`</span> statement.

Let us now carefully examine the <span class="keywordflow">`if`</span> statement. It can be used for various data types that are known at compile time and at runtime. In addition to this, it allows to compose and test complex conditions. Wait wait wait, if the <span class="keywordflow">`if`</span> statement can work with data known at compile-time, then why can't compiler optimize this statement just like the <span class="keywordflow">`switch`</span> statement? In theory, it can. Let me even say that there is a compiler that is capable of doing so and this is clang.

**For example**:
@htmlonly
<button onclick="location.href='https://godbolt.org/z/rq7fzn'" type="button">
         View on <strong>CompilerExplorer</strong></button>
@endhtmlonly
<table>
<tr>
    <td>
```cpp
int foo(int num) 
{
    if     ( num == 10 ) return 1;
    else if( num == 13 ) return 2;
    else if( num == 11 ) return 3;
    else if( num == 19 ) return 4;
    else if( num == 12 ) return 5;
    else                 return 6;
}











int bar(int num) {
    switch( num )
    {
        case 19: return 4;
        case 10: return 1;
        case 13: return 2;
        case 11: return 3;
        case 12: return 5;
        default: return 6;
    };
}                     
```
<td>
```cpp
foo(int):                                # @foo(int)
        add     edi, -10
        cmp     edi, 9
        ja      .LBB0_2
        movsxd  rax, edi
        mov     eax, dword ptr [4*rax + .Lswitch.table.bar(int)]
        ret
.LBB0_2:
        mov     eax, 6
        ret
bar(int):                                # @bar(int)
        add     edi, -10
        cmp     edi, 9
        ja      .LBB1_2
        movsxd  rax, edi
        mov     eax, dword ptr [4*rax + .Lswitch.table.bar(int)]
        ret
.LBB1_2:
        mov     eax, 6
        ret
.Lswitch.table.bar(int):
        .long   1                               # 0x1
        .long   3                               # 0x3
        .long   5                               # 0x5
        .long   2                               # 0x2
        .long   6                               # 0x6
        .long   6                               # 0x6
        .long   6                               # 0x6
        .long   6                               # 0x6
        .long   6                               # 0x6
        .long   4                               # 0x4                        
```
</table>

If those optimizations are possible, then it means that the limitations of the <span class="keywordflow">`switch`</span> statement make no sense and never had, given that even 10 years old clang-3.0 could do such optimizations. Clang has shown us that similar optimizations can be applied to both of these statements. However, the <span class="keywordflow">`if`</span> statement can also work with runtime data, whereas the <span class="keywordflow">`switch`</span> statement cannot.

And this situation is unfortunate for me and others. The Internet is filled with questions(over million views) from people who are trying to find an insight or a workaround:
    - [Why strings cannot be used in switch statement?](https://stackoverflow.com/questions/650162/why-the-switch-statement-cannot-be-applied-on-strings)
    - [Evaluate a string with a switch in C++](https://stackoverflow.com/questions/16388510/evaluate-a-string-with-a-switch-in-c)
    - [switch/case statement in C++ with a QString type](https://stackoverflow.com/questions/5452188/switch-case-statement-in-c-with-a-qstring-type)
    - [How to compose complex condition in switch statement?](https://stackoverflow.com/questions/68578/multiple-cases-in-switch-statement)
    - [Multiple conditions in switch case?](https://stackoverflow.com/questions/8644096/multiple-conditions-in-switch-case)
    - [How do I select a range of values in a switch statement?](https://stackoverflow.com/questions/9432226/how-do-i-select-a-range-of-values-in-a-switch-statement/24133078)

Moreover, I can see all over the place many attempts to overcome those limitations in some way or another:

- Compilers such as clang and gcc implemented non-standard extension for matching [case ranges](https://gcc.gnu.org/onlinedocs/gcc/Case-Ranges.html):
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

- LLVM implemented [StringSwitch](https://llvm.org/doxygen/classllvm_1_1StringSwitch.html): 
```cpp
Color color = StringSwitch<Color>(argv[i])
    .Case("red", Red)
    .Case("orange", Orange)
    .Case("yellow", Yellow)
    .Case("green", Green)
    .Case("blue", Blue)
    .Case("indigo", Indigo)
    .Cases("violet", "purple", Violet)
    .Default(UnknownColor);
```
- Apple created the programming language [Swift](https://docs.swift.org/swift-book/LanguageGuide/ControlFlow.html#ID548) with advanced the <span class="keywordflow">`switch`</span> statement:
<table cellspacing="0" cellpadding="0" >
<tr>
    <td>
```cpp
switch num 
{
    case 1..<10:
        print("range [1,10)")
    case 10..<100:
        print("range [10,100)")
    case 100..<1000:
        print("range [100,1000)")
    default:
        print("unknown range")
}
```
<td>
```cpp
switch someCharacter 
{
    case "a", "e", "i", "o", "u":
        print("a vowel")
    case "b", "c", "d", "f", "g", "h", "j",
         "k", "l", "m", "n", "p", "q", "r", 
         "s", "t", "v", "w", "x", "y", "z":
        print("a consonant")
    default:
        print("neither a vowel nor a consonant")
}
```
<td>
```cpp
switch somePoint 
{
    case (0, 0): print("at the origin")
    case (_, 0): print("on the x-axis")
    case (0, _): 
        print("on the y-axis")
    case (-2...2, -2...2):
        print("inside the box")
    default:
        print("outside of the box")
}
```
</table>

Apparently, the <span class="keywordflow">`switch`</span> statement also can be as powerful as the <span class="keywordflow">`if`</span> statement. This insight became my motivation to start this project. Where I tried:
1. Address all the limitations
2. Leave the syntax of `eswitch` as close as possible to the <span class="keywordflow">`switch`</span> statement. Compare:
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
Pretty close, isn't it? Except the places where I've been either limited by the language or intentionally tried to avoid one particular behavior of the <span class="keywordflow">`switch`</span> statement such as default fallthrough.\n
\n
@note In C++ <span class="keywordflow">`switch`</span> statement has explicit <span class="keywordflow">`break`</span> and implicit fallthrough.

\n
This behavior is considered error-prone. Since developers occasionally forget to use 
<span class="keywordflow">`break`</span>. Thus, in my implementation, I reversed this concept i.e. `eswitch` has an implicit <span class="keywordflow">`break`</span> and an explicit fallthrough. Compare:
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
of `eswitch`. I worked so hard in order to be as close as possible to the performance of the <span class="keywordflow">`switch`</span> statement.

### Comparison

--------------------------------------------
Unfortunately, the full comparison of `eswitch` vs the <span class="keywordflow">`switch`</span> statement isn't possible due to the limitations of the latter. Thus I'm going to make a comparison with the <span class="keywordflow">`if`</span> statement instead.

<table>
<tr><th>eswitch<th>if statement
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
<b>Match and withdraw value from `std::any` and `std::variant`</b> - in terms of this  library the usage for matching type in either of<br> those objects is unified, whereas in raw C++ usage is different.
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
    Case( R"(\w*)"_r ) { return "word"; },
    Case( R"(\d*)"_r ) { return "number"; }
);
```
<td>
```cpp
smatch match;

if( regex_match( text, match, regex( R"(\w*)" ) ) )
{  
    return "word";
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
It is a header-only library. On top of that entire library was implemented within a single file, thus you can get that [file](https://github.com/rabdumalikov/eswitch_v5/blob/main/include/eswitch/eswitch_v5.hpp) from the [repository](https://github.com/rabdumalikov/eswitch_v5) and just include it.

\subsection tutorial-license License

--------------------------------------------
This code is distributed under the Boost Software License, Version 1.0. (See
accompanying file [LICENSE.txt](https://github.com/rabdumalikov/eswitch_v5/blob/main/LICENSE.txt) or copy at http://www.boost.org/LICENSE_1_0.txt)


\subsection tutorial-compilers Supported Compilers

--------------------------------------------------------------------------------
Should work on all major compilers which support C++20. I personally tested on the following:

- **clang++-11** (or later)
- **g++-10.2** (or later)
- **Visual Studio 2019** - isn't supported for now, just because [familiar template syntax for generic lambdas](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0428r2.pdf) from C++20 wasn't implemented by _Microsoft_ compiler. I reported about this [issue](https://developercommunity.visualstudio.com/t/keyword-requires-within-if-statement-doesnt-work/1287202), right now it is under investigation.

### Conventions used in this Document

--------------------------------------------
In all code examples, I omit the namespace prefixes for names in the `eswitch_v5` and `std` namespaces.

\section tutorial-implementation-details Implementation Details

--------------------------------------------------------------------------------
This section contains all the details, which user need to know in order to use this library successfully.

### Keywords

--------------------------------------------

| Name | Description |
| :---: | :---: |
| `eswitch` | accepts list of arguments |
| `Case` | accepts condition to check and body next to it will be executed if condition matched |
| `Default` | body next to it will be executed if nothing else matched |
| `fallthrough_` | next body will be executed without checking its condition |
| `any_from` | accepts values to choose from |
| `is<sometype>` | used within `Case` for matching types like `std::any`, `std::variant` and `polymorphic types match` |
| `_r` | user defined literal for `std::regex` |



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
            The list of arguments( i.e. `arg_1, arg_2, ..., arg_n` )
        </div>
<tr>
    <td rowspan="2">
        <span style="color:blue">__conditions__</span>
    <td>
        <div align="left">
        It is a _lazy expression_, where indexes `_1, _2, ...` represent one-to-one<br> correspondance with arguments in `eswitch`. Consider following code:
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
Case( _1 == smth1 && _2 == smth2 && ... )                    (1)
________________________________________________________________

Case( smth1, smth2, ... )                                    (2)
________________________________________________________________

Case( _1 == any_from( smth1, smth2, ... ) )                  (3)
________________________________________________________________

Case( any_from( smth1, smth2, ... ) )                        (4)
________________________________________________________________

Case( ( pred1, _1 ) && ( pred2, _2 ) && ... )                (5)
________________________________________________________________
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
            Optional:<br>
            <span style="color:blue">__param__</span>
        </div>
    <td>
        <div align="left">Correspond to withdrawn value from: `std::any`, `std::variant`,<br> 
            `polymorphic match` or `std::regex` match. But also it corresponds to the<br>
            returned value wrapped into `std::optional` from custom extensions.<br>
            @note
            Keyword <span class="keyword">`auto`</span> here is forbidden,<br>
            i.e. type of <span style="color:blue">__param__</span> should be specified explicitly.
        </div>
<tr>
    <td>
        <div align="center">
            Optional:<br>
            <span style="color:blue">__options__</span>
        </div>
    <td>
<table>
<tr>
    <td>-<td>following `Case`'s won't be executed
<tr>
    <td>fallthrough_<td>execute body of the following `Case`
<tr>
    <td>likely_<td>messing with branch predictions<br>
    ( will be introduced in future )
</table>
</table>

## How to write Custom extensions?

--------------------------------------------

This guide deals with how to define the custom `Case` behavior for any types. We will implement a custom extension and I walk you through all steps and details.

@note
The only things which we can be customized are all the comparison `operators` such as `==`, `!=`, `>`, `<` and so on.

**Let's begin:**

As you know, comparing floating points is a tricky, for example, the following code fails due to inaccurate precision:
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
So the standard way of comparing floating points doesn't work properly. On top of that, simple overloading of `operator==` for primitives is [forbidden](https://eel.is/c++draft/over.oper#general-7).

```cpp
bool operator==( const double d1, const double d1 )
{
    ...
}
/// Compilation ERROR: overloaded 'operator==' must have at least one parameter of class or enumeration type
```
To overcome this, we must use an intermediate class that will hold our value. Like this: 
```cpp
struct double_value
{
    double value;
};
```
Then we can use `double_value` in our overloaded `operator==` :
```cpp
bool operator==( const double d1, const double_value d2 )
{
    return fabs( d1 - d2.value ) < __FLT_EPSILON__;
}
```
Now code below will work as we desired since using `double_value` allows for compiler through [name lookup](https://en.cppreference.com/w/cpp/language/lookup) to find our custom `operator==`.
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
- An intermediate class should be used all the time (not only for primitives due to compiler restrictions), otherwise it won't be possible for compiler to find custom `operator` (unless this `operator` will be defined before `#include <eswitch_v5.hpp>`, only then intermediate class won't be needed).

- Also the example below demonstrate if a certain value should be returned from custom `operator` and transferred to `Case` as [input parameter](#syntax), then the value should be wrapped into `std::optional`. 

**Example:** [Value and Type transferring](@ref example-value-and-type-transferring)

## Rationalities

--------------------------------------------

- Using macroses for `Case` and `Default` allowed me to be as close as possible to the <span class="keywordflow">`switch`</span> statement regarding syntax, otherwise there was no way to hide verbose lambda declaration. 
- **I can use compile-time hashing in order to use string within the <span class="keywordflow">`switch`</span> statement**. Yes you can. But there are several downsides. First of all, you need to implement it yourself since there is no standard implementation. Second of all, you need to deal with possible collisions. After all those steps you will solve just one limitation, it still won't be as powerful as the <span class="keywordflow">`if`</span> statement, nor my implementation. Which is much more advance since it allows:
    - match for any types, 
    - compose complex conditions, 
    - write extensions and 
    - `eswitch` can work at compile-time as long as the provided data known at compile-time, but there is one catch, now it is not working and the reason is that `std::reference_wrapper` isn't <span class="keyword">`constexpr`</span> althought it must be since C++20.