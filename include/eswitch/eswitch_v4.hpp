//  Copyright (c) 2019-2020 Rustam Abdumalikov
//
//  "eswitch_v4" library
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

/// TODO
// move Default case to the END - DONE
// lazy_eswitch - DECLINED( since it doesn't have sence for now )
// tuple+pair handle - DONE
// write concept Comparable( or maybe stl has one ) DONE
// willcard

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

namespace eswitch_v4
{
    template< typename T >
    concept Index = requires( T ){ std::decay_t<T>::eswitch_index; };

    template< typename T >
    concept Condition = requires( T t )
    { 
        t( std::make_tuple() ); 
        std::decay_t<T>::template is_out_of_range<int{}>(); 
    };

    template< typename T >
    concept ConditionClass = requires( T t )
    { 
        t.value();
    };

    template< typename T >
    concept ReturnValueNoneVoid = std::is_same_v< 
        std::invoke_result_t< std::remove_reference_t< T > >,
        void 
    >;

    template< typename TPred, uint32_t ... Is >
    class predicate_condition;

    template< Index TIndex, typename T >
    class condition;

    template< Condition TCnd1, Condition TCnd2 >
    class conditions;

    enum class Logical_operators{ and_, or_ };
    enum class Comparison_operators{ equal_, not_equal_ };

    namespace extension
    {
        class Range
        {
            int32_t start_;
            int32_t end_;

            public:
            Range( const int32_t start, const int32_t end ) 
                : start_( start ), end_( end )
                {                
                }

            friend bool operator==( const int32_t val, const Range & rm )
            {
                return val >= rm.start_ && val <= rm.end_;
            }
        };

        struct any;        
    }


    template< int32_t Idx >
    struct Index_
    { 
        static constexpr int32_t eswitch_index = Idx;

        auto in( const int32_t start, const int32_t end ) const
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
        auto Just_find_out_return_type( T && value ){ return std::forward< T >( value ); }
        
        template< typename T >
        struct is_condition : std::false_type {};
        
        template< Index I, typename T >
        struct is_condition< condition< I, T > > : std::true_type {};

        template< typename T >
        struct is_default_case : std::false_type {};

        template< Index I >
        struct is_default_case< condition< I, extension::any > > : std::true_type {};

        template< typename T >
        constexpr bool is_default_case_v = is_default_case< T >::value;

        template< typename T >
        constexpr bool is_condition_v = is_condition< T >::value;

        template< typename T >
        struct is_std_variant : std::false_type {};
        
        template< typename ... T >
        struct is_std_variant< std::variant< T... > > : std::true_type {};

        template< typename ... T >
        constexpr bool is_std_variant_v = is_std_variant< T... >::value;

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
        constexpr bool is_std_any_v = std::is_same_v< T, std::any >;

        template< typename T >
        struct is_callable_impl : std::false_type {};

        template< typename R, typename C, typename ... Args >
        struct is_callable_impl< R(C::*)(Args...) const > : std::true_type {};

        template< typename R, typename C, typename ... Args >
        struct is_callable_impl< R(C::*)(Args...) > : std::true_type {};

        template< typename T >
        constexpr bool is_callable_v = is_callable_impl< decltype( &T::operator() ) >::value;

        static bool unreachable() { assert( false ); return false; }

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
            static constexpr int value = sizeof...(Args);
            using type = R;
        };

        template< typename R, typename C, typename ... Args >
        struct amount_args_function_has< R(C::*)(Args...) >
        {
            static constexpr int value = sizeof...(Args);
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

        template< typename T >
        concept has_type = requires{ typename T::type; };

        template< typename T >
        concept has_value = requires{ T::value; };

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
        constexpr auto tuple_merge( Tup1 && tup1, Tup2 && tup2 )
        {
            return std::make_tuple( (std::get< Is >( std::move( tup1 ) ) == std::get< Is >( std::move( tup2 ) ) )... );
        }

    } // namespace details
    
    template< typename T >
    concept ComparableExceptAnyAndVariant = 
        !details::is_std_any_v< T > || 
        !details::is_std_variant_v< T > || 
        requires( T a, T b ) {
        { a == b };
        { a != b };
    };

    template< typename T >
    concept IsCndPredicate = details::is_predicate_v< T >;

    template< typename T >
    concept Callable = details::is_callable_v< T >;

    template< typename T >
    concept StdTuple = details::is_std_tuple_v< T >;

    template< typename T >
    concept StdPair = details::is_std_pair_v< T >;

    namespace extension
    {             
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

        template< typename T, typename ... TArgs >
        struct Any_from_impl
        {
            std::array< T, sizeof...(TArgs) + 1 > anythings;

            template< typename T1, typename ... TOthers >
            constexpr Any_from_impl( T1 && arg, TOthers && ... args )
                : anythings{ std::forward< T1 >( arg ), std::forward< TOthers >( args )... }
                {
                }

            template< typename T_ >
            friend constexpr bool operator==( const T_ & value, const Any_from_impl& st ) 
            {
                return is_in_set( value, st.anythings );
            }
        };        
    } // namespace extension


    template< typename T >
    struct my_type
    {
        using type = T;
    };

    template< typename T >
    static bool operator==( const std::any & t1, my_type< T > )
    {
        return std::any_cast< T >(&t1) != nullptr;        
    }

    template< typename ... Args, typename T >
    static bool operator==( const std::variant< Args... > & t1, my_type< T > )
    {
        return std::get_if< T >(&t1) != nullptr;        
    }

    template< typename T >
    static auto get_value( const std::any & t1, my_type< T > )
    {
        return std::any_cast< T >(t1);        
    }

    template< typename ... Args, typename T >
    static auto get_value( const std::variant< Args... > & t1, my_type< T > )
    {
        return std::get< T >(t1);        
    }

    template< Index TIndex, typename T >
    class condition
    {
        Comparison_operators cmp_operator;        
        T value_;
    public:

        template< typename Arg >
        constexpr condition( const Comparison_operators cmp_type, Arg && value ) 
            : cmp_operator( cmp_type ), value_( std::forward< Arg >( value ) )
            { 
            }

        template< StdTuple TSrcTuple >
        bool operator()( const TSrcTuple & src_tuple ) const
        {
            static_assert( !is_out_of_range< std::tuple_size_v< TSrcTuple > >(), 
                "Case Index is OUT OF RANGE" ); 

            return compare( cmp_operator, std::get< TIndex::eswitch_index >( src_tuple ), value_ );         
        }

        template< StdTuple TSrcTuple >
        auto value( const TSrcTuple & src_tuple ) const
        {
            return get_value( std::get< TIndex::eswitch_index >( src_tuple ), value_ );
        }

        template< uint32_t MaxIndex >
        static constexpr bool is_out_of_range() 
        { 
            return TIndex::eswitch_index >= MaxIndex; 
        }

    private:

        template< typename T1, typename T2 >
        static bool compare( const Comparison_operators CmpOper, T1 && t1, T2 && t2 )
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
    };

    template< Condition TCnd1, Condition TCnd2 >
    class conditions
    {
        Logical_operators logical_operator;
        TCnd1 cnd1_;
        TCnd2 cnd2_;

    public:

        template< Condition Cnd1, Condition Cnd2 >
        conditions( const Logical_operators logical_operator, Cnd1 && cnd1, Cnd2 && cnd2 ) 
            : logical_operator( logical_operator )
            , cnd1_( std::forward< Cnd1 >( cnd1 ) )
            , cnd2_( std::forward< Cnd2 >( cnd2 ) )
            {                
            }

        template< uint32_t MaxIndex >
        static constexpr bool is_out_of_range() 
        {
            return TCnd1::template is_out_of_range< MaxIndex >() || 
                   TCnd2::template is_out_of_range< MaxIndex >();
        }
     
        template< StdTuple TSrcTuple >
        bool operator()( const TSrcTuple & src_tuple ) const
        {
            return compare( logical_operator, cnd1_( src_tuple ),  cnd2_( src_tuple ) );            
        }

    private:

        static bool compare( const Logical_operators LogicalOperator, bool t1, bool t2 )
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

    template< Condition Cnd, Callable Func >
    struct condition_with_predicate
    {
        template< StdTuple TSrcTuple >
        void operator()( const TSrcTuple & src_tuple ) const
        {
            if( cnd( src_tuple ) ) func();
        }

        using F = Func;

        Cnd  cnd;
        Func func;
        bool fallthrough = false;
    };

    template< typename T, typename F >
    condition_with_predicate( T, F ) -> condition_with_predicate< T, F >;

    template< Index Idx, typename T >
    auto operator==( Idx idx, T && rhv )
    {
        return condition< Idx, T >( Comparison_operators::equal_, std::forward< T >( rhv ) );
    }

    template< Condition T, Callable Func >
    auto operator%( T && cnd, Func && f )
    {
        return condition_with_predicate{ std::move( cnd ), std::move( f ) };
    }
    
    struct Fallthrough {};

    template< typename Cnd, ReturnValueNoneVoid Func >
    auto operator^( condition_with_predicate< Cnd, Func >&& cp, const Fallthrough & )
    {
        cp.fallthrough = true;

        return std::move( cp );
    }

    template< typename ... Args >
    class eswitch_impl
    {
        static_assert( ( ComparableExceptAnyAndVariant<Args> && ... ), "Input Types should be COMPARABLE!" );

        std::tuple< const Args &... > tup_;
    public:
        eswitch_impl( const Args &... ts ) : tup_{ std::cref( ts )... }
        {            
        }

        template< typename T >
        using underlying = details::invoke_result< typename T::F >;

        template< typename T >
        using underlying_t = details::invoke_result_t< typename T::F >;

        template< typename T >
        using args_t = details::amount_args< typename T::F >;

        template< typename ... Cnds >
        auto operator()( Cnds && ... cnds )
        {
            if constexpr( ( !details::has_value< args_t< Cnds > > || ... ) )
            {
                static_assert( !( !details::has_value< args_t< Cnds > > || ... ), 
                    "Predicate with 'auto' argument aren't ALLOWED!" );

            }        
            else if constexpr( ( details::has_type< underlying< Cnds > > && ... ) )
            {
                static_assert( details::has_type< std::common_type< underlying_t< Cnds >... > >, 
                    "Inconsistent 'Return type'!" );
            }
        }

        struct Padding {};

        template< typename ... Cnds >
        auto operator()( Cnds && ... cnds ) requires details::has_type< std::common_type< underlying_t< Cnds >... > >
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

                    if( break_ || !cnd.cnd( tup_ ) ) return;

                    constexpr auto amount_args = amount_args_v< decltype(cnd.func) >;

                    using first_type_t = std::decay_t< decltype( std::get< 0 >( tup_ ) ) >;

                    if constexpr( sizeof...( Args ) == 1 && amount_args == 1 &&
                                  is_condition_v< decltype( cnd.cnd ) > &&
                                ( is_std_any_v< first_type_t > || is_std_variant_v< first_type_t > ) )
                    {
                        if constexpr( !has_return_value )
                        {
                            cnd.func( cnd.cnd.value( tup_ ) );
                        }
                        else
                        {
                            return_value = cnd.func( cnd.cnd.value( tup_ ) );
                        }                            
                    }
                    else if constexpr( amount_args == 0 )
                    {
                        if constexpr( !has_return_value )
                        {
                            cnd.func();
                        }
                        else
                        { 
                            return_value = cnd.func();
                        }
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
    auto eswitch( Ts && ... ts )
    {
        return eswitch_impl( std::forward< Ts >( ts )... );
    }

    template< StdPair T >
    auto eswitch( T && pair )
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
    auto eswitch( T && tup )
    {
        auto expand_tuple = [] < typename _T, _T ... ints >
            ( std::index_sequence< ints... > &&, const auto & tup )
        {
            return eswitch_impl( std::get< ints >( tup )... );
        };

        return expand_tuple( std::make_index_sequence< std::tuple_size_v< std::decay_t< T > > >{}, tup );
    }

    template< Index Idx, typename T >
    auto operator!=( Idx idx, T && rhv )
    {
        return condition< Idx, T >( Comparison_operators::not_equal_, std::forward< T >( rhv ) );
    }

    template< typename T1, typename T2, typename T3 >
    auto operator&&( T1 && i, condition< T2, T3 > && j )
    {
        return conditions( Logical_operators::and_, std::forward< T1 >( i ), std::move( j ) );
    }

    template< typename T1, typename T2, typename T3 >
    auto operator||( T1 && i, condition< T2, T3 > && j )
    {
        return conditions( Logical_operators::or_, std::forward< T1 >( i ), std::move( j ) );
    }

    template< typename TPred, uint32_t ... Is >
    class predicate_condition
    {
        public:
        TPred pred_;

        template< typename T >
        predicate_condition( T && pred ) 
            : pred_( std::forward< T >( pred ) )
            {                    
            }

        template< typename TSrcTuple >
        bool operator()( const TSrcTuple & src_tuple ) const
        {
            static_assert( !is_out_of_range< std::tuple_size_v< TSrcTuple > >(), 
                "Case Index is OUT OF RANGE" ); 

            return pred_( std::get< Is >( src_tuple )... );
        }

        template< uint32_t MaxIndex >
        static constexpr bool is_out_of_range()
        {
            constexpr std::array< bool, sizeof...( Is ) > list{ ( Is >= MaxIndex )... };

            return std::find( std::begin( list ), std::end( list ), true ) != std::end( list );
        }
    };

    template< typename R, typename... Args, Index Idx >
    auto operator,( R(*pred)(Args...), Idx )
    {
        return predicate_condition< R(*)(Args...), Idx::eswitch_index >( pred );
    }

    template< typename Pred, Index Idx >
        requires ( details::is_predicate_v< std::decay_t< Pred > > == false )
    auto operator,( Pred && pred, Idx ) 
    {
        return predicate_condition< std::remove_reference_t< Pred >, Idx::eswitch_index >( 
            std::forward< Pred >( pred ) );
    }

    template < typename P, uint32_t ... I, Index Idx >
    predicate_condition< P, I..., Idx::eswitch_index > 
        compose_new_predicate_condition_type( const predicate_condition< P, I... > & pred_cnd, Idx idx );
         
    template< IsCndPredicate Pred, Index Idx >
    auto operator,( Pred && pred, Idx idx )
    {
        return decltype( compose_new_predicate_condition_type( pred, idx ) )( std::move( pred.pred_ ) );
    }

    template< typename T1, Condition Cnd >
    auto operator&&( T1 && i, Cnd && cnd )
    {
        return conditions( Logical_operators::and_, std::forward< T1 >( i ), std::move( cnd ) );
    }

    template< typename T1, Condition Cnd >
    auto operator||( T1 && i, Cnd && cnd )
    {
        return conditions( Logical_operators::or_, std::forward< T1 >( i ), std::move( cnd ) );
    }

    template< Condition Cnd1, Condition Cnd2 >
    auto operator&&( Cnd1 && cnd1, Cnd2 && cnd2 )
    {
        return conditions( Logical_operators::and_, std::forward< Cnd1 >( cnd1 ), std::forward< Cnd2 >( cnd2 ) );
    }

    template< Condition Cnd1, Condition Cnd2 >
    auto operator||( Cnd1 && cnd1, Cnd2 && cnd2 )
    {
        return conditions( Logical_operators::or_, std::forward< Cnd1 >( cnd1 ), std::move( cnd2 ) );
    }

    template< Condition Cnd >
    Cnd case_( Cnd && cnd )
    { 
        return std::move( cnd ); 
    }

    template< typename ... Ts >
    auto case_( Ts &&... values )
    {        
        auto lmbd0 = []< int I >( Index_< I > && idx, const auto & tup, const auto & f )
        {
            f( idx, std::get< I >( tup ) );
        };

        auto lmbd1 = [&] < typename T, T ... ints >( std::index_sequence< ints... >&&, const auto & tup )
        {
            auto t = std::make_tuple( Index_< ints >{}... );
            auto merged = details::tuple_merge< ints... >( std::move( t ), std::move( tup ) );

            //return std::get< 0 >( merged ) && std::get< 1 >( merged );
            return ( std::get< ints >( merged ) && ... );
        };


        return lmbd1( std::make_index_sequence< sizeof...(Ts) >{}, std::make_tuple( std::forward< Ts >( values )... ) );

        // auto tup = std::make_tuple( std::forward< Ts >( values )... );

        // auto lmbd0 = []< int I >( Index_< I > && idx, const auto & tup, const auto & f )
        // {
        //     f( idx, std::get< I >( tup ) );
        // };

        // auto lmbd1 = [&] < typename T, T ... ints >( std::index_sequence< ints... >&&, const auto & tup, auto && f )
        // {
        //     ( lmbd0( Index_< ints >{}, tup, f ), ... );
        // };

        // std::tuple<> output;

        // lmbd1( std::make_index_sequence< sizeof...( Ts ) >{}, tup, 
        //     [&]< int I >( Index_< I > idx, const auto & val )
        //     {
        //         if constexpr( Condition< decltype( val ) > )
        //         {
        //             output = std::tuple_cat( output, std::make_tuple( val ) );
        //         }
        //         else
        //         {
        //             output = std::tuple_cat( output, std::make_tuple( idx == val ) );
        //         }                
        //     } );

        // auto lmbd3 = [&] < typename T, T ... ints >( std::index_sequence< ints... >&&, auto & tup )
        // {
        //     return ( ( std::get< ints >( tup ) && ... ) );
        // };

        // return lmbd3( std::make_index_sequence< sizeof...(Ts) >{}, output );
    }

    template< typename T, typename ... TArgs >
    auto any_from( T && arg1, TArgs&& ... args )
    {
        using return_t = decltype( details::Just_find_out_return_type( std::forward< T >( arg1 ) ) );

        return extension::Any_from_impl< return_t, TArgs... >( 
            std::forward< T >( arg1 ), std::forward< TArgs >( args )... );
    }
    
    template< int64_t From, int64_t To >
    bool in_range( const int64_t value )
    {
        return value >= From && value <= To;
    }

    /// static declarations
    static const Fallthrough fallthrough_;

} // namespace eswitch_v4