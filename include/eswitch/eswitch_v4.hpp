//  Copyright (c) 2019-2020 Rustam Abdumalikov
//
//  "eswitch_v4" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tuple>
#include <experimental/tuple>
#include <array>
#include <cassert>
#include <type_traits>

//namespace std = std::experimental;

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
        struct has_static_member_index : std::false_type {};

        template< typename T >
        struct has_static_member_index< T, typename void_t< decltype( T::eswitch_index ) >::type > 
            : std::true_type {};

        template< typename T, typename = void >
        struct is_determined : std::false_type {};

        template< typename T >
        struct is_determined< T, typename void_t< decltype( T::type ) >::type > 
            : std::true_type {};
        
        template <typename... Ts>
        using has_common_type = typename details::is_determined< std::common_type_t<Ts...> >::type;

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

    namespace other_details
    {
        template <typename F>
        struct return_type_impl;
    
        template <typename R, typename... Args>
        struct return_type_impl<R(Args...)> { using type = R; };

        template <typename R, typename... Args>
        struct return_type_impl<R(Args..., ...)> { using type = R; };

        template <typename R, typename... Args>
        struct return_type_impl<R(*)(Args...)> { using type = R; };

        template <typename R, typename... Args>
        struct return_type_impl<R(*)(Args..., ...)> { using type = R; };

        template <typename R, typename... Args>
        struct return_type_impl<R(&)(Args...)> { using type = R; };

        template <typename R, typename... Args>
        struct return_type_impl<R(&)(Args..., ...)> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args...)> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args..., ...)> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args...) &> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args..., ...) &> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args...) &&> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args..., ...) &&> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args...) const> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args..., ...) const> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args...) const&> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args..., ...) const&> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args...) const&&> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args..., ...) const&&> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args...) volatile> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args..., ...) volatile> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args...) volatile&> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args..., ...) volatile&> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args...) volatile&&> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args..., ...) volatile&&> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args...) const volatile> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args..., ...) const volatile> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args...) const volatile&> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args..., ...) const volatile&> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args...) const volatile&&> { using type = R; };

        template <typename R, typename C, typename... Args>
        struct return_type_impl<R(C::*)(Args..., ...) const volatile&&> { using type = R; };

        template <typename T>
        using return_type_t = typename return_type_impl< decltype( &T::operator() ) >::type;
    } // namespace other_details

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


    template< typename ... Cnds >
    struct conditions_with_predicate
    {    
        template< typename ... Args >
        conditions_with_predicate( Args && ... args ) : cnds_{ std::forward< Args >( args )... } {}
        
        template< typename TSrcTuple >
        void operator()( const TSrcTuple & src_tuple ) const
        {
            std::experimental::apply( [&]( auto && ... args ) { ( args( src_tuple ), ... ); }, cnds_ );
        }

        using conditions_t = std::tuple< Cnds... >;
    private:
        conditions_t  cnds_;
    };    
    
    template< class ... Ts > 
    conditions_with_predicate( Ts && ... ) -> conditions_with_predicate< Ts... >;


    template< int I, typename T >
    auto operator==( const Index_< I >& idx, T && rhv )
    {
        return condition< Index_< I >, T >( Comparison_operators::equal_, std::forward< T >( rhv ) );
    }

    template< typename TIndex, typename T, typename Func >
    auto operator>>( condition< TIndex, T > && cnd, Func && f )
    {
        return condition_with_predicate{ std::move( cnd ), std::move( f ) };
    }

    template< typename TCnd1, typename TCnd2, typename Func >
    auto operator>>( conditions< TCnd1, TCnd2 > && cnds, Func && f )
    {
        return condition_with_predicate{ std::move( cnds ), std::move( f ) };
    }

    struct Fallthrough {};

    /// TODO change to CONCEPTS 
    template< typename Cnd, typename Func, 
        std::enable_if_t< 
            std::is_same< 
                other_details::return_type_t< std::remove_reference_t< Func > >, 
                void 
            >::value, int > = 0 >
    auto operator^( condition_with_predicate< Cnd, Func >&& cp, const Fallthrough & )
    {
        cp.fallthrough = true;

        return std::move( cp );
    }

    // return value is obtained lazy, thus we need mechanism to construct on demand
    template< typename T >
    class Ondemand_construction
    {
        union data
        {
            data(){}
           ~data(){}

            T internals;
        };

        template< typename T__, typename T_ >
        static void construct( Ondemand_construction< T__ > & lazy_one, T_ && value )
        {
            new (&lazy_one.dt.internals) T__( std::forward< T_ >( value ) );
            lazy_one.was_set_ = true;
        }
    
        data dt;
        bool was_set_ = false;

    public:

        Ondemand_construction() = default;
        Ondemand_construction( Ondemand_construction && t ) = delete;
        Ondemand_construction& operator=( Ondemand_construction& ) = delete;
        Ondemand_construction& operator=( Ondemand_construction&& ) = delete;
        
       ~Ondemand_construction()
        {
            if( was_set_ ) dt.internals.~T();
        }

        template< typename T_ >
        explicit Ondemand_construction( T_ && t )
        {
            construct( *this, std::forward< T >( t ) );
        }

        Ondemand_construction& operator=( const T & t ) 
        {
            construct( *this, t );
            return *this;
        }

        template< typename T_ >
        Ondemand_construction( T_ && t, bool was_set )
        {
            if( was_set ) construct( *this, std::forward< T >( t ) );
        }

        operator bool() const { return was_set_; }

        T release() 
        { 
            return std::move( dt.internals ); 
        }

        T release_with_check() 
        { 
            if( !was_set_ ) throw( std::logic_error( "Ondemand_construction is empty!!!" ) ); 

            auto tmp = release();
            was_set_ = false;

            return tmp; 
        }
    };

    template<>
    class Ondemand_construction< void >
    {
    public:

        Ondemand_construction() = default;
        Ondemand_construction( Ondemand_construction && t ) = delete;
        Ondemand_construction& operator=( Ondemand_construction& ) = delete;
        Ondemand_construction& operator=( Ondemand_construction&& ) = delete;
        
       ~Ondemand_construction() = default;

        template< typename T_ >
        explicit Ondemand_construction( T_ && t )
        {
        }

        template< typename T_ >
        Ondemand_construction( T_ && t, bool was_set )
        {
        }

        constexpr operator bool() const { return false; }
    };

    template< typename ... Args >
    class eswitch2_impl
    {
        std::tuple< Args... > tup_;
    public:
        eswitch2_impl( Args ... ts ) : tup_{ ts... }
        {            
        }

        template< typename T >
        using underlying_t = other_details:: return_type_t< typename T::F >;

        template< typename ... Cnds >
        std::common_type_t< underlying_t< Cnds >... > operator()( Cnds && ... cnds )
        {
            constexpr auto generic_lambda = []< typename T, T... ints >
                ( std::index_sequence< ints... > && int_seq, auto && tup, auto && f )
                {
                    ( f( std::get< ints >( tup ) ), ... );
                };

            using return_t = std::common_type_t< underlying_t< Cnds >... >;

            Ondemand_construction< return_t > return_val;

            generic_lambda( 
                std::make_index_sequence< sizeof...( Cnds ) >{}, 
                std::make_tuple( std::forward< Cnds >( cnds )... ), 
                [ this, &return_val, break_ = false ]( const auto & cnd ) mutable
                {
                    if( !break_ && cnd.cnd( tup_ ) ) 
                    {
                        if constexpr( std::is_same< return_t, void >::value )
                            cnd.func();
                        else
                        {
                            return_val = cnd.func();
                        }

                        break_ = !cnd.fallthrough;
                    }
                });

            if constexpr( !std::is_same< return_t, void >::value ) return return_val.release_with_check();
        }
    };

    template< typename ... Ts >
    auto eswitch2( Ts && ... ts )
    {
        return eswitch2_impl( std::forward< Ts >( ts )... );
    }


    template< int I, typename T >
    auto operator!=( const Index_< I >& idx, T && rhv )
    {
        return condition< Index_< I >, T >( Comparison_operators::not_equal_, std::forward< T >( rhv ) );
    }

    template< typename T1, typename T2, typename T3 >
    auto operator&&( T1 && i, condition< T2, T3 > && j )
    {
        return conditions< T1, condition< T2, T3 > >( Logical_operators::and_, std::forward< T1 >( i ), std::move( j ) );
    }

    template< typename T1, typename T2, typename T3 >
    auto operator||( T1 && i, condition< T2, T3 > && j )
    {
        return conditions< T1, condition< T2, T3 > >( Logical_operators::or_, std::forward< T1 >( i ), std::move( j ) );
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

    template< typename R, typename... Args, typename T, typename std::enable_if_t< details::has_static_member_index< T >::value, int > = 0 >
    auto operator,( R(*pred)(Args...), const T& t1 )
    {
        return predicate_condition< R(*)(Args...), T::eswitch_index >( pred );
    }

    template< typename Pred, typename T, typename std::enable_if_t< !details::is_predicate< std::remove_reference_t< Pred > >::value && 
        details::has_static_member_index< T >::value, int > = 0 >
    auto operator,( Pred&& pred, const T& t1 )
    {
        return predicate_condition< std::remove_reference_t< Pred >, T::eswitch_index >( std::forward< Pred >( pred ) );
    }

    template < typename P, uint32_t ... I, typename T, typename std::enable_if_t< details::has_static_member_index< T >::value, int > = 0 >
    predicate_condition< P, I..., T::eswitch_index > compose_new_predicate_condition_type( const predicate_condition< P, I... > & pred_cnd, const T& t1 ); 

    template< typename Pred, typename T, typename std::enable_if_t< details::is_predicate< std::remove_reference_t< Pred > >::value, int > = 0 >
    auto operator,( Pred&& pred, const T& t1 )
    {
        return decltype( compose_new_predicate_condition_type( pred, t1 ) )( std::move( pred.pred_ ) );
    }

    template< typename T1, typename P, uint32_t ... I >
    auto operator&&( T1 && i, predicate_condition< P, I... > && j )
    {
        return conditions< T1, predicate_condition< P, I... > >( Logical_operators::and_, std::forward< T1 >( i ), std::move( j ) );
    }

    template< typename T1, typename P, uint32_t ... I >
    auto operator||( T1 && i, predicate_condition< P, I... > && j )
    {
        return conditions< T1, predicate_condition< P, I... > >( Logical_operators::or_, std::forward< T1 >( i ), std::move( j ) );
    }

    template< typename T >
    struct Return_value_impl
    {
        T callback;
    };

    struct In_place_return_value{};

    struct Padding{};
    struct Filled{};

    template< typename T1, typename T2 >
    condition< T1, T2 > case_( condition< T1, T2 > && cnd );

    template< typename T1, typename T2 >
    conditions< T1, T2 > case_( conditions< T1, T2 > && cnds );
     
    struct Default_impl
    {
        condition< Index_< 0 >, extension::any > case_for_any_match = case_( _1 == extension::any{} );
    };

    template< typename ... Ts >
    struct Always_false
    {
 	    static constexpr bool value = false;
    };

    template< typename TValue >
    struct Value_to_return{
        TValue return_value_;
    };

    struct Switch_states
    {      
        bool was_case_executed = false;
        bool execute_current_case = false;
        bool need_fallthrough = false;
        bool need_break = true;
    };

    template< typename R, typename ... TArgs >
    class Eswitch
    {
        std::tuple< TArgs... > pack_;
        Ondemand_construction< R > return_val_;

        Switch_states states_;

        template< typename T, typename ... Ts >
        friend class Eswitch;

        template< typename TEswitch >
        friend struct Eswitch_for_case_only;

    public:
        template< typename T, typename ... Ts >
        Eswitch( Eswitch< T, Ts... >&& args ) 
            : return_val_( static_cast< R >( args.return_val_.release() ), args.return_val_.operator bool() )
            , pack_( std::move( args.pack_ ) )
            , states_( args.states_ )
            {
            }

        template< typename ... Ts >
        Eswitch( Eswitch< Filled*, Ts... >&& args )
            : return_val_( static_cast< R >( nullptr ), args.return_val_.operator bool() )
            , pack_( std::move( args.pack_ ) )
            , states_( args.states_ )
            {
            }

        template< typename TR, typename T, typename ... Ts >
        Eswitch( TR&& return_value, Eswitch< T, Ts... >&& args ) 
            : return_val_( std::forward< TR >( return_value ) )
            , pack_( std::move( args.pack_ ) )
            , states_( args.states_ )
            {
            }

        template< typename ... Ts >
        Eswitch( Eswitch< Padding*, Ts... >&& args )
            : pack_( std::move( args.pack_ ) )
            , states_( args.states_ )
            {
            }

        template< typename ... T >
        Eswitch( T&& ... args ) : pack_( std::forward< T >( args )... )
        {
        }

        Eswitch operator>>( const Fallthrough& )
        {
            if( states_.was_case_executed && states_.execute_current_case ) 
            { 
                states_.need_fallthrough = true; 
                states_.need_break = false; 
            }

            return std::move( *this );
        }
        
        auto operator>>( const In_place_return_value& )
        {
            if( !return_val_ ) throw( std::logic_error( "None of the cases returned Ondemand_construction!" ) );

            return return_val_.release_with_check();
        }

        template< typename T >
        void operator>>( Return_value_impl< T > && lambda )
        {
            if( return_val_ ) lambda.callback( return_val_.release_with_check() );
        }

        // auto operator>>( const Default_impl & default_lambda )
        // {
        //     return Eswitch_for_return_only< decltype( *this >> default_lambda.case_for_any_match ) >( *this >> default_lambda.case_for_any_match );
        // }

        template< typename Tlambda, typename std::enable_if_t< details::is_callable< std::remove_reference_t< Tlambda > >::value && std::is_same< other_details::return_type_t< std::remove_reference_t< Tlambda > >, void >::value, int > = 0 >
        Eswitch operator>>( Tlambda && lambda )
        {
            if( return_val_ ) return std::move( *this );

            if( states_.execute_current_case && !states_.was_case_executed ) 
            {
                lambda();
                states_.was_case_executed = states_.execute_current_case;
            }
            else if( states_.need_fallthrough ) 
            {
                lambda();
                states_.need_fallthrough = false;
                states_.need_break = true;
                states_.execute_current_case = true;
            }

            return std::move( *this );
        }

        template< typename Tlambda, typename std::enable_if_t< details::is_callable< std::remove_reference_t< Tlambda > >::value && !std::is_same< other_details::return_type_t< std::remove_reference_t< Tlambda > >, void >::value, int > = 0 >
        auto operator>>( Tlambda && lambda )
        {
            if( return_val_ ) 
                return decltype( handle_return_value( lambda() ) )( std::move( *this ) );

            if( states_.execute_current_case || states_.need_fallthrough )
                return handle_return_value( lambda() );
            else
                return decltype( handle_return_value( lambda() ) )( std::move( *this ) );
        }

        // template< typename T1, typename T2 >
        // auto operator>>( const conditions< T1, T2 >& cnds )
        // {            
        //     static_assert( !conditions< T1, T2 >::template is_out_of_range< sizeof...( TArgs ) >(), "Index in 'Predicate' is out of range!!" );

        //     return Eswitch_for_case_only< Eswitch >( handle_case( cnds ) );
        // }

        // template< typename T1, typename T2 >
        // auto operator>>( const condition< T1, T2 >& cnd)
        // {
        //     return Eswitch_for_case_only< Eswitch >( handle_case( cnd ) );
        // }
            
        // template< typename TPred, uint32_t ... Is >
        // auto operator>>( const predicate_condition< TPred, Is... > & value )
        // {
        //     static_assert( !predicate_condition< TPred, Is... >::template is_out_of_range< sizeof...( TArgs ) >(), "Index in 'Predicate' is out of range!!" );
            
        //     return Eswitch_for_case_only< Eswitch >( handle_case( value ) );
        // }

        template< typename TReturnValue >
        auto operator>>( Value_to_return< TReturnValue >&& value )
        {
            if( return_val_ ) return decltype( handle_return_value( std::move( value.return_value_ ) ) )( std::move( *this ) );

            return handle_return_value( std::move( value.return_value_ ) );           
        }

    private:
        
        template< typename TReturnValue, typename std::enable_if_t< 
            !Always_false< TReturnValue >::value &&
          ( !std::is_same< R, Padding* >::value && 
             std::is_convertible< R, TReturnValue >::value ) || 
             std::is_same< R, Filled* >::value ||
             std::is_same< R, Padding* >::value, int > = 0 >
        auto handle_return_value( TReturnValue && value )
        {
            return handle_case_with_return( std::forward< TReturnValue >( value ) );
        }
        
        template< typename TReturnValue, typename std::enable_if_t< 
            !Always_false< TReturnValue >::value &&
            !std::is_same< R, Padding* >::value &&
             std::is_convertible< TReturnValue, R >::value &&
            !std::is_convertible< R, TReturnValue >::value, int > = 0 >
        auto handle_return_value( TReturnValue && value )
        {
            return handle_case_with_return( static_cast< R&& >( value ) );
        }


        template< typename TReturnValue, typename std::enable_if_t<  
            !Always_false< TReturnValue >::value &&
            !std::is_convertible< R, TReturnValue >::value && 
            !std::is_convertible< TReturnValue, R >::value && 
            !std::is_same< R, Padding* >::value && 
            details::has_common_type< R, TReturnValue >::value, int > = 0 >
        auto handle_return_value( TReturnValue && value )
        {
            return handle_case_with_return( static_cast< std::common_type_t< R, TReturnValue >&& >( value ) );
        }

        auto handle_return_value( std::nullptr_t && value )
        {
            return handle_case_with_return( static_cast< typename std::conditional< std::is_same< R, Padding* >::value, Filled*, R >::type >( nullptr ) );
        }

        template< typename TReturnValue >
        auto handle_case_with_return( TReturnValue && value )
        {            
            if( states_.execute_current_case && !states_.was_case_executed ) 
            {
                states_.was_case_executed = states_.execute_current_case;

                return Eswitch< TReturnValue, TArgs... >( std::forward< TReturnValue >( value ), std::move( *this ) );
            }
            else if( states_.need_fallthrough ) 
            {
                states_.need_fallthrough = false;
                states_.need_break = true;

                return Eswitch< TReturnValue, TArgs... >( std::forward< TReturnValue >( value ), std::move( *this ) );
            }
            
            return Eswitch< TReturnValue, TArgs... >( std::move( *this ) );
        } 

        template< typename T >
        auto handle_case( const T & cnd )
        {
            states_.execute_current_case = false;
            
            if( states_.was_case_executed && ( states_.need_break || states_.need_fallthrough ) ) return std::move( *this );

            states_.execute_current_case = cnd( pack_ );

            return std::move( *this );
        }
    };

    template< typename ... TArgs >
    auto eswitch( TArgs && ... args )
    { 
        return Eswitch< Padding*, TArgs... >( std::forward< TArgs >( args )... ); 
    }

    template< typename T1, typename T2 >
    condition< T1, T2 > case_( condition< T1, T2 > && cnd )
    { 
        return std::move( cnd ); 
    }

    template< typename T1, typename T2 >
    conditions< T1, T2 > case_( conditions< T1, T2 > && cnds )
    { 
        return std::move( cnds ); 
    }

    template< typename TPred, uint32_t ... Is >
    auto case_( predicate_condition< TPred, Is... > && value )
    { 
        return std::move( value );  
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
    
    template< typename T >
    auto handle_return( T && lmbd )
    { 
        return Return_value_impl< T >{ std::forward< T >( lmbd ) };
    }

    template< typename T >
    auto to_return( T && value )
    { 
        using return_t = decltype( details::Just_find_out_return_type( std::forward< T >( value ) ) );

        return Value_to_return< return_t >{ std::forward< T >( value ) };
    }

    template< int64_t From, int64_t To >
    bool in_range( const int64_t value )
    {
        return value >= From && value <= To;
    }

    /// static declarations
    static const In_place_return_value in_place_return_; // return_in_place_;
    static const Default_impl default_;
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

