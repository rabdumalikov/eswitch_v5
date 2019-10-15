#pragma once

#include <tuple>
#include <array>

namespace eswitch_v4
{
    template< int32_t Idx >
    struct Index_{ static constexpr int32_t index = Idx; };

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

    namespace details 
    {
        template <typename T, bool >
        class is_callable_impl;

        template< typename T >
        class is_callable_impl< T, true >
        {
            struct Fallback { int operator()() { return 1; } };
            struct Derived : T, Fallback { };

            template<typename U, U> struct Check;

            struct Yes{};

            struct No
            {
                Yes no[2];
            };

            template<typename U>
            static No has_alternative_callable_operator(Check< int(Fallback::*)(), &U::operator()> *);

            template<typename U>
            static Yes has_alternative_callable_operator(...);

        public:
            static constexpr int value = sizeof(has_alternative_callable_operator<Derived>(0)) == sizeof(Yes);
        };


        template <typename T >
        class is_callable_impl< T, false >
        {
        public:
            static constexpr int value = false;
        };

        template <typename T >
        class is_callable
        {
        public:
            static constexpr int value = is_callable_impl< T, std::is_class< T >::value >::value;
        };

        template< int Idx, typename ... T, typename TLambda, typename std::enable_if< (  Idx >= sizeof...( T ) ), int >::type = 0 >
        constexpr void tuple_for_each_impl( const std::tuple< T... > & tup, const TLambda & callback ) 
        {
        }

        template< int Idx, typename ... T, typename TLambda, typename std::enable_if< ( Idx < sizeof...( T ) ), int >::type = 0 >
        constexpr void tuple_for_each_impl( const std::tuple< T... > & tup, const TLambda & callback ) 
        { 
            return callback( std::get< Idx >( tup ) ), tuple_for_each_impl< Idx + 1 >( tup, callback ); 
        }

        template< typename ... T, typename TLambda >
        constexpr void tuple_for_each( const std::tuple< T... > & tup, TLambda && callback ) 
        { 
            return tuple_for_each_impl< 0 >( tup, callback );
        }

        template< int Idx, typename ... T, typename TLambda, typename std::enable_if< (  Idx >= sizeof...( T ) ), int >::type = 0 >
        constexpr void tuple_for_each_with_index_impl( const std::tuple< T... > & tup, const TLambda & callback ) {}

        template< int Idx, typename ... T, typename TLambda, typename std::enable_if< ( Idx < sizeof...( T ) ), int >::type = 0 >
        constexpr void tuple_for_each_with_index_impl( const std::tuple< T... > & tup, const TLambda & callback ) 
        { 
            return callback( Idx, std::get< Idx >( tup ) ), tuple_for_each_with_index_impl< Idx + 1 >( tup, callback ); 
        }

        template< typename ... T, typename TLambda >
        constexpr void tuple_for_each_with_index( const std::tuple< T... > & tup, TLambda && callback ) 
        { 
            return tuple_for_each_with_index_impl< 0 >( tup, callback );
        }

        template< int Idx, typename ... T, typename TLambda, typename std::enable_if< (  Idx >= sizeof...( T ) ), int >::type = 0 >
        constexpr void tuple_find_and_call_impl( const int32_t index_to_find, const std::tuple< T... > & tup, const TLambda & callback ) {}

        template< int Idx, typename ... T, typename TLambda, typename std::enable_if< ( Idx < sizeof...( T ) ), int >::type = 0 >
        constexpr void tuple_find_and_call_impl( const int32_t index_to_find, const std::tuple< T... > & tup, const TLambda & callback ) 
        { 
            if( Idx == index_to_find )
            {
                callback( std::get< Idx >( tup ) );
                return;
            }

            tuple_find_and_call_impl< Idx + 1 >( index_to_find, tup, callback ); 
        }

        template< typename ... T, typename TLambda >
        constexpr void tuple_find_and_call( const int32_t index_to_find, const std::tuple< T... > & tup, TLambda && callback ) 
        { 
            return tuple_find_and_call_impl< 0 >( index_to_find, tup, callback );
        }
    }

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
    }

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
            friend constexpr bool operator==( const any& st, const T& value ) {
                return true;
            }

            template < typename T >
            friend constexpr bool operator==( const T& value, const any& st ) {
                return true;
            }

            template < typename T >
            friend constexpr bool operator!=( const any& st, const T& value ) {
                return false;
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

            friend constexpr bool operator==( const Any_from_impl& st, const T & value )
            {
                return is_in_set( value, st.arr );
            }

            friend constexpr bool operator==( const T & value, const Any_from_impl& st )
            {
                return is_in_set( value, st.arr );
            }

            friend constexpr bool operator!=( const Any_from_impl& st, const T & value )
            {
                return !operator==( value, st );
            }

            friend constexpr bool operator!=( const T & value, const Any_from_impl& st )
            {
                return !operator==( value, st );
            }
        };        
    }

    template< typename TIndex, typename T >
    class condition
    {
        std::tuple< T > tup_value_;
        Comparison_operators cmp_operator;
    public:

        template< typename ... TT >
        friend class conditions;

        template< typename TVal >
        constexpr condition( const Comparison_operators cmp_type, TVal&& value ) 
        : cmp_operator( cmp_type ), tup_value_( std::forward< TVal >( value ) )
        { 
        }

        template< typename TSrcTuple >
        bool handle( const TSrcTuple & src_tuple ) const
        {
            return compare( cmp_operator, std::get< TIndex::index >( src_tuple ), std::get< 0 >( tup_value_ ) );         
        }

        int32_t amount_cmp() const { return 1; }       

    private:
        template< typename T1, typename T2 >
        static bool compare( const Comparison_operators CmpOper, T1 && t1, T2 && t2 )
        {                
            switch( CmpOper )
            {
            case Comparison_operators::equal_:
                return t1 == t2;
            case Comparison_operators::not_equal_:
                return t1 != t2;
            };
        }
    };

    template< typename ... T >
    class conditions
    {
        std::tuple< T... > pack_;
        Logical_operators logical_operator;
    public:

        template< typename ... TVals >
        conditions( const Logical_operators logical_operator, TVals&&... value ) : logical_operator( logical_operator ), pack_( std::forward< TVals >( value )... )
        {}

        constexpr int32_t amount_cmp() const 
        { 
            int32_t sum_of_cmp = 0;

            details::tuple_for_each( pack_, [&]( const auto & v ) 
                {
                    sum_of_cmp += v.amount_cmp();
                } );

            return sum_of_cmp;
        }
     
        template< typename TSrcTuple >
        bool handle( const TSrcTuple & src_tuple ) const
        {
            return compare( logical_operator, std::get< 0 >( pack_ ).handle( src_tuple ),  std::get< 1 >( pack_ ).handle( src_tuple ) );            
        }

    private:
        template< typename T1, typename T2 >
        static bool compare( const Logical_operators LogOper, T1 && t1, T2 && t2 )
        {                
            switch( LogOper )
            {
            case Logical_operators::and_:
                return t1 && t2;
            case Logical_operators::or_:
                return t1 || t2;
            };
        }
    };

    template< typename T >
    auto operator!=( const Index_< 0 >& idx, T && rhv )
    {
        return condition< Index_< 0 >, T >( Comparison_operators::not_equal_, std::forward< T >( rhv ) );
    }
    template< typename T >
    auto operator!=( const Index_< 1 >& idx, T && rhv )
    {
        return condition< Index_< 1 >, T >( Comparison_operators::not_equal_, std::forward< T >( rhv ) );
    }
    template< typename T >
    auto operator!=( const Index_< 2 >& idx, T && rhv )
    {
        return condition< Index_< 2 >, T >( Comparison_operators::not_equal_, std::forward< T >( rhv ) );
    }
    template< typename T >
    auto operator!=( const Index_< 3 >& idx, T && rhv )
    {
        return condition< Index_< 3 >, T >( Comparison_operators::not_equal_, std::forward< T >( rhv ) );
    }
    template< typename T >
    auto operator!=( const Index_< 4 >& idx, T && rhv )
    {
        return condition< Index_< 4 >, T >( Comparison_operators::not_equal_, std::forward< T >( rhv ) );
    }
    template< typename T >
    auto operator!=( const Index_< 5 >& idx, T && rhv )
    {
        return condition< Index_< 5 >, T >( Comparison_operators::not_equal_, std::forward< T >( rhv ) );
    }

    template< typename T >
    auto operator==( const Index_< 0 >& idx, T && rhv )
    {
        return condition< Index_< 0 >, T >( Comparison_operators::equal_, std::forward< T >( rhv ) );
    }
    template< typename T >
    auto operator==( const Index_< 1 >& idx, T && rhv )
    {
        return condition< Index_< 1 >, T >( Comparison_operators::equal_, std::forward< T >( rhv ) );
    }
    template< typename T >
    auto operator==( const Index_< 2 >& idx, T && rhv )
    {
        return condition< Index_< 2 >, T >( Comparison_operators::equal_, std::forward< T >( rhv ) );
    }
    template< typename T >
    auto operator==( const Index_< 3 >& idx, T && rhv )
    {
        return condition< Index_< 3 >, T >( Comparison_operators::equal_, std::forward< T >( rhv ) );
    }
    template< typename T >
    auto operator==( const Index_< 4 >& idx, T && rhv )
    {
        return condition< Index_< 4 >, T >( Comparison_operators::equal_, std::forward< T >( rhv ) );
    }
    template< typename T >
    auto operator==( const Index_< 5 >& idx, T && rhv )
    {
        return condition< Index_< 5 >, T >( Comparison_operators::equal_, std::forward< T >( rhv ) );
    }


    template< typename T1, typename T2 >
    auto operator&&( T1 && i, T2 && j )
    {
        return conditions< T1, T2 >( Logical_operators::and_, std::forward< T1 >( i ), std::forward< T2 >( j ) );
    }

    template< typename T1, typename T2 >
    auto operator||( T1 && i, T2 && j )
    {
        return conditions< T1, T2 >( Logical_operators::or_, std::forward< T1 >( i ), std::forward< T2 >( j ) );
    }

    template< typename T >
    struct Return_value_impl
    {
        T callback;
    };

    struct In_place_return_value{};
    static In_place_return_value in_place_return_;

    struct Fallthrough {};
    static Fallthrough fallthrough_;

    struct Padding{};

    template< typename T >
    T case_( T && cnd );

    struct Default_impl
    {
        condition< Index_< 0 >, extension::any > case_for_any_match = case_( _1 == extension::any() );
    };

    template< typename ... Ts >
    struct Always_assert
    {
 	    static constexpr bool value = false;
    };


    template< typename ... Ts >
    struct Common_type;

    template< typename T >
    struct Common_type< T >{ using type = T; };

    template< typename T1, typename T2 >
    struct Common_type< T1, T2 >{ using type = std::common_type_t< T1, T2 >; };

    template<>
    struct Common_type< std::nullptr_t >{ using type = Padding*; };

    template< typename T >
    struct Common_type< Padding, T >{ using type = typename Common_type< T >::type; };

    template< typename T >
    struct Common_type< T, Padding >{ using type = typename Common_type< T >::type; };

    template< typename T >
    struct Common_type< Padding*, T >{ using type = typename Common_type< T >::type; };

    template< typename T >
    struct Common_type< T, Padding* >{ using type = typename Common_type< T >::type; };


    template< typename ... Ts >
    using Common_type_t = typename Common_type< Ts... >::type;


    template< typename TEswitch >
    struct _Eswitch_for_return_only
    {
        TEswitch eswitch_;

        template< typename T >
        _Eswitch_for_return_only( T && t ) : eswitch_( std::forward< T >( t ) ){}
        
        template< typename Tlambda, typename std::enable_if< details::is_callable< Tlambda >::value, int >::type = 0 >
        auto operator>>( Tlambda && lambda )
        {
            /// after handling lambda TEswitch could change, in particular ReturnValue could change
            return _Eswitch_for_return_only< decltype( eswitch_ >> std::move( lambda ) ) >( std::move( eswitch_ >> std::move( lambda ) ) );            
        }

        template< typename T >
        void operator>>( Return_value_impl< T > && lambda )
        {            
            eswitch_ >> std::move( lambda );
        }

        auto operator>>( const In_place_return_value& ret_in_place )
        {
            return eswitch_ >> ret_in_place;            
        }

        template< typename ... Ts >
        _Eswitch_for_return_only operator>>( const conditions< Ts... >& cnd)
        {
            static_assert( Always_assert< Ts... >::value, "You can't use CASE after DEFAULT, only RETURN must go there!" );
        }

        template< typename T1, typename T2 >
        _Eswitch_for_return_only operator>>( const condition< T1, T2 >& cnd)
        {
            static_assert( Always_assert< T1, T2 >::value, "You can't use CASE after DEFAULT, only RETURN must go there!" );
        }
    };

    template< typename TValue >
    struct Value_to_return{
        explicit Value_to_return( TValue && val ) : return_value_( std::move( val ) ){}
        TValue return_value_;
    };

    template< typename T >
    struct anything
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

        anything() = default;

        template< typename T_ >
        anything( std::initializer_list< T_ > && t ) : was_set( true )
        {
            new (&dt.internals) std::decay_t< T_ >( std::move( *t.begin() ) );
        }

       ~anything()
        {
            if( was_set ) dt.internals.~T();
        }

        template< typename T_ >
        anything( const anything< T_ > & other ) : was_set( other.was_set )
        {
            if( other.was_set )
            {
                new (&dt.internals) std::decay_t< T_ >( other.dt.internals );
            }
        }

        template< typename T_ >
        anything( anything< T_ > && other ) : was_set( other.was_set )
        {
            if( other.was_set )
            {
                new (&dt.internals) std::decay_t< T_ >( std::move( other.dt.internals ) );
            }
        }

        operator bool() const { return was_set; }

        T reset() 
        { 
            was_set = false;
            return std::move( dt.internals ); 
        }

        const T& front() const 
        { 
            if( !was_set ) throw( std::logic_error( "anything is empty!!!" ) ); 

            return dt.internals; 
        }
    };

    template< typename R, typename ... TArgs >
    class Eswitch
    {
        std::tuple< TArgs... > pack_;
        anything< R > return_val_;        
        bool is_return_value_set_ = false;
        bool was_case_executed = false;
        bool execute_current_case = false;
        bool need_fallthrough = false;
        bool need_break = true;

        template< typename T, typename ... Ts >
        friend class Eswitch;

    public:
        template< typename T, typename ... Ts >
        Eswitch( Eswitch< T, Ts... >&& args ) 
            : return_val_{ static_cast< R >( args.return_val_.reset() ) }
            , pack_( std::move( args.pack_ ) )
            , is_return_value_set_( true )
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

        template< typename ... T >
        Eswitch( T&& ... args ) : pack_( std::forward< T >( args )... )
        {
        }

        template< typename TR, typename T, typename ... Ts >
        Eswitch( TR&& return_value, Eswitch< T, Ts... >&& args ) 
            : return_val_{ static_cast< R >( std::forward< TR >( return_value ) ) }
            , pack_( std::move( args.pack_ ) )
            , is_return_value_set_( true )
            , was_case_executed( args.was_case_executed )
            , execute_current_case( args.execute_current_case )
            , need_fallthrough( args.need_fallthrough )
            , need_break( args.need_break )
        {
            printf( "RUSTAM is_return_value_set_=TRUE\n" );
        }

        Eswitch operator>>( const Fallthrough& )
        {
            if( was_case_executed ) { need_fallthrough = true; need_break = false; }

            return std::move( *this );
        }
        
        auto operator>>( const In_place_return_value& )
        {
            if( !is_return_value_set_ ) throw( std::logic_error( "None of the cases returned anything!" ) );

            return return_val_.front();
        }

        template< typename T >
        void operator>>( Return_value_impl< T > && lambda )
        {
            if( is_return_value_set_ ) lambda.callback( return_val_.front() );
        }

        auto operator>>( const Default_impl & default_lambda )
        {
            return _Eswitch_for_return_only< Eswitch >( *this >> default_lambda.case_for_any_match );
        }

        template< typename Tlambda, typename std::enable_if< details::is_callable< std::remove_reference_t< Tlambda > >::value && std::is_same< other_details::return_type_t< std::remove_reference_t< Tlambda > >, void >::value, int >::type = 0 >
        Eswitch operator>>( Tlambda && lambda )
        {
            if( is_return_value_set_ ) return std::move( *this );

            if( execute_current_case ) 
            {
                lambda();
                was_case_executed = execute_current_case;
                execute_current_case = false;
            }
            else if( need_fallthrough ) 
            {
                lambda();
                need_fallthrough = false;
                need_break = true;
            }

            return std::move( *this );
        }


        template< typename Tlambda, typename std::enable_if< details::is_callable< std::remove_reference_t< Tlambda > >::value && !std::is_same< other_details::return_type_t< std::remove_reference_t< Tlambda > >, void >::value, int >::type = 0 >
        auto operator>>( Tlambda && lambda )
        {
            using TReturnValue = other_details::return_type_t< std::remove_reference_t< Tlambda > >;

            using new_return_type_t = typename std::conditional< 
                std::is_same< R, Padding* >::value, 
                Common_type_t< std::remove_reference_t< TReturnValue > >, 
                    Common_type_t< std::remove_reference_t< TReturnValue >, R > >::type;

            if( is_return_value_set_ )  return Eswitch< new_return_type_t, TArgs... >( std::move( *this ) );

            if( execute_current_case ) 
            {
                was_case_executed = execute_current_case;
                execute_current_case = false;

                return Eswitch< new_return_type_t, TArgs... >( lambda(), std::move( *this ) );
            }
            else if( need_fallthrough ) 
            {
                need_fallthrough = false;
                need_break = true;

                return Eswitch< new_return_type_t, TArgs... >( lambda(), std::move( *this ) );
            }
            
            return Eswitch< new_return_type_t, TArgs... >( std::move( *this ) );
        }

        template< typename ... Ts >
        Eswitch operator>>( const conditions< Ts... >& cnd)
        {
            return handle_condition( cnd );
        }

        template< typename T1, typename T2 >
        Eswitch operator>>( const condition< T1, T2 >& cnd)
        {
            return handle_condition( cnd );
        }
            
        template< typename TReturnValue >
        auto operator>>( Value_to_return< TReturnValue >&& value )
        {
            //static_assert( std::is_same< R, Padding* >::value || std::is_convertible< 
                //Common_type_t< std::remove_reference_t< TReturnValue > >,
                //Common_type_t< std::remove_reference_t< TReturnValue >, R > >::value,
                //"ReturnValue across all CASEs should have same type" );

            // static_assert( std::is_same< R, Padding* >::value || std::is_convertible< 
            //     Common_type_t< std::remove_reference_t< TReturnValue >, R >, R >::value,
            //     "ReturnValue across all CASEs should have same type" );

            printf( "RUSTAM that operator\n");

            using new_return_type_t = typename std::conditional< 
                std::is_same< R, Padding* >::value, 
                Common_type_t< std::remove_reference_t< TReturnValue > >, 
                    Common_type_t< std::remove_reference_t< TReturnValue >, R > >::type;

            if( is_return_value_set_ )  return Eswitch< new_return_type_t, TArgs... >( std::move( *this ) );

            if( execute_current_case ) 
            {
                printf( "RUSTAM that operator is_same\n" );

                was_case_executed = execute_current_case;
                execute_current_case = false;

                return Eswitch< new_return_type_t, TArgs... >( std::move( value.return_value_ ), std::move( *this ) );
            }
            else if( need_fallthrough ) 
            {
                printf( "RUSTAM that operator2\n");

                need_fallthrough = false;
                need_break = true;

                return Eswitch< new_return_type_t, TArgs... >( std::move( value.return_value_ ), std::move( *this ) );
            }
            
            return Eswitch< new_return_type_t, TArgs... >( std::move( *this ) );
        }

    private:

        template< typename T >
        Eswitch handle_condition( T && cnd )
        {
            if( was_case_executed && ( need_break || need_fallthrough ) ) return std::move( *this );

            execute_current_case = cnd.handle( pack_ );
            
            return std::move( *this );
        }
    };

    template< typename ... TArgs >
    auto eswitch( TArgs && ... args )
    { 
        return Eswitch< Padding*, TArgs... >( std::forward< TArgs >( args )... ); 
    }

    template< typename T >
    T case_( T && cnd )
    { 
        return std::forward< T >( cnd ); 
    }

    template< typename ... TArgs >
    auto any_from( TArgs&& ... args )
    {
        return extension::Any_from_impl< TArgs... >( std::forward< TArgs >( args )... );
    }

    static auto default_ = Default_impl();
    
    template< typename T >
    auto handle_return( T && lmbd )
    { 
        return  Return_value_impl< T >{ std::forward< T >( lmbd ) };
    }

    template< typename T >
    auto dumb( T && value ){ return value; }

    template< typename T >
    auto to_return( T && value )
    { 
        using new_type = decltype( dumb( value ) );
        return  Value_to_return< new_type >{ std::forward< new_type >( value ) };
    }
 }