__________
Motivation:
----------
**eswitch** library implements extension for **native switch**.<br/>
Main motivation for **eswitch** was to overcome **native switch** limitations:

- one parameter per **native switch**
- the parameter restricted to only primitive types( **int**, **char**, **enum** ... ).

**eswitch** supports any number of _parameters_ and almost without restriction on their _type_,<br/>
 except that _type_ has to be **comparable**( i.e. must have **operator==** ).
__________
Example:
-------
```
    enum EConnectionStatus { CONNECTING, CONNECTED, DISCONNECTED, FAILED };

    EConnectionStatus connectionStatus = ...;
    bool clientAlive = true;
    std::string http_status_line = "...";
    
    E_SWITCH( connectionStatus, clientAlive, http_status_line )
        CASE( CONNECTING) {...} BREAK_
        CASE( CONNECTED AND true AND "HTTP 200 OK" ) { ... } BREAK_
        CASE( CONNECTED OR false OR "HTTP 404 Not Found" ) { ... } BREAK_
        CASE( ANY_FROM( DISCONNECTED, FAILED ) ) {...}
        DEFAULT_ {...}
    END_;

    OR =====================================================================>

    std::unique_ptr< Client > client;

    E_SWITCH( connectionStatus, client, http_status_line )
        CASE( CONNECTING) {...} BREAK_
        CASE( CONNECTED AND NOT(nullptr) AND "HTTP 200 OK" ) { ... } BREAK_
        CASE( CONNECTED AND NOT(nullptr) AND "HTTP 404 Not Found" ) { ... } BREAK_
        CASE( ANY_FROM( DISCONNECTED, FAILED ) ) {...}
        DEFAULT_ {...}
    END_;
```
__________
Comparison:<br/>
----------
&nbsp;**===============================================================================**                                               

|          NATIVE SWITCH                  |             EXTENDED SWITCH                                          |
|-----------------------------------------|:---------------------------------------------------------------------|
|** =============================  **     | **=================================================**                                               
|**switch**( *somevalue* )                |          **E_SWITCH**( *somevalue* )				           
|{                                        |               &nbsp;&nbsp;&nbsp;&nbsp;**CASE**( CONNECTING) ... **BREAK_**	    
|    **case** CONNECTING: ... **break**;  |               &nbsp;&nbsp;&nbsp;&nbsp;**CASE**( CONNECTED ) ... **BREAK_**	    
|    **case** CONNECTED: ... **break**;   |               &nbsp;&nbsp;&nbsp;&nbsp;**CASE**( DISCONNECTED ) ... **BREAK_**  
|    **case** DISCONNECTED: ... **break**;|               &nbsp;&nbsp;&nbsp;&nbsp;**CASE**( FAILED ) ... **BREAK_**		  
|    **case** FAILED: ... **break**;      |               &nbsp;&nbsp;&nbsp;&nbsp;**DEFAULT_** ...				      
|    **default**: ...;                    |           **END_**;								  
|};                                       |												  
|                                         |												 
|** =============================  **     | **=================================================**                                    
|                                         |          **E_SWITCH**( *somevalue* )
|**switch**( *somevalue* )                |              **CASE**( CONNECTING OR CONNECTED ) **BREAK_**
|{                                        |              **CASE**( DISCONNECTED OR FAILED ) **BREAK_**
|    **case** CONNECTING:                 |          **END_**;
|    **case** CONNECTED:                  |              
|        **break**;                       |          ==========**OR**==========
|    **case** DISCONNECTED:               |
|    **case** FAILED:                     |          **E_SWITCH**( *somevalue* )
|        **break**;                       |              **CASE**( **ANY_FROM**( CONNECTING, CONNECTED ) ) **BREAK_**
|};                                       |              **CASE**( **ANY_FROM**( DISCONNECTED, FAILED ) ) **BREAK_**
|                                         |          **END_**;
|                                         |
|** =============================  **     | **=================================================**
|                                         |                                                                      |
|**switch**( *somevalue* )                |          **E_SWITCH**( *somevalue* )
|{                                        |              **CASE**( CONNECTING )
|    **case** CONNECTING:                 |              {
|    {                                    |                  &nbsp;&nbsp;&nbsp;&nbsp;//code...
|&nbsp;&nbsp;&nbsp;&nbsp;//code...        |              } 
|    }                                    |              **CASE**( CONNECTED ) 
|    **case** CONNECTED:                  |              {
|    {                                    |                  &nbsp;&nbsp;&nbsp;&nbsp;//code...
|&nbsp;&nbsp;&nbsp;&nbsp;//code...        |              } 
|    }                                    |          **END_**;
|};                                       |         

&nbsp;**===============================================================================**                                               
<br>
_______________


E_SWITCH details:
----------------

* **E_SWITCH** must be closed by macro **END_**, otherwise code won't compile:

```
    E_SWITCH(...)  CASE( ... ){} END_;  /// OK
    E_SWITCH(...) CASE( ... ){};        /// ERROR
```

* **BREAK_** and **DEFAULT_** have identical meaning as for **native switch**:

* **CASE** conditions could be build via operators **AND**/**OR** + <br/>helper functions: **ANY_FROM( ... )** and **NOT( *value* )**:
```
    // - AND/OR
    E_SWITCH( param1, param2 )
        CASE( true AND CONNECTED ) ... BREAK_
        CASE( false OR FAILED ) ... BREAK_

    // - ANY_FROM(...)
    E_SWITCH( param1, param2 )
        CASE( true AND ANY_FROM( CONNECTING, CONNECTED ) ){...}

    // - NOT( ... )
    E_SWITCH( param )
        CASE( NOT( nullptr ) ) {...}
```
* **MIXING** of operators **AND**/**OR** per **CASE** is not supported, will yield compilation error:
```
    E_SWITCH( param1, param2, param3 )
        CASE( true AND CONNECTED OR NOT(nullptr) ) ... BREAK_  /// Error
        CASE( true AND CONNECTED AND NOT(nullptr) ) ... BREAK_ /// OK
        CASE( true OR CONNECTED OR NOT(nullptr) ) ... BREAK_   /// OK
```
* Each _param_ in the **E_SWITCH** is going to be matched only against<br/> 
   corresponding _param_ in the **CASE** regardless operators you use:
```
    E_SWITCH(  param1,     param2,   ...,   param_n)
                 ^           ^                 ^
                 |           |                 |                
        CASE( cparam1 OR  cparam2 OR ... OR cparam_n )
    END_;

    /// C++ equivalent code
    if( param1 == cparam1 || param2 == cparam2 || param_n == cparam_n) ...
```
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
**EXCEPTION:** <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
if you have only one _parameter_ in **E_SWITCH** and many _parameters_ in **CASE** via **OR**,<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
then matching will be performed sequencially until first match is found.
```
    E_SWITCH( param )
        CASE( cparam1 OR cparam2 OR ... OR cparam_n ){...}
    END_;
    
    /// C++ equivalent code
    i.e. if( param == cparam1 || param == cparam2 || param == ... || param == cparam_n )
```

* **CASE** - supports *partial matching* of *parameters*:
```
    E_SWITCH( param1, param2, ..., param_n )
        CASE( param1 ){} BREAK_
        CASE( param1 OR param2 ){} BREAK_
        CASE( param1 AND param2 ){} BREAK_
        CASE( param1 OR param2 OR ... OR param_n ){} BREAK_
    END_;
```

* **return** statement within **CASE**:
     - **WILL**  interrupt execution of the current matched **CASE**, but
     - **WON'T** interrupt execution of another **CASE's**
```
        E_SWITCH( param1, param2, ..., param_n )
           CASE( param1 )
           {
               return; /// interrupt current CASE
    
               value = ...;
           }
           CASE( param1 AND param2 )
           {
               /// This case will be executed
           }
        END_;
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

