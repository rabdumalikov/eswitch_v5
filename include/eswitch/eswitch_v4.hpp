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

namespace eswitch_v4
{
    template< typename TPred, uint32_t ... Is >
    class predicate_condition;

    template< typename TIndex, typename T >
    class condition;

    template< typename TCnd1, typename TCnd2 >
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

        template< typename ... Args >
        struct void_t { using type = void; };
        
        template< typename T, typename = void >
        struct is_callable : std::false_type {};

        template< typename T >
        struct is_callable< T, typename void_t< decltype( std::declval< T >()() ) >::type > 
            : std::true_type {};

        static bool unreachable() { assert( false ); return false; }

        template< typename T >
        struct holder{};


        template < template < typename > class H, typename S >
        void is_predicate_condition( H<S> && );

        template < template < typename > class H, template< typename, uint32_t ... > class Pr, typename P, uint32_t ... Idxs,
            typename std::enable_if_t< std::is_same< holder< predicate_condition< P, Idxs... > >, H< Pr< P, Idxs... > > >::value, int > = 0 >
        bool is_predicate_condition( H< Pr< P, Idxs... > > && );

        template < template < typename > class H, template< typename, typename > class Cnd, typename T1, typename T2,
            typename std::enable_if_t< std::is_same< holder< condition< T1, T2 > >, H< Cnd< T1, T2 > > >::value, int > = 0 >
        bool is_predicate_condition( H< Cnd< T1, T2 > > && );

        template < template < typename > class H, template< typename T1, typename T2 > class Cnds, typename T1, typename T2,
            typename std::enable_if_t< std::is_same< holder< conditions< T1, T2 > >, H< Cnds< T1, T2 > > >::value, int > = 0 >
        bool is_predicate_condition( H< Cnds< T1, T2 > > && );


        template < typename T >
        struct is_predicate
        {
            static constexpr bool value = std::is_same< decltype( is_predicate_condition( holder< T >() ) ), bool >::value;
        };

    } // namespace details

    template< typename T >
    concept IsCndPredicate = details::is_predicate< T >::value;
    
    namespace extension
    {             
        template< typename T, typename TArray >
        constexpr bool is_in_set( const T & to_check, const TArray & arr )
        {
            for( uint32_t i = 0; i < arr.size(); ++i )
            {
                if (to_check == arr[ i ]) return true;
            }

            return false;
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

    template< typename T2 >
    static bool operator==( const std::any & t1, my_type< T2 > )
    {
        return std::any_cast< T2 >(&t1) != nullptr;        
    }

    template< typename TIndex, typename T >
    class condition
    {
        Comparison_operators cmp_operator;        
        T value_;
    public:

        template< typename TVal >
        constexpr condition( const Comparison_operators cmp_type, TVal&& value ) 
            : cmp_operator( cmp_type ), value_( std::forward< TVal >( value ) )
            { 
            }

        template< typename TSrcTuple >
        bool operator()( const TSrcTuple & src_tuple ) const
        {
            return compare( cmp_operator, std::get< TIndex::eswitch_index >( src_tuple ), value_ );         
        }

        template< int MaxIndex >
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

    template< typename TCnd1, typename TCnd2 >
    class conditions
    {
        Logical_operators logical_operator;
        TCnd1 cnd1_;
        TCnd2 cnd2_;

    public:

        template< typename T_1, typename T_2 >
        conditions( const Logical_operators logical_operator, T_1 && cnd1, T_2 && cnd2 ) 
            : logical_operator( logical_operator )
            , cnd1_( std::forward< T_1 >( cnd1 ) )
            , cnd2_( std::forward< T_2 >( cnd2 ) )
            {                
            }

        template< int MaxIndex >
        static constexpr bool is_out_of_range() 
        {
            return TCnd1::template is_out_of_range< MaxIndex >() || TCnd2::template is_out_of_range< MaxIndex >();
        }
     
        template< typename TSrcTuple >
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

    template< typename Cnd, typename Func >
    struct condition_with_predicate
    {
        template< typename TSrcTuple >
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

    template< typename T >
    concept Index = requires( T ){ T::eswitch_index; };

    template< typename T >
    concept Condition = requires( T t )
    { 
        t( std::make_tuple() ); 
        T::template is_out_of_range<int{}>(); 
    };

    template< typename T >
    concept ReturnValueNoneVoid = std::is_same_v< 
        std::invoke_result_t< std::remove_reference_t< T > >,
        void 
    >;


    template< Index Idx, typename T >
    auto operator==( Idx idx, T && rhv )
    {
        return condition< Idx, T >( Comparison_operators::equal_, std::forward< T >( rhv ) );
    }

    template< Condition T, typename Func >
    auto operator>>( T && cnd, Func && f ) requires details::is_callable< Func >::value
    {
        return condition_with_predicate{ std::move( cnd ), std::move( f ) };
    }
    
    struct Fallthrough {};
    struct Padding{};

    template< typename Cnd, ReturnValueNoneVoid Func >
    auto operator^( condition_with_predicate< Cnd, Func >&& cp, const Fallthrough & )
    {
        cp.fallthrough = true;

        return std::move( cp );
    }

    template< typename ... Args >
    class eswitch2_impl
    {
        std::tuple< Args... > tup_;
    public:
        eswitch2_impl( Args ... ts ) : tup_{ ts... }
        {            
        }

        template< typename T >
        using underlying_t = std::invoke_result_t< typename T::F >;
        
        template< typename ... Cnds >
        auto operator()( Cnds && ... cnds )
        {
            constexpr auto generic_lambda = []< typename T, T... ints >
                ( std::index_sequence< ints... > && int_seq, auto && tup, auto && f )
                {
                    ( f( std::get< ints >( tup ) ), ... );
                };

            using return_t = std::common_type_t< underlying_t< Cnds >... >;

            std::optional< std::conditional_t< std::is_same_v< return_t, void >, Padding, return_t >
                > return_value;

            generic_lambda( 
                std::make_index_sequence< sizeof...( Cnds ) >{}, 
                std::make_tuple( std::forward< Cnds >( cnds )... ), 
                [ this, &return_value, break_ = false ]( const auto & cnd ) mutable
                {
                    if( !break_ && cnd.cnd( tup_ ) ) 
                    {
                        if constexpr( std::is_same_v< return_t, void > )
                            cnd.func();
                        else
                            return_value = cnd.func();

                        break_ = !cnd.fallthrough;
                    }
                });

            if constexpr( !std::is_same_v< return_t, void > ) 
            {
                return return_value.value();
            }
        }
    };

    template< typename ... Ts >
    auto eswitch2( Ts && ... ts )
    {
        return eswitch2_impl( std::forward< Ts >( ts )... );
    }

    template< typename T1, typename T2 >
    conditions( Logical_operators, T1, T2 ) -> conditions< T1, T2 >;

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
            return pred_( std::get< Is >( src_tuple )... );
        }

        template< int MaxIndex >
        static constexpr bool is_out_of_range()
        {
            constexpr std::array< int, sizeof...( Is ) > arr{ Is... };

            for( int32_t i = 0; i < arr.size(); ++i )
            {
                if( arr[ i ] >= MaxIndex ) return true;
            }

            return false;
        }
    };

    template< typename R, typename... Args, Index Idx >
    auto operator,( R(*pred)(Args...), Idx idx )
    {
        return predicate_condition< R(*)(Args...), Idx::eswitch_index >( pred );
    }

    template< typename Pred, Index Idx >
        requires ( details::is_predicate< std::remove_reference_t< Pred > >::value == false )
    auto operator,( Pred && pred, Idx t1 ) 
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

    template< typename T1, typename T2 >
    condition< T1, T2 > case_( condition< T1, T2 > && cnd );

    template< typename T1, typename T2 >
    conditions< T1, T2 > case_( conditions< T1, T2 > && cnds );
     
    template< typename ... Ts >
    struct Always_false
    {
 	    static constexpr bool value = false;
    };

    template< Condition Cnd >
    Cnd case_( Cnd && cnd )
    { 
        return std::move( cnd ); 
    }

    template< typename T >
    auto case_( T && value )
    { 
        return _1 == std::forward< T >( value ); 
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

#define CASE_OVERLOAD( TCase, TCmp ) \
namespace eswitch_v4\
{\
    template< int I >\
    auto operator==( const eswitch_v4::Index_< I > & val, const TCase & rgx )\
    {\
        return val == TCmp( rgx );\
    }\
\
    template< int I >\
    auto operator==( const eswitch_v4::Index_< I > & val, TCase & rgx )\
    {\
        return val == TCmp( rgx );\
    }\
\
    template< int I >\
    auto operator==( const eswitch_v4::Index_< I > & val, TCase && rgx )\
    {\
        return val == TCmp( std::move( rgx ) );\
    }\
}

