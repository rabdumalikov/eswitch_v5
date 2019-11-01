#pragma once

//  Copyright 2019 Rustam Abdumalikov.
//
//  "eswitch_v4" library
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <tuple>
#include <array>
#include <cassert>

namespace eswitch_v4
{
    template< int32_t Idx >
    struct Index_{ static constexpr int32_t eswitch_index = Idx; };

    const Index_< 0 > _1;
    const Index_< 1 > _2;
    const Index_< 2 > _3;
    const Index_< 3 > _4;
    const Index_< 4 > _5;
    const Index_< 5 > _6;
    const Index_< 6 > _7;
    const Index_< 7 > _8;
    const Index_< 8 > _9;
    const Index_< 9 > _10;
     
    enum class Logical_operators{ and_, or_ };
    enum class Comparison_operators{ equal_, not_equal_ };

    template< typename TPred, uint32_t ... Is >
    class predicate_condition;

    template< typename TIndex, typename T >
    class condition;

    template< typename TCnd1, typename TCnd2 >
    class conditions;
    

    namespace details 
    {
        template< typename T >
        auto Just_find_out_return_type( T && value ){ return std::forward< T >( value ); }

        struct base 
        {
            struct Yes{};
            struct No{ Yes no[2]; };
        };

        template< typename T >
        struct has_static_member_index : private base
        {            
            template< typename U1, int U2 > struct check;
            
            template< typename T_ >
            static Yes is_determine( check< T_, T_::eswitch_index > * );
            
            template< typename T_ >
            static No is_determine( ... );

            static constexpr bool value = sizeof( is_determine< T >( 0 ) ) == sizeof( Yes );  
        };

        template< typename T >
        using has_static_member_index_t = has_static_member_index< T >;

        template< typename T >
        struct is_determined : private base
        {           
            template< typename U1, typename U2 > struct check;
            
            template< typename T_ >
            static Yes is_determine( check< T_, typename T_::type > * );
            
            template< typename T_ >
            static No is_determine( ... );

            static constexpr bool value = sizeof( is_determine< T >( 0 ) ) == sizeof( Yes );  
        };

        template <bool TStatus, typename... Ts>
        struct has_common_type_impl;

        template <typename... Ts>
        struct has_common_type_impl< false, Ts... > : std::false_type {};

        template <typename... Ts>
        struct has_common_type_impl< true, Ts... > : std::true_type {};
        
        template <typename... Ts>
        using has_common_type = typename details::is_determined< std::common_type_t<Ts...> >::type;


        template <typename T, bool >
        class is_callable_impl;

        template< typename T >
        class is_callable_impl< T, true >  : private base
        {
            struct Fallback { int operator()() { return 1; } };
            struct Derived : T, Fallback { };

            template<typename U, U> struct Check;

            template<typename U>
            static No has_alternative_callable_operator(Check< int(Fallback::*)(), &U::operator()> *);

            template<typename U>
            static Yes has_alternative_callable_operator(...);

        public:
            static constexpr bool value = sizeof(has_alternative_callable_operator<Derived>(0)) == sizeof(Yes);
        };


        template <typename T >
        class is_callable_impl< T, false > : public std::false_type {};

        template <typename T >
        class is_callable
        {
        public:
            static constexpr bool value = is_callable_impl< T, std::is_class< T >::value >::value;
        };

        template< int Idx, typename ... T, typename TLambda, typename std::enable_if< (  Idx >= sizeof...( T ) ), int >::type = 0 >
        constexpr void Tuple_for_each_impl( const std::tuple< T... > & tup, const TLambda & callback ) 
        {
        }

        template< int Idx, typename ... T, typename TLambda, typename std::enable_if< ( Idx < sizeof...( T ) ), int >::type = 0 >
        constexpr void Tuple_for_each_impl( const std::tuple< T... > & tup, const TLambda & callback ) 
        { 
            return callback( std::get< Idx >( tup ) ), Tuple_for_each_impl< Idx + 1 >( tup, callback ); 
        }

        template< typename ... T, typename TLambda >
        constexpr void Tuple_for_each( const std::tuple< T... > & tup, TLambda && callback ) 
        { 
            return Tuple_for_each_impl< 0 >( tup, callback );
        }

        template< int Idx, typename ... T, typename TLambda, typename std::enable_if< (  Idx >= sizeof...( T ) ), int >::type = 0 >
        constexpr void Tuple_for_each_with_index_impl( const std::tuple< T... > & tup, const TLambda & callback ) {}

        template< int Idx, typename ... T, typename TLambda, typename std::enable_if< ( Idx < sizeof...( T ) ), int >::type = 0 >
        constexpr void Tuple_for_each_with_index_impl( const std::tuple< T... > & tup, const TLambda & callback ) 
        { 
            return callback( Idx, std::get< Idx >( tup ) ), Tuple_for_each_with_index_impl< Idx + 1 >( tup, callback ); 
        }

        template< typename ... T, typename TLambda >
        constexpr void Tuple_for_each_with_index( const std::tuple< T... > & tup, TLambda && callback ) 
        { 
            return Tuple_for_each_with_index_impl< 0 >( tup, callback );
        }

        template< int Idx, typename ... T, typename TLambda, typename std::enable_if< (  Idx >= sizeof...( T ) ), int >::type = 0 >
        constexpr void Tuple_find_and_call_impl( const int32_t index_to_find, const std::tuple< T... > & tup, const TLambda & callback ) {}

        template< int Idx, typename ... T, typename TLambda, typename std::enable_if< ( Idx < sizeof...( T ) ), int >::type = 0 >
        constexpr void Tuple_find_and_call_impl( const int32_t index_to_find, const std::tuple< T... > & tup, const TLambda & callback ) 
        { 
            if( Idx == index_to_find )
            {
                callback( std::get< Idx >( tup ) );
                return;
            }

            Tuple_find_and_call_impl< Idx + 1 >( index_to_find, tup, callback ); 
        }

        template< typename ... T, typename TLambda >
        constexpr void Tuple_find_and_call( const int32_t index_to_find, const std::tuple< T... > & tup, TLambda && callback ) 
        { 
            return Tuple_find_and_call_impl< 0 >( index_to_find, tup, callback );
        }

        static bool unreachable() { assert( false ); return false; }

        template< typename T >
        struct holder{};

        template < template < typename > class H, typename S >
        void is_predicate_condition( H<S> && );

        template < template < typename > class H, template< typename, uint32_t ... > class Pr, typename P, uint32_t ... Idxs,
            typename std::enable_if< std::is_same< holder< predicate_condition< P, Idxs... > >, H< Pr< P, Idxs... > > >::value, int >::type = 0 >
        bool is_predicate_condition( H< Pr< P, Idxs... > > && );

        template < template < typename > class H, template< typename, typename > class Cnd, typename T1, typename T2,
            typename std::enable_if< std::is_same< holder< condition< T1, T2 > >, H< Cnd< T1, T2 > > >::value, int >::type = 0 >
        bool is_predicate_condition( H< Cnd< T1, T2 > > && );

        template < template < typename > class H, template< typename T1, typename T2 > class Cnds, typename T1, typename T2,
            typename std::enable_if< std::is_same< holder< conditions< T1, T2 > >, H< Cnds< T1, T2 > > >::value, int >::type = 0 >
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
            std::array< T, sizeof...(TArgs) + 1 > arr;

            template< typename T1, typename ... TOthers >
            constexpr Any_from_impl( T1 && arg, TOthers && ... args ) : arr{ std::forward< T1 >( arg ), std::forward< TOthers >( args )... }
            {
                static_assert( std::is_same< T, T1 >::value, "T and T1 should be SAME TYPE" );
            }

            friend constexpr bool operator==( const T & value, const Any_from_impl& st ) 
            {
                return is_in_set( value, st.arr );
            }

            friend constexpr bool operator!=( const T & value, const Any_from_impl& st )
            {
                return !( operator==( value, st ) );
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
        inline bool operator()( const TSrcTuple & src_tuple ) const
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
        static inline bool compare( const Comparison_operators CmpOper, T1 && t1, T2 && t2 )
        {                
            switch( CmpOper )
            {
            case Comparison_operators::equal_:
                return t1 == t2;
            case Comparison_operators::not_equal_:
                return t1 != t2;
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
        inline bool operator()( const TSrcTuple & src_tuple ) const
        {
            return compare( logical_operator, cnd1_( src_tuple ),  cnd2_( src_tuple ) );            
        }

    private:

        static inline bool compare( const Logical_operators LogicalOperator, bool t1, bool t2 )
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

    template< int I, typename T >
    auto operator!=( const Index_< I >& idx, T && rhv )
    {
        return condition< Index_< I >, T >( Comparison_operators::not_equal_, std::forward< T >( rhv ) );
    }

    template< int I, typename T >
    auto operator==( const Index_< I >& idx, T && rhv )
    {
        return condition< Index_< I >, T >( Comparison_operators::equal_, std::forward< T >( rhv ) );
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
        // TODO move to private scope
        TPred pred_;

        template< typename T >
        predicate_condition( T && pred ) 
            : pred_( std::forward< T >( pred ) )
            {                    
            }

        template< typename TSrcTuple >
        inline bool operator()( const TSrcTuple & src_tuple ) const
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

    template< typename R, typename... Args, typename T, typename std::enable_if< details::has_static_member_index_t< T >::value, int >::type = 0 >
    auto operator,( R(*pred)(Args...), const T& t1 )
    {
        return predicate_condition< R(*)(Args...), T::eswitch_index >( pred );
    }

    template< typename Pred, typename T, typename std::enable_if< !details::is_predicate< std::remove_reference_t< Pred > >::value && 
        details::has_static_member_index_t< T >::value, int >::type = 0 >
    auto operator,( Pred&& pred, const T& t1 )
    {
        return predicate_condition< std::remove_reference_t< Pred >, T::eswitch_index >( std::forward< Pred >( pred ) );
    }

    template < typename P, uint32_t ... I, typename T, typename std::enable_if< details::has_static_member_index_t< T >::value, int >::type = 0 >
    predicate_condition< P, I..., T::eswitch_index > compose_new_predicate_condition_type( const predicate_condition< P, I... > & pred_cnd, const T& t1 ); 

    template< typename Pred, typename T, typename std::enable_if< details::is_predicate< std::remove_reference_t< Pred > >::value, int >::type = 0 >
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
    struct Fallthrough {};
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

    template< typename TEswitch >
    struct Eswitch_for_return_only
    {
        TEswitch eswitch_;

        template< typename T >
        Eswitch_for_return_only( T && t ) : eswitch_( std::forward< T >( t ) ){}
        
        template< typename Tlambda, typename std::enable_if< details::is_callable< std::remove_reference_t< Tlambda > >::value, int >::type = 0 >    
        inline auto operator>>( Tlambda && lambda )
        {
            /// after handling lambda TEswitch could change, in particular ReturnValue could change
            return Eswitch_for_return_only< decltype( eswitch_ >> std::forward< Tlambda >( lambda ) ) >( 
                eswitch_ >> std::forward< Tlambda >( lambda ) );            
        }

        template< typename T >
        inline void operator>>( Return_value_impl< T > && lambda )
        {            
            eswitch_ >> std::move( lambda );
        }
    
        template< typename TReturnValue >
        inline auto operator>>( Value_to_return< TReturnValue >&& value )
        {
           return eswitch_ >> std::move( value );
        }

        inline auto operator>>( const In_place_return_value& ret_in_place )
        {
            return eswitch_ >> ret_in_place;            
        }
        
        template< typename T1, typename T2 >
        inline Eswitch_for_return_only operator>>( const conditions< T1, T2 >& cnd)
        {
            static_assert( Always_false< T1, T2 >::value, "You can't use CASE after DEFAULT, only RETURN must go there!" );
        }

        template< typename T1, typename T2 >
        inline Eswitch_for_return_only operator>>( const condition< T1, T2 >& cnd)
        {
            static_assert( Always_false< T1, T2 >::value, "You can't use CASE after DEFAULT, only RETURN must go there!" );
        }
    };

    // to handle match cases without body, since next cases shouldn't be consider.
    template< typename TEswitch >
    struct Eswitch_for_case_only
    {
        TEswitch eswitch_;

        template< typename T >
        Eswitch_for_case_only( T && t ) : eswitch_( std::forward< T >( t ) ){ }
        
        template< typename Tlambda, typename std::enable_if< 
             details::is_callable< std::remove_reference_t< Tlambda > >::value && 
            !details::is_predicate< std::remove_reference_t< Tlambda > >::value, int >::type = 0 >
        inline auto operator>>( Tlambda && lambda )
        {
            return eswitch_ >> std::forward< Tlambda >( lambda ); 
        }

        inline auto operator>>( const Default_impl & default_lambda )
        {
            return eswitch_ >> default_lambda; 
        }

        template< typename TReturnValue >
        inline auto operator>>( Value_to_return< TReturnValue >&& value )
        {
            return eswitch_ >> std::move( value );
        }
        
        template< typename T1, typename T2 >
        inline auto operator>>( const conditions< T1, T2 >& cnds )
        {
            return handle_condition( cnds );
        }

        template< typename T1, typename T2 >
        inline auto operator>>( const condition< T1, T2 >& cnd )
        {
            return handle_condition( cnd );
        }

        template< typename TPred, uint32_t ... Is >
        inline auto operator>>( const predicate_condition< TPred, Is... > & value )
        {
            return handle_condition( value );
        }

    private:
        template< typename TCnd >    
        inline auto handle_condition( const TCnd& cnd )
        {
            if( eswitch_.execute_current_case && !eswitch_.was_case_executed )
                eswitch_ >> []{};           

            eswitch_ >> cnd;

            return std::move( *this );
        }
    };

    template< typename T >
    struct Anything
    {
        using type = T;

        union data
        {
            data(){}
           ~data(){}

            T internals;
        };
    
        data dt;
        bool was_set = false;

        Anything() = default;

        template< typename T_ >
        explicit Anything( T_ && t ) : was_set( true )
        {
            new (&dt.internals) T( std::forward< T_ >( t ) );
        }

        template< typename T_ >
        explicit Anything( T_ && t, bool was_set ) : was_set( was_set )
        {
            if( was_set ) new (&dt.internals) T( std::forward< T_ >( t ) );
        }

        Anything& operator==( Anything& ) = delete;
        Anything& operator==( Anything&& ) = delete;
        Anything( Anything && t ) = delete;
        
       ~Anything()
        {
            if( was_set ) dt.internals.~T();
        }

        inline operator bool() const { return was_set; }

        inline T release() 
        { 
            return std::move( dt.internals ); 
        }

        inline T release_with_check() 
        { 
            if( !was_set ) throw( std::logic_error( "Anything is empty!!!" ) ); 

            was_set = false;
            return std::move( dt.internals ); 
        }
    };

    template< typename R, typename ... TArgs >
    class Eswitch
    {
        std::tuple< TArgs... > pack_;
        Anything< R > return_val_;        
        bool is_return_value_set_ = false;
        bool was_case_executed = false;
        bool execute_current_case = false;
        bool need_fallthrough = false;
        bool need_break = true;

        template< typename T, typename ... Ts >
        friend class Eswitch;

        template< typename TEswitch >
        friend struct Eswitch_for_case_only;

    public:
        template< typename T, typename ... Ts >
        Eswitch( Eswitch< T, Ts... >&& args ) 
            : return_val_( static_cast< R >( args.return_val_.release() ), args.return_val_.was_set )
            , pack_( std::move( args.pack_ ) )
            , is_return_value_set_( return_val_.was_set )
            , was_case_executed( args.was_case_executed )
            , execute_current_case( args.execute_current_case )
            , need_fallthrough( args.need_fallthrough )
            , need_break( args.need_break )
        {
        }

        template< typename ... Ts >
        Eswitch( Eswitch< Padding*, Ts... >&& args )
            : pack_( std::move( args.pack_ ) )
            , is_return_value_set_( false )
            , was_case_executed( args.was_case_executed )
            , execute_current_case( args.execute_current_case )
            , need_fallthrough( args.need_fallthrough )
            , need_break( args.need_break )
        {
        }

        template< typename ... Ts >
        Eswitch( Eswitch< Filled*, Ts... >&& args )
            : return_val_( static_cast< R >( nullptr ), args.return_val_.was_set )
            , pack_( std::move( args.pack_ ) )
            , is_return_value_set_( args.return_val_.was_set )
            , was_case_executed( args.was_case_executed )
            , execute_current_case( args.execute_current_case )
            , need_fallthrough( args.need_fallthrough )
            , need_break( args.need_break )
        {
        }

        template< typename ... T >
        Eswitch( T&& ... args ) : pack_( std::forward< T >( args )... )
        {
        }

        template< typename TR, typename T, typename ... Ts >
        Eswitch( TR&& return_value, Eswitch< T, Ts... >&& args ) 
            : return_val_( std::forward< TR >( return_value ) )
            , pack_( std::move( args.pack_ ) )
            , is_return_value_set_( true )
            , was_case_executed( args.was_case_executed )
            , execute_current_case( args.execute_current_case )
            , need_fallthrough( args.need_fallthrough )
            , need_break( args.need_break )
        {
        }

        inline Eswitch operator>>( const Fallthrough& )
        {
            if( was_case_executed && execute_current_case ) { need_fallthrough = true; need_break = false; }

            return std::move( *this );
        }
        
        inline auto operator>>( const In_place_return_value& )
        {
            if( !is_return_value_set_ ) throw( std::logic_error( "None of the cases returned Anything!" ) );

            return return_val_.release_with_check();
        }

        template< typename T >
        inline void operator>>( Return_value_impl< T > && lambda )
        {
            if( is_return_value_set_ ) lambda.callback( return_val_.release_with_check() );
        }

        inline auto operator>>( const Default_impl & default_lambda )
        {
            return Eswitch_for_return_only< decltype( *this >> default_lambda.case_for_any_match ) >( *this >> default_lambda.case_for_any_match );
        }

        template< typename Tlambda, typename std::enable_if< details::is_callable< std::remove_reference_t< Tlambda > >::value && std::is_same< other_details::return_type_t< std::remove_reference_t< Tlambda > >, void >::value, int >::type = 0 >
        inline Eswitch operator>>( Tlambda && lambda )
        {
            if( is_return_value_set_ ) return std::move( *this );

            if( execute_current_case && !was_case_executed ) 
            {
                lambda();
                was_case_executed = execute_current_case;
            }
            else if( need_fallthrough ) 
            {
                lambda();
                need_fallthrough = false;
                need_break = true;
                execute_current_case = true;
            }

            return std::move( *this );
        }

        template< typename Tlambda, typename std::enable_if< details::is_callable< std::remove_reference_t< Tlambda > >::value && !std::is_same< other_details::return_type_t< std::remove_reference_t< Tlambda > >, void >::value, int >::type = 0 >
        inline auto operator>>( Tlambda && lambda )
        {
            if( is_return_value_set_ ) 
                return decltype( handle_return_value( lambda() ) )( std::move( *this ) );

            if( execute_current_case || need_fallthrough )
                return handle_return_value( lambda() );
            else
                return decltype( handle_return_value( lambda() ) )( std::move( *this ) );
        }

        template< typename T1, typename T2 >
        inline auto operator>>( const conditions< T1, T2 >& cnds )
        {            
            static_assert( !conditions< T1, T2 >::template is_out_of_range< sizeof...( TArgs ) >(), "Index in 'Predicate' is out of range!!" );

            return Eswitch_for_case_only< Eswitch >( handle_condition( cnds ) );
        }

        template< typename T1, typename T2 >
        inline auto operator>>( const condition< T1, T2 >& cnd)
        {
            return Eswitch_for_case_only< Eswitch >( handle_condition( cnd ) );
        }
            
        template< typename TPred, uint32_t ... Is >
        inline auto operator>>( const predicate_condition< TPred, Is... > & value )
        {
            static_assert( !predicate_condition< TPred, Is... >::template is_out_of_range< sizeof...( TArgs ) >(), "Index in 'Predicate' is out of range!!" );
            
            return Eswitch_for_case_only< Eswitch >( handle_condition( value ) );
        }

        template< typename TReturnValue >
        inline auto operator>>( Value_to_return< TReturnValue >&& value )
        {
            if( is_return_value_set_ ) return decltype( handle_return_value( std::move( value.return_value_ ) ) )( std::move( *this ) );

            return handle_return_value( std::move( value.return_value_ ) );           
        }

    private:
        
        template< typename TReturnValue, typename std::enable_if< 
            !Always_false< TReturnValue >::value &&
          ( !std::is_same< R, Padding* >::value && 
             std::is_convertible< R, TReturnValue >::value ) || 
             std::is_same< R, Filled* >::value ||
             std::is_same< R, Padding* >::value, int >::type = 0 >
        inline auto handle_return_value( TReturnValue && value )
        {
            return actual_handle_return_value( std::forward< TReturnValue >( value ) );
        }
        
        template< typename TReturnValue, typename std::enable_if< 
            !Always_false< TReturnValue >::value &&
            !std::is_same< R, Padding* >::value &&
             std::is_convertible< TReturnValue, R >::value &&
            !std::is_convertible< R, TReturnValue >::value, int >::type = 0 >
        inline auto handle_return_value( TReturnValue && value )
        {
            return actual_handle_return_value( static_cast< R&& >( value ) );
        }


        template< typename TReturnValue, typename std::enable_if<  
            !Always_false< TReturnValue >::value &&
            !std::is_convertible< R, TReturnValue >::value && 
            !std::is_convertible< TReturnValue, R >::value && 
            !std::is_same< R, Padding* >::value && 
            details::has_common_type< R, TReturnValue >::value, int >::type = 0 >
        inline auto handle_return_value( TReturnValue && value )
        {
            return actual_handle_return_value( static_cast< std::common_type_t< R, TReturnValue >&& >( value ) );
        }

        inline auto handle_return_value( std::nullptr_t && value )
        {
            return actual_handle_return_value( static_cast< typename std::conditional< std::is_same< R, Padding* >::value, Filled*, R >::type >( nullptr ) );
        }

        template< typename TReturnValue >
        inline auto actual_handle_return_value( TReturnValue && value )
        {            
            if( execute_current_case && !was_case_executed ) 
            {
                was_case_executed = execute_current_case;

                return Eswitch< TReturnValue, TArgs... >( std::forward< TReturnValue >( value ), std::move( *this ) );
            }
            else if( need_fallthrough ) 
            {
                need_fallthrough = false;
                need_break = true;

                return Eswitch< TReturnValue, TArgs... >( std::forward< TReturnValue >( value ), std::move( *this ) );
            }
            
            return Eswitch< TReturnValue, TArgs... >( std::move( *this ) );
        } 

        template< typename T >
        inline auto handle_condition( const T & cnd )
        {
            execute_current_case = false;
            
            if( was_case_executed && ( need_break || need_fallthrough ) ) return std::move( *this );

            execute_current_case = cnd( pack_ );

            return std::move( *this );
        }
    };

    template< typename ... TArgs >
    inline auto eswitch( TArgs && ... args )
    { 
        return Eswitch< Padding*, TArgs... >( std::forward< TArgs >( args )... ); 
    }

    template< typename T1, typename T2 >
    inline condition< T1, T2 > case_( condition< T1, T2 > && cnd )
    { 
        return std::move( cnd ); 
    }

    template< typename T1, typename T2 >
    inline conditions< T1, T2 > case_( conditions< T1, T2 > && cnds )
    { 
        return std::move( cnds ); 
    }

    template< typename TPred, uint32_t ... Is >
    inline auto case_( predicate_condition< TPred, Is... > && value )
    { 
        return std::move( value );  
    }

    template< typename T >
    inline auto case_( T && value )
    { 
        return _1 == std::forward< T >( value ); 
    }

    template< typename ... TArgs >
    inline auto any_from( TArgs&& ... args )
    {
        return extension::Any_from_impl< TArgs... >( std::forward< TArgs >( args )... );
    }
    
    template< typename T >
    inline auto handle_return( T && lmbd )
    { 
        return Return_value_impl< T >{ std::forward< T >( lmbd ) };
    }

    template< typename T >
    inline auto to_return( T && value )
    { 
        using return_t = decltype( details::Just_find_out_return_type( std::forward< T >( value ) ) );

        return Value_to_return< return_t >{ std::forward< T >( value ) };
    }

    /// static declarations
    static const In_place_return_value in_place_return_; // return_in_place_;
    static const Default_impl default_;
    static const Fallthrough fallthrough_;

} // namespace eswitch_v4

#define CASE_OVERLOAD( TCase, TCmp ) \
namespace eswitch_v4 \
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
\
    template< int I >\
    auto operator!=( const eswitch_v4::Index_< I > & val, TCase & rgx )\
    {\
        return val != TCmp( rgx );\
    }\
\
    template< int I >\
    auto operator!=( const eswitch_v4::Index_< I > & val, const TCase & rgx )\
    {\
        return val != TCmp( rgx );\
    }\
\
    template< int I >\
    auto operator!=( const eswitch_v4::Index_< I > & val, TCase && rgx )\
    {\
        return val != TCmp( std::move( rgx ) );\
    }\
}