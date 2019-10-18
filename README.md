__________
Motivation:
----------
**eswitch_v4** library implements extension for **native switch**.<br/>
**eswitch_v4** based on several crutial concepts of implementation **switch** in **Swift**.<br/>
i.e. **break** is *implicit*, while **fallthrough** is *explicit*.<br/>

Main motivation for **eswitch_v4** was to overcome **native switch** limitations:

- one parameter per **native switch**
- the parameter restricted to only primitive types( **int**, **char**, **enum** ... ).

**eswitch** supports any number of _parameters_ and almost without restriction on their _type_,<br/>
 except that _type_ has to be **comparable**( i.e. must have **operator==** ).
__________
Example:
-------
```

    int Val = 10;

    switch (Val % 100) {                     
    case 11:                       
    case 12:                         
    case 13:
        return "th";
    default:
    switch (Val % 10) {
        case 1: return "st";
        case 2: return "nd";
        case 3: return "rd";
        default: return "th";
    }
    }

    using namespace eswitch_v4;

    eswitch( Val % 100, Val % 10 )  >>
        case_( _1 == any_from( 11, 12, 13 ) ) >> []{ return "th"; } >>
        case_( _2 == 1 ) >> []{ return "st"; } >>
        case_( _2 == 2 ) >> []{ return "nd"; } >>
        case_( _2 == 3 ) >> []{ return "rd"; } >>
        default_         >> []{ return "th"; };
```
_______________

Minimum C++ standard:
-------------------
C++11
_______________

Supported Compilers:
-------------------
Should work on all major compilers which support **C++11**.<br/>
I personally tested on following:

- **clang** 8 (or later)
- **GCC** 6.3.0 (or later)
- **Visual Studio** **2017** and **2019** (or later).

_______________
Performance:
------------

**TOOL TO MEASURE:** google benchmark

**FILE:** */benchmarks/eswitch_benchmark.cpp*

**COMPILERS:** clang-8, gcc-6.3.0 and visual studio 2019.

**BUILD FLAGS:**

- **clang-8** and **gcc-6.3.0** => **"-O3"**
- **visual studio 2019 **=> **"/Ox"**.
	
**RESULTS:**

- **clang-8**   - no performance difference. 
- **gcc-6.3.0** - no performance difference.
- **visual studio 2019** - *slower*, but **not critical**.
_______________

How to build:
------------
cmake ..  -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++-6 -DCMAKE_CROSSCOMPILING=1 -DRUN_HAVE_STD_REGEX=0 -DRUN_HAVE_POSIX_REGEX=0<br/>
make
_______________

License:
--------
Boost Software License( Version 1.0 )

_______________

