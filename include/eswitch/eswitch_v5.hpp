//  Copyright (c) 2019-2020 Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tuple>
#include <array>
#include <cassert>
#include <type_traits>
#include <optional>
#include <concepts>
#include <any>
#include <variant>
#include <regex>

/// TODO
// move Default case to the END - DONE
// lazy_eswitch - DECLINED( since it doesn't have sence for now )
// tuple+pair handle - DONE
// write concept Comparable( or maybe stl has one ) - DONE
// Willcard - DONE
// Add ERROR if type are incomparable - DONE
// Support: Case( "hello", 1, _3 != nullptr ) - DONE
// Support: Case( std::regex( "" ) )
/*

    int i = 1;
    std::string str{ "hello" };

    eswithc( i, str )
    (
        Case( 1, "Hello" ){ return ""; },
        Default{ return "fail"; }
    );

    j = true;
    eswithc( i, str )
    (
        Case( 1, "Hello", _3 != true ){ return ""; },
        Default{ return "fail"; }
    );

    auto swtch = 
        lazy_eswitch (
            Case( _1 == 1 && _2 == 0 && _3 == 1 )
        );
  
    for_each( vec, lazy_switch( 
        Case( ( is_odd, _1 ) )( int val ){ count++; }
    ) );

    for_each( vec, eswitch( 
        Case( ( is_odd ) ) ( int val ){ odd_count++; },
        Case( ( is_even ) )( int val ){ even_count++; },
        );

    for_each( vec, []( const auto & val )
    {
        if( is_odd( val ) ) ++odd_count;
        else if( is_even( val ) ) ++even_count;
    } );

    ( vec | range::filter( is_odd  ) ).size();
    ( vec | range::filter( is_even ) ).size();
    
   std::tuple< int, int, int > tup;
   eswitch( tup )
   (
       Case( _1 == 1 && _2 == 0 && _3 == 1 )
   );


   eswitch( pair )
   (
       Case( _1 == true && _2 == "buu" )
   );

    auto [f,s] = some_pair;
    if( f == true && s == "buu" )
 */

namespace eswitch_v5
{
    template< typename T >
    concept Index = requires( T ){ std::decay_t< T >::eswitch_index; };

    template< typename T >
    concept Condition = requires( T t )
    { 
        t( std::make_tuple() ); 
        std::decay_t< T >::template is_out_of_range< std::size_t{} >(); 
    };

    template< typename TPred, std::size_t ... Is >
    class predicate_condition;

    template< Index TIndex, typename T >
    class condition;

    enum class Logical_operators{ and_, or_ };
    enum class Comparison_operators{ equal_, not_equal_ };

    namespace extension
    {
        class Range
        {
            std::size_t start_;
            std::size_t end_;

            public:
            constexpr Range( const std::size_t start, const std::size_t end ) 
                : start_( start ), end_( end )
                {                
                }

            friend constexpr bool operator==( const std::size_t val, const Range & rm )
            {
                return val >= rm.start_ && val <= rm.end_;
            }
        };

        template< typename T, typename TArray >
        constexpr bool is_in_set( const T & to_check, const TArray & arr )
        {
            return std::find( std::begin( arr ), std::end( arr ), to_check ) != std::end( arr );
        }

        // any
        struct any 
        {
            template < typename T >
            friend constexpr bool operator==( const T& value, const any& st ) {
                return true;
            }

            template < typename T >
            friend constexpr bool operator!=( const T& value, const any& st ) {
                return false;
            }
        };

        template< typename T, std::size_t Sz >
        struct Any_from_impl
        {
            std::array< T, Sz > anythings;

            template< typename ... Args >
            constexpr Any_from_impl( Args &&... args )
                : anythings{ std::forward< Args >( args )... }
                {
                }

            template< typename T_ >
            friend constexpr bool operator==( const T_ & value, const Any_from_impl& st ) 
            {
                return is_in_set( value, st.anythings );
            }
        };        

        template< typename ... Args >
        Any_from_impl( Args &&... ) -> Any_from_impl< std::common_type_t< std::decay_t< Args >... >, sizeof...( Args ) >;        
    }


    template< std::size_t Idx >
    struct Index_
    { 
        static constexpr std::size_t eswitch_index = Idx;

        auto in( const std::size_t start, const std::size_t end ) const
        {
            return condition< Index_< Idx >, extension::Range >( Comparison_operators::equal_, extension::Range( start, end ) );
        }
    };

    const Index_< 0 > _1;    const Index_< 10 > _11;    const Index_< 20 > _21;  
    const Index_< 1 > _2;    const Index_< 11 > _12;    const Index_< 21 > _22;
    const Index_< 2 > _3;    const Index_< 12 > _13;    const Index_< 22 > _23;
    const Index_< 3 > _4;    const Index_< 13 > _14;    const Index_< 23 > _24;
    const Index_< 4 > _5;    const Index_< 14 > _15;    const Index_< 24 > _25;
    const Index_< 5 > _6;    const Index_< 15 > _16;    const Index_< 25 > _26;
    const Index_< 6 > _7;    const Index_< 16 > _17;    const Index_< 26 > _27;
    const Index_< 7 > _8;    const Index_< 17 > _18;    const Index_< 27 > _28;
    const Index_< 8 > _9;    const Index_< 18 > _19;    const Index_< 28 > _29;
    const Index_< 9 > _10;   const Index_< 19 > _20;    const Index_< 29 > _30;           
         
    namespace details 
    {        
        template< typename T >
        struct is_default_case : std::false_type {};

        template< Index I >
        struct is_default_case< condition< I, extension::any > > : std::true_type {};

        template< typename T >
        constexpr bool is_default_case_v = is_default_case< T >::value;

        template< typename T >
        constexpr std::false_type is_std_variant( T &&  );

        template< typename ... Ts >
        constexpr std::true_type  is_std_variant( std::variant< Ts... > && );

        template< typename T >
        constexpr bool is_std_variant_v = decltype( is_std_variant( std::declval< std::decay_t< T > >() ) )();
        
        template< typename T >
        struct is_std_pair : std::false_type {};
        
        template< template< typename T1, typename T2 > class Pair, typename T1, typename T2 >
        struct is_std_pair< Pair< T1, T2 > > :
            std::conditional_t< std::is_same_v< Pair< T1, T2 >, std::pair< T1, T2 > >, 
                std::true_type, std::false_type >
         {};

        template< typename T >
        constexpr bool is_std_pair_v = is_std_pair< std::decay_t< T > >::value;

        template< typename T >
        struct is_std_tuple : std::false_type {};
        
        template< template< typename ... Ts > class Tuple, typename ... Ts >
        struct is_std_tuple< Tuple< Ts... > > :
            std::conditional_t< std::is_same_v< Tuple< Ts... >, std::tuple< Ts... > >, 
                std::true_type, std::false_type >
         {};

        template< typename T >
        constexpr bool is_std_tuple_v = is_std_tuple< std::decay_t< T > >::value;

        template< typename T >
        constexpr bool is_std_any_v = std::is_same_v< std::decay_t< T >, std::any >;

        template< typename T >
        struct is_callable_impl : std::false_type {};

        template< typename R, typename C, typename ... Args >
        struct is_callable_impl< R(C::*)(Args...) const > : std::true_type {};

        template< typename R, typename C, typename ... Args >
        struct is_callable_impl< R(C::*)(Args...) > : std::true_type {};

        template< typename T >
        constexpr bool is_callable_v = is_callable_impl< decltype( &T::operator() ) >::value;

        static constexpr bool unreachable() 
        { 
            if( !std::is_constant_evaluated() ) assert( false ); 

            return false; 
        }

        template< typename T >
        struct is_predicate : std::false_type {};

        template< template< typename, uint32_t ... > class C, typename T, uint32_t ... Is >
        struct is_predicate< C< T, Is... > >
            : std::conditional_t< 
                std::is_same_v< C<T, Is... >, predicate_condition< T, Is... > >, 
                std::true_type, std::false_type > 
        {            
        };

        template< typename T >
        constexpr bool is_predicate_v = is_predicate< T >::value;

        template< typename T >
        struct amount_args_function_has
        {
        };

        template< typename R, typename C, typename ... Args >
        struct amount_args_function_has< R(C::*)(Args...) const >
        {
            static constexpr std::size_t value = sizeof...( Args );
            using type = R;
        };

        template< typename R, typename C, typename ... Args >
        struct amount_args_function_has< R(C::*)(Args...) >
        {
            static constexpr std::size_t value = sizeof...( Args );
            using type = R;
        };

        template< typename T, typename = void >
        struct amount_args{};

        template< typename T >
        struct amount_args< T, std::void_t< decltype( &T::operator() ) > >
        { 
            constexpr static auto value = amount_args_function_has< decltype( &T::operator() ) >::value;
        };

        template< typename T >
        constexpr bool amount_args_v = amount_args< T >::value;

        template< typename T, typename = void >
        struct invoke_result {};
        
        template< typename T >
        struct invoke_result< T, std::void_t< decltype( &T::operator() ) > >
        {
            using type = typename amount_args_function_has< decltype( &T::operator() ) >::type;
        };

        template< typename T >
        using invoke_result_t = typename invoke_result< T >::type;

        template< typename ... Ts >
        struct Always_false
        {
            static constexpr bool value = false;
        };

        template< typename ... TupleCnds, typename Cnd, typename ... Cnds >
        constexpr auto move_default_case_to_the_end_impl( std::tuple< TupleCnds... > && tup, Cnd && cnd, Cnds &&...cnds )
        {
            if constexpr( is_default_case_v< decltype( std::declval< Cnd >().cnd ) > )
            {
                return std::tuple_cat( std::move( tup ), 
                    std::make_tuple( 
                        std::forward< Cnds >( cnds )..., 
                        std::forward< Cnd >( cnd ) ) );
            }
            else if constexpr( sizeof...( Cnds ) > 0 )
            {
                return move_default_case_to_the_end_impl( 
                    std::tuple_cat( std::move( tup ), std::make_tuple( std::forward< Cnd >( cnd ) ) ), 
                    std::forward< Cnds >( cnds )... );
            }
            else
            {
                return std::tuple_cat( std::move( tup ), std::make_tuple( std::forward< Cnd >( cnd ) ) );
            }                        
        }

        template< typename ... Cnds >
        constexpr auto move_default_case_to_the_end( Cnds &&... cnds )
        {
            return move_default_case_to_the_end_impl( std::make_tuple(),
                std::forward< Cnds >( cnds )... );
        }

        template< int ... Is, typename Tup1, typename Tup2 >
        constexpr auto tuple_combine( Tup1 && tup1, Tup2 && tup2 )
        {
            auto combine = []< typename T1, typename T2 >( T1 && t1, T2 && t2 )
            {
                if constexpr( Condition< T2 > && Index< T1 > )
                {
                    static_assert( T1::eswitch_index == T2::idx::eswitch_index, "Wrong Index!" );
                    return std::forward< T2 >( t2 );
                }
                else
                    return std::forward< T1 >( t1 ) == std::forward< T2 >( t2 );
            };
            
            return std::make_tuple( combine( 
                std::get< Is >( std::move( tup1 ) ), 
                std::get< Is >( std::move( tup2 ) ) 
                )... );
        }
    } // namespace details
    
    template< typename T >
    concept has_type = requires{ typename T::type; };

    template< typename T >
    concept has_value = requires{ T::value; };

    template< typename T >
    concept ComparableExceptAnyAndVariant = 
        !details::is_std_any_v< T > || 
        !details::is_std_variant_v< T > || 
        requires( T a, T b ) {
        { a == b };
        { a != b };
    };

    template< typename T, typename U >
    concept Comparable = 
        requires( T a, U b ) {
        { a == b };
        { a != b };
    };

    template< typename T >
    concept ReturnValueNoneVoid = std::is_same_v< 
        std::invoke_result_t< std::remove_reference_t< T > >,
        void 
    >;
    template< typename T >
    concept IsCndPredicate = details::is_predicate_v< T >;

    template< typename T >
    concept Callable = details::is_callable_v< T >;

    template< typename T >
    concept StdTuple = details::is_std_tuple_v< T >;

    template< typename T >
    concept StdPair = details::is_std_pair_v< T >;

    template< typename T >
    concept IsRegexCondition = details::is_default_case_v< std::decay_t< T > > || ( Condition< T > && std::is_same_v< typename std::decay_t< T >::value_type, std::regex > );

    template< Index TIndex >
    struct regex_support
    {        
        template<  typename T, StdTuple TSrcTuple >
        static auto execute( Comparison_operators, const T & value_, const TSrcTuple & src_tuple ) 
            requires std::is_same_v< std::decay_t< T >, std::regex > &&
                ( std::tuple_size_v< std::decay_t< TSrcTuple > > >= 1 )
        {
            const auto & text( std::get< TIndex::eswitch_index >( src_tuple ) );

            if( std::smatch match; std::regex_match( text, match, value_ ) )
            {              
                std::vector< std::string > vs;
                vs.reserve( match.size() );

                for( const auto & v : match )  
                    vs.push_back( v );

                return std::make_optional( std::move( vs ) );
            }   

            return std::optional< std::vector< std::string > >{};
        }
    };

    template< Index TIndex >
    struct Any_and_Variant_support
    {
        template< typename T, typename T1, typename ... Ts >
        static constexpr auto execute( Comparison_operators, const T & value_, const std::tuple< T1, Ts... > & src_tuple )
            requires has_type< T > && 
                ( details::is_std_any_v<     decltype( std::get< TIndex::eswitch_index >( src_tuple ) ) > || 
                  details::is_std_variant_v< decltype( std::get< TIndex::eswitch_index >( src_tuple ) ) > )
        {
            auto entry = std::get< TIndex::eswitch_index >( src_tuple );

            using type = typename T::type;
            using _T = decltype( std::get< TIndex::eswitch_index >( src_tuple ) );

            if constexpr( details::is_std_any_v< _T > )
            {
                if( auto * val = std::any_cast< type >( &entry ) )
                {
                    return std::make_optional( *val );
                }
            }
            else if constexpr( details::is_std_variant_v< _T > ) 
            {
                if( auto * val = std::get_if< type >( &entry ) )
                {
                    return std::make_optional( *val );
                }
            }
                        
            return std::optional< type >{};
        }
    };

    template< Index TIndex, typename T >
    class condition 
        : public regex_support< TIndex >
        , public Any_and_Variant_support< TIndex >
    {
        Comparison_operators cmp_operator;        
        T value_;

        template< Index I, typename T_ >
        friend class condition;
        
    public:

        using value_type = T;
        using idx = TIndex;

        template< typename Arg >
        constexpr condition( const Comparison_operators cmp_type, Arg && value ) 
            : cmp_operator( cmp_type ), value_( std::forward< Arg >( value ) )
            { 
            }

        template< StdTuple TSrcTuple >
        constexpr auto operator()( const TSrcTuple & src_tuple ) const
        {
            static_assert( !is_out_of_range< std::tuple_size_v< TSrcTuple > >(), 
                    "Case Index is OUT OF RANGE" ); 

            return execute( cmp_operator, value_, src_tuple );
        }

        template< StdTuple TSrcTuple >
        constexpr bool operator()( const TSrcTuple & src_tuple ) const
            requires ( std::tuple_size_v< std::decay_t< TSrcTuple > > == 0 )
        {
            return false;
        }

        template< std::size_t MaxIndex >
        static constexpr bool is_out_of_range() 
        { 
            return TIndex::eswitch_index >= MaxIndex; 
        }

    private:
    
        using regex_support< TIndex >::execute;
        using Any_and_Variant_support< TIndex >::execute;

        template< typename T_, StdTuple TSrcTuple >
        static constexpr bool execute( Comparison_operators cmp_operator, const T_& value_, const TSrcTuple & src_tuple )
        {
            return compare( cmp_operator, std::get< TIndex::eswitch_index >( src_tuple ), value_ );         
        }

        template< typename T1, typename T2 >
        static constexpr bool compare( const Comparison_operators CmpOper, T1 && t1, T2 && t2 ) 
            requires Comparable< T1, T2 >
        {                            
            switch( CmpOper )
            {
                case Comparison_operators::equal_:
                    return t1 == t2;
                case Comparison_operators::not_equal_:
                    return !( t1 == t2 );
                default:
                    return details::unreachable();
            };
        }

        template< typename T1, typename T2 >
        static constexpr bool compare( const Comparison_operators CmpOper, T1 && t1, T2 && t2 ) 
        {                
            static_assert( Comparable< T1, T2 >, "Types are not COMPARABLE!" );
            
            return false;
        }
    };

    template< Condition TCnd1, Condition TCnd2 >
    class conditions
    {
        Logical_operators logical_operator;
        TCnd1 cnd1_;
        TCnd2 cnd2_;

    public:

        template< Condition Cnd1, Condition Cnd2 >
        constexpr conditions( const Logical_operators logical_operator, Cnd1 && cnd1, Cnd2 && cnd2 ) 
            : logical_operator( logical_operator )
            , cnd1_( std::forward< Cnd1 >( cnd1 ) )
            , cnd2_( std::forward< Cnd2 >( cnd2 ) )
            {                
            }

        template< std::size_t MaxIndex >
        static constexpr bool is_out_of_range() 
        {
            return TCnd1::template is_out_of_range< MaxIndex >() || 
                   TCnd2::template is_out_of_range< MaxIndex >();
        }
     
        template< StdTuple TSrcTuple >
        constexpr bool operator()( const TSrcTuple & src_tuple ) const
        {
            return compare( logical_operator, 
                static_cast< bool >( cnd1_( src_tuple ) ),  
                static_cast< bool >( cnd2_( src_tuple ) ) );            
        }

    private:

        static constexpr bool compare( const Logical_operators LogicalOperator, const bool t1, const bool t2 )
        {                
            switch( LogicalOperator )
            {
            case Logical_operators::and_:
                return t1 && t2;
            case Logical_operators::or_:
                return t1 || t2;
            default: 
                return details::unreachable();
            };
        }
    };

    template< Condition Cnd1, Condition Cnd2 >
    conditions( Logical_operators, Cnd1, Cnd2 ) -> conditions< Cnd1, Cnd2 >;

    template< typename T1, Condition Cnd >
    constexpr auto operator&&( T1 && i, Cnd && cnd )
    {
        return conditions( Logical_operators::and_, std::forward< T1 >( i ), std::move( cnd ) );
    }

    template< typename T1, Condition Cnd >
    constexpr auto operator||( T1 && i, Cnd && cnd )
    {
        return conditions( Logical_operators::or_, std::forward< T1 >( i ), std::move( cnd ) );
    }

    template< Index Idx, typename T >
    constexpr auto operator==( Idx idx, T && rhv )
    {
        return condition< Idx, T >( Comparison_operators::equal_, std::forward< T >( rhv ) );
    }

    template< Index Idx, typename T >
    constexpr auto operator!=( Idx idx, T && rhv )
    {
        return condition< Idx, T >( Comparison_operators::not_equal_, std::forward< T >( rhv ) );
    }

    template< typename ... Args >
    class eswitch_impl
    {
        static_assert( ( ComparableExceptAnyAndVariant<Args> && ... ), "Input Types should be COMPARABLE!" );

        std::tuple< Args... > tup_;
    public:

        template< typename ... Ts >
        constexpr eswitch_impl( Ts &&... ts ) : tup_{ std::forward< Ts >( ts )... }
        {            
        }

        template< typename T >
        using underlying = details::invoke_result< typename T::F >;

        template< typename T >
        using underlying_t = details::invoke_result_t< typename T::F >;

        template< typename T >
        using args_t = details::amount_args< typename T::F >;

        template< typename ... Cnds >
        constexpr auto operator()( Cnds && ... cnds )
        {
            if constexpr( ( !has_value< args_t< Cnds > > || ... ) )
            {
                static_assert( !( !has_value< args_t< Cnds > > || ... ), 
                    "Predicate with 'auto' argument aren't ALLOWED!" );
            }        
            else if constexpr( ( has_type< underlying< Cnds > > && ... ) )
            {
                static_assert( has_type< std::common_type< underlying_t< Cnds >... > >, 
                    "Inconsistent 'Return type'!" );
            }
        }

        struct Padding {};

        template< typename ... Cnds >
        constexpr auto operator()( Cnds && ... cnds ) requires has_type< std::common_type< underlying_t< Cnds >... > >
        {
            constexpr auto generic_lambda = []< typename T, T... ints >
                ( std::index_sequence< ints... > && int_seq, auto && tup, auto && f )
                {
                    ( f( std::get< ints >( tup ) ), ... );
                };

            using return_t = std::common_type_t< underlying_t< Cnds >... >;

            constexpr auto has_return_value = !std::is_same_v< return_t, void >;

            std::optional< std::conditional_t< has_return_value, return_t, Padding >
                > return_value;

            generic_lambda( 
                std::make_index_sequence< sizeof...( Cnds ) >{}, 
                details::move_default_case_to_the_end( std::forward< Cnds >( cnds )... ), 
                [ this, &return_value, break_ = false ]( const auto & cnd ) mutable
                {
                    using namespace details;

                    if( break_ ) return;

                    constexpr auto amount_args = amount_args_v< decltype(cnd.func) >;

                    auto res = cnd.cnd( tup_ );

                    if( !res ) return;

                    if constexpr( amount_args == 1 && !std::is_same_v< decltype( res ), bool > )
                    {                  
                        if constexpr( has_return_value )
                        {
                            return_value = static_cast< return_t >( cnd.func( *res ) );                            
                        }
                        else
                        {
                            cnd.func( *res );                            
                        }       
                    }
                    else if constexpr( amount_args == 0 )
                    {
                        if constexpr( has_return_value )
                        {
                            return_value = static_cast< return_t >( cnd.func() );
                        }
                        else
                        { 
                            cnd.func();
                        }
                    }
                    else
                    {
                        details::unreachable();
                    }                                               

                    break_ = !cnd.fallthrough;
                });

            if constexpr( has_return_value ) 
            {
                return return_value.value();
            }
        }
    };

    template< typename ... Ts >
    eswitch_impl( Ts... ) -> eswitch_impl< Ts... >;

    template< typename ... Ts >
    constexpr auto eswitch( Ts && ... ts )
    {
        return eswitch_impl( std::forward< Ts >( ts )... );
    }

    template< StdPair T >
    constexpr auto eswitch( T && pair )
    {
        if constexpr( std::is_rvalue_reference_v< T&& > )
        {
            return eswitch_impl( std::move( pair.first ), std::move( pair.second ) );
        }
        else
        {
            return eswitch_impl( pair.first, pair.second );
        }
    }

    template< StdTuple T >
    constexpr auto eswitch( T && tup )
    {
        auto expand_tuple = [] < typename _T, _T ... ints >
            ( std::index_sequence< ints... > &&, const auto & tup )
        {
            return eswitch_impl( std::get< ints >( tup )... );
        };

        return expand_tuple( std::make_index_sequence< std::tuple_size_v< std::decay_t< T > > >{}, tup );
    }

    template< Condition Cnd, Callable Func >
    struct condition_with_predicate
    {
        using F = Func;

        Cnd  cnd;
        Func func;
        bool fallthrough = false;
    };

    template< typename T, typename F >
    condition_with_predicate( T, F ) -> condition_with_predicate< T, F >;

    template< Condition T, Callable Func >
    constexpr auto operator%( T && cnd, Func && f )
    {
        return condition_with_predicate{ std::move( cnd ), std::move( f ) };
    }
    
    struct Fallthrough {};

    template< typename Cnd, ReturnValueNoneVoid Func >
    constexpr auto operator^( condition_with_predicate< Cnd, Func >&& cp, const Fallthrough & )
    {
        cp.fallthrough = true;

        return std::move( cp );
    }

    template< typename TPred, std::size_t ... Is >
    class predicate_condition
    {
        TPred pred_;

        public:

        template< typename T >
        constexpr predicate_condition( T && pred ) 
            : pred_( std::forward< T >( pred ) )
            {                    
            }

        template< typename TSrcTuple >
        constexpr bool operator()( const TSrcTuple & src_tuple ) const
        {
            static_assert( !is_out_of_range< std::tuple_size_v< TSrcTuple > >(), 
                "Case Index is OUT OF RANGE" ); 

            return pred_( std::get< Is >( src_tuple )... );
        }

        template< std::size_t MaxIndex >
        static constexpr bool is_out_of_range()
        {
            constexpr std::array< bool, sizeof...( Is ) > list{ ( Is >= MaxIndex )... };

            return std::find( std::begin( list ), std::end( list ), true ) != std::end( list );
        }
    };

    template< typename R, typename... Args, Index Idx >
    constexpr auto operator,( R(*pred)(Args...), Idx )
    {
        return predicate_condition< R(*)(Args...), Idx::eswitch_index >( pred );
    }

    template< typename Pred, Index Idx >
        requires ( details::is_predicate_v< std::decay_t< Pred > > == false )
    constexpr auto operator,( Pred && pred, Idx ) 
    {
        return predicate_condition< std::remove_reference_t< Pred >, Idx::eswitch_index >( 
            std::forward< Pred >( pred ) );
    }

    template < typename P, std::size_t ... I, Index Idx >
    predicate_condition< P, I..., Idx::eswitch_index > 
        compose_new_predicate_condition_type( const predicate_condition< P, I... > & pred_cnd, Idx idx );
         
    template< IsCndPredicate Pred, Index Idx >
    constexpr auto operator,( Pred && pred, Idx idx )
    {
        return decltype( compose_new_predicate_condition_type( pred, idx ) )( std::move( pred.pred_ ) );
    }

    template< Condition Cnd >
    constexpr auto case_( Cnd && cnd )
    { 
        return std::move( cnd ); 
    }

    template< typename ... Ts >
    constexpr auto case_( Ts &&... values )
    {
        auto lmbd = [&]< typename T, T ... ints >( std::index_sequence< ints... >&&, auto && tup )
        {
            auto merged = details::tuple_combine< ints... >( 
                std::make_tuple( Index_< ints >{}... ), std::move( tup ) );

            return ( std::get< ints >( merged ) && ... );
        };

        return lmbd( std::make_index_sequence< sizeof...(Ts) >{},
            std::make_tuple( std::forward< Ts >( values )... ) );
    }

    constexpr auto case_( std::regex && rgx )
    {
        return _1 == rgx;
    }

    template< typename ... Args >
    constexpr auto any_from( Args &&... args )
    {
        return extension::Any_from_impl( std::forward< Args >( args )... );
    }
    
    template< std::size_t From, std::size_t To >
    constexpr bool in_range( const std::size_t value )
    {
        return value >= From && value <= To;
    }

    template< typename T >
    struct my_type
    {
        using type = T;
    };

    auto operator ""_r( const char * rgx, const std::size_t sz )
    {
        return std::regex{ rgx };
    }

    /// static declarations
    static const Fallthrough fallthrough_;
    static const extension::any _;

    #define Case( ... ) case_( __VA_ARGS__ ) % [&]
    #define Default  ( _1 == extension::any{} ) % [&]

} // namespace eswitch_v5