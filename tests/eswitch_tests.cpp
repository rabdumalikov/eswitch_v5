//  Copyright 2019 Rustam Abdumalikov.
//
//  "eswitch_v4" library
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include "eswitch_v4.hpp"
#include "gtest/gtest.h"
#include <memory>
#include <sstream>

enum Place { unknown = 0, new_york=5, washington=129, new_jersey=501 };

TEST(eswitch_v4_case, one_condition_1st_param_EQUAL)
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, one_condition_2nd_param_EQUAL)
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _2 == new_jersey ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, one_condition_3rd_param_EQUAL)
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _3 == new_york ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, one_condition_1st_param_NOT_EQUAL)
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != new_jersey ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, one_condition_2nd_param_NOT_EQUAL)
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _2 != washington ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, one_condition_3rd_param_NOT_EQUAL)
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _3 != new_jersey ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}


TEST(eswitch_v4_case, two_condition_1st_and_2nd_param_EQUAL_via_AND)
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington && _2 == new_jersey ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, two_condition_2nd_and_1st_param_EQUAL_via_AND)
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _2 == new_jersey && _1 == washington ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, two_condition_1st_and_3rd_param_EQUAL_via_AND)
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington && _3 == new_york ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, two_condition_3rd_and_1st_param_EQUAL_via_AND)
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _3 == new_york  && _1 == washington ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, two_condition_2nd_and_3rd_param_EQUAL_via_AND)
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _2 == new_jersey && _3 == new_york ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, two_condition_3rd_and_2nd_param_EQUAL_via_AND)
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _3 == new_york && _2 == new_jersey ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, two_condition_1st_and_2nd_param_EQUAL_via_OR_first_true )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington || _2 == new_york ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, two_condition_1st_and_2nd_param_EQUAL_via_OR_second_true )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == new_york || _2 == new_jersey ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, two_condition_1st_and_3rd_param_EQUAL_via_OR_first_true )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington || _3 == new_jersey ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, two_condition_1st_and_3rd_param_EQUAL_via_OR_second_true )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == new_jersey || _3 == new_york ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, two_condition_2nd_and_3rd_param_EQUAL_via_OR_first_true )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _2 == new_jersey || _3 == washington ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, two_condition_2nd_and_3rd_param_EQUAL_via_OR_second_true )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _2 == washington || _3 == new_york ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}


TEST(eswitch_v4_case, 3_cases_1st_true )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> [&](){ executed = true; }
        >> case_( _2 == washington ) >> [&](){ FAIL(); }
        >> case_( _3 == washington ) >> [&](){ FAIL(); };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, 3_cases_2nd_true )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == new_jersey ) >> [&](){ FAIL(); }
        >> case_( _2 == new_jersey ) >> [&](){ executed = true; }
        >> case_( _3 == new_jersey ) >> [&](){ FAIL(); };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, 3_cases_3rd_true )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == new_york ) >> [&](){ FAIL(); }
        >> case_( _2 == new_york ) >> [&](){ FAIL(); }
        >> case_( _3 == new_york ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, 3_complex_cases_1st_true )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington && _2 == new_jersey && _3 == new_york ) >> [&](){ executed = true; }
        >> case_( _1 == washington || _2 == new_jersey || _3 == new_york ) >> [&](){ FAIL(); }
        >> case_( _1 != washington && _2 != new_jersey && _3 != new_york ) >> [&](){ FAIL(); };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, 3_complex_cases_2nd_true )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington && _2 == new_jersey && _3 == new_york ) >> [&](){ FAIL(); }
        >> case_( _1 == washington || _2 == new_jersey || _3 == new_york ) >> [&](){ executed = true; }
        >> case_( _1 != washington && _2 != new_jersey && _3 != new_york ) >> [&](){ FAIL(); };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, 3_complex_cases_3rd_true )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington && _2 == new_jersey && _3 == new_york ) >> [&](){ FAIL(); }
        >> case_( _1 != washington || _2 != new_jersey || _3 != new_york ) >> [&](){ FAIL(); }
        >> case_( _1 == washington && _2 == new_jersey && _3 == new_york ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, all_source_entries_have_different_type )
{
    using namespace eswitch_v4;

    bool executed = false;

    const double value = 2.5;

    eswitch( washington, std::string( "Hello" ), value )
        >> case_( _1 == washington && _2 == "Hello" && _3 == 2.5 ) >> [&](){ executed = true; }
        >> case_( _1 == washington && _2 == "Hello" && _3 == 2.5 ) >> [&](){ FAIL(); };
    
    ASSERT_EQ(executed, true );
}


TEST(eswitch_v4_case, 1st_match_not_equal_any_from )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != any_from( new_jersey, new_york ) ) >> [&](){ executed = true; }
        >> case_( _2 == any_from( washington, new_jersey, new_york ) ) >> [&](){ FAIL(); }
        >> case_( _3 == any_from( washington, new_jersey, new_york ) ) >> [&](){ FAIL(); };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, 2nd_match_not_equal_any_from )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == any_from( new_jersey, new_york ) ) >> [&](){ FAIL(); }
        >> case_( _2 != any_from( washington, new_york ) ) >> [&](){ executed = true; }
        >> case_( _3 == any_from( washington, new_jersey, new_york ) ) >> [&](){ FAIL(); };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, 3rd_match_not_equal_any_from )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == any_from( new_jersey, new_york ) ) >> [&](){ FAIL(); }
        >> case_( _2 == any_from( washington, new_york ) ) >> [&](){ FAIL(); }
        >> case_( _3 != any_from( washington, new_jersey ) ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, 1st_match_any_from )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == any_from( washington, new_jersey, new_york ) ) >> [&](){ executed = true; }
        >> case_( _2 == any_from( washington, new_jersey, new_york ) ) >> [&](){ FAIL(); }
        >> case_( _3 == any_from( washington, new_jersey, new_york ) ) >> [&](){ FAIL(); };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, 2nd_match_any_from )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != any_from( washington, new_jersey, new_york ) ) >> [&](){ FAIL(); }
        >> case_( _2 == any_from( washington, new_jersey, new_york ) ) >> [&](){ executed = true; }
        >> case_( _3 == any_from( washington, new_jersey, new_york ) ) >> [&](){ FAIL(); };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, 3rd_match_any_from )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != any_from( washington, new_jersey, new_york ) ) >> [&](){ FAIL(); }
        >> case_( _2 != any_from( washington, new_jersey, new_york ) ) >> [&](){ FAIL(); }
        >> case_( _3 == any_from( washington, new_jersey, new_york ) ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_case, default_match_any_from )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != any_from( washington, new_jersey, new_york ) ) >> [&](){ FAIL(); }
        >> case_( _2 != any_from( washington, new_jersey, new_york ) ) >> [&](){ FAIL(); }
        >> case_( _3 != any_from( washington, new_jersey, new_york ) ) >> [&](){ FAIL(); }
        >> default_ >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

struct index_value 
{
    int i; 
    friend bool operator==( const index_value& lhv, const index_value& rhv ) { return lhv.i == rhv.i; }
    friend bool operator==( const index_value& lhv, const int rhv ) { return lhv.i == rhv; }    
    friend bool operator!=( const index_value& lhv, const index_value& rhv ) { return lhv.i != rhv.i; }
    friend bool operator!=( const index_value& lhv, const int rhv ) { return lhv.i != rhv; }    
};

TEST(eswitch_v4_case, all_source_entries_have_different_type_1_is_custom )
{
    using namespace eswitch_v4;

    bool executed_case_1 = false;
    bool executed_case_2 = false;

    ASSERT_EQ( index_value{ 10 }, index_value{ 10 } );
    ASSERT_EQ( index_value{ 10 }, 10 );
   
    std::vector< int > arg1{ 1, 2 };
    std::string arg2{ "Hello" };
    index_value arg3{ 10 };

    eswitch( arg1, arg2, arg3 )
      >> case_( _1 == std::vector< int >{ 1, 2 } && _2 == "Hello" && _3 == index_value{ 10 } ) >> [&](){ executed_case_1 = true; } >> fallthrough_
      >> case_( _1 == std::vector< int >{ 1, 2 } && _2 == "Hello" && _3 == 10 ) >> [&](){ executed_case_2 = true; };
    
    ASSERT_EQ(executed_case_1, true );
    ASSERT_EQ(executed_case_2, true );
}

TEST(eswitch_v4_implicit_break, 1st_case_true_other_inaccessable )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> [&](){ executed = true; }
        >> case_( _1 == washington ) >> [&](){ FAIL(); }
        >> case_( _1 == washington ) >> [&](){ FAIL(); };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_implicit_break, 2nd_case_true_other_inaccessable )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == new_jersey ) >> [&](){ FAIL(); }
        >> case_( _2 == new_jersey ) >> [&](){ executed = true; }
        >> case_( _3 == new_jersey ) >> [&](){ FAIL(); };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_implicit_break, 3rd_case_true_other_inaccessable )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == new_york ) >> [&](){ FAIL(); }
        >> case_( _2 == new_york ) >> [&](){ FAIL(); }
        >> case_( _3 == new_york ) >> [&](){ executed = true; }
        >> default_ >> [&](){ FAIL(); };
    
    ASSERT_EQ(executed, true );
}






TEST(eswitch_v4_default, none_of_cases_valid_thus_DEFAULT )
{
    using namespace eswitch_v4;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ FAIL(); }
        >> case_( _2 != new_jersey ) >> [&](){ FAIL(); }
        >> case_( _3 != new_york ) >> [&](){ FAIL(); }
        >> default_ >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_fallthrought, 1st_match_fallthrought_to_2nd )
{
    using namespace eswitch_v4;

    bool executed_1_case = false;
    bool executed_2_case = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> [&](){ executed_1_case = true; } >> fallthrough_
        >> case_( _2 == new_jersey ) >> [&](){ executed_2_case = true; }
        >> case_( _3 == new_york )   >> [&](){ FAIL(); }
        >> default_                  >> [&](){ FAIL(); };
    
    ASSERT_EQ(executed_1_case, true );
    ASSERT_EQ(executed_2_case, true );
}

TEST(eswitch_v4_fallthrought, 2nd_match_fallthrought_to_3rd )
{
    using namespace eswitch_v4;

    bool executed_1_case = false;
    bool executed_2_case = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ FAIL(); }
        >> case_( _2 == new_jersey ) >> [&](){ executed_1_case = true; } >> fallthrough_
        >> case_( _3 == new_york )   >> [&](){ executed_2_case = true; }
        >> default_                  >> [&](){ FAIL(); };
    
    ASSERT_EQ(executed_1_case, true );
    ASSERT_EQ(executed_2_case, true );
}

TEST(eswitch_v4_fallthrought, 3rd_match_fallthrought_to_DEFAULT_ )
{
    using namespace eswitch_v4;

    bool executed_1_case = false;
    bool executed_2_case = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ FAIL(); }
        >> case_( _2 != new_jersey ) >> [&](){ FAIL(); }
        >> case_( _3 == new_york )   >> [&](){ executed_1_case = true; } >> fallthrough_
        >> default_                  >> [&](){ executed_2_case = true; };
    
    ASSERT_EQ(executed_1_case, true );
    ASSERT_EQ(executed_2_case, true );
}


TEST(eswitch_v4_return, 1st_match_and_to_return_one_statement )
{
    using namespace eswitch_v4;

    Place actual_result = unknown;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> to_return( washington )
        >> case_( _2 == new_jersey ) >> [&](){ return new_jersey; }
        >> case_( _3 == new_york )   >> [&](){ return new_york; }
        >> default_ >> [](){ FAIL(); }
        >> handle_return( [&]( const auto & res ){ actual_result = res; } );
    
    ASSERT_EQ(actual_result, washington );
}

TEST(eswitch_v4_return, 2nd_match_and_to_return_one_statement )
{
    using namespace eswitch_v4;

    Place actual_result = unknown;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> to_return( washington )
        >> case_( _2 == new_jersey ) >> to_return( new_jersey )
        >> case_( _3 == new_york )   >> [&](){ return new_york; }
        >> default_ >> [](){ FAIL(); }
        >> handle_return( [&]( const auto & res ){ actual_result = res; } );
    
    ASSERT_EQ(actual_result, new_jersey );
}

TEST(eswitch_v4_return, 3rd_match_and_to_return_one_statement )
{
    using namespace eswitch_v4;

    Place actual_result = unknown;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> to_return( washington )
        >> case_( _2 != new_jersey ) >> to_return( new_jersey )
        >> case_( _3 == new_york )   >> to_return( new_york )
        >> default_ >> [](){ FAIL(); }
        >> handle_return( [&]( const auto & res ){ actual_result = res; } );
    
    ASSERT_EQ(actual_result, new_york );
}

TEST(eswitch_v4_return, 1st_match_to_return_plus_in_place_return )
{
    using namespace eswitch_v4;

    Place actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> to_return( washington )
        >> case_( _2 == new_jersey ) >> to_return( new_jersey )
        >> case_( _3 == new_york )   >> to_return( new_york )
        >> default_ >> [](){ FAIL(); }
        >> in_place_return_;
    
    ASSERT_EQ(actual_result, washington );
}

TEST(eswitch_v4_return, 2nd_match_to_return_plus_in_place_return )
{
    using namespace eswitch_v4;

    Place actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> to_return( washington )
        >> case_( _2 == new_jersey ) >> to_return( new_jersey )
        >> case_( _3 == new_york )   >> to_return( new_york )
        >> default_ >> [](){ FAIL(); }
        >> in_place_return_;
    
    ASSERT_EQ(actual_result, new_jersey );
}

TEST(eswitch_v4_return, 3rd_match_to_return_plus_in_place_return )
{
    using namespace eswitch_v4;

    Place actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> to_return( washington )
        >> case_( _2 != new_jersey ) >> to_return( new_jersey )
        >> case_( _3 == new_york )   >> to_return( new_york )
        >> default_ >> [](){ FAIL(); }
        >> in_place_return_;
    
    ASSERT_EQ(actual_result, new_york );
}

TEST(eswitch_v4_return, 1st_match_to_return_plus_in_place_return_const_char )
{
    using namespace eswitch_v4;

    const char* actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> to_return( "washington" )
        >> case_( _2 == new_jersey ) >> to_return( "new_jersey" )
        >> case_( _3 == new_york )   >> to_return( "new_york" )
        >> default_ >> [](){ FAIL(); }
        >> in_place_return_;
    
    ASSERT_EQ(actual_result, std::string( "washington" ) );
}

TEST(eswitch_v4_return, 2nd_match_to_return_plus_in_place_return_const_char )
{
    using namespace eswitch_v4;

    const char* actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> to_return( "washington" )
        >> case_( _2 == new_jersey ) >> to_return( "new_jersey" )
        >> case_( _3 == new_york )   >> to_return( "new_york" )
        >> default_ >> [](){ FAIL(); }
        >> in_place_return_;
    
    ASSERT_EQ(actual_result, std::string( "new_jersey" ) );
}

TEST(eswitch_v4_return, 3rd_match_to_return_plus_in_place_return_const_char )
{
    using namespace eswitch_v4;

    const char* actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> to_return( "washington" )
        >> case_( _2 != new_jersey ) >> to_return( "new_jersey" )
        >> case_( _3 == new_york )   >> to_return( "new_york" )
        >> default_ >> [](){ FAIL(); }
        >> in_place_return_;
    
    ASSERT_EQ(actual_result, std::string( "new_york" ) );
}



TEST(eswitch_v4_return, 1st_match_to_return_plus_in_place_return_dynamic_int_array )
{
    using namespace eswitch_v4;

    std::unique_ptr< int > actual_result( eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> to_return( new int(10) )
        >> case_( _2 == new_jersey ) >> to_return( nullptr )
        >> case_( _3 == new_york )   >> to_return( nullptr )
        >> default_ >> [](){ FAIL(); }
        >> in_place_return_ );
    
    EXPECT_TRUE(actual_result != nullptr );
}

TEST(eswitch_v4_return, 2nd_match_to_return_plus_in_place_return_dynamic_int_array )
{
    using namespace eswitch_v4;

    std::unique_ptr< int > actual_result( eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> to_return( nullptr )
        >> case_( _2 == new_jersey ) >> to_return( new int(10) )
        >> case_( _3 == new_york )   >> to_return( nullptr )
        >> default_ >> [](){ FAIL(); }
        >> in_place_return_ );
    
    EXPECT_TRUE(actual_result != nullptr );
}

TEST(eswitch_v4_return, 3rd_match_to_return_plus_in_place_return_dynamic_int_array )
{
    using namespace eswitch_v4;

    std::unique_ptr< int > actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> to_return( nullptr )
        >> case_( _2 != new_jersey ) >> to_return( new int(10) )
        >> case_( _3 == new_york )   >> to_return( nullptr )
        >> default_ >> [](){ FAIL(); }
        >> in_place_return_ );
    
    EXPECT_TRUE(actual_result == nullptr );
}

TEST(eswitch_v4_in_place_return, 1st_match_lambda_return_conversion_from_const_char_ptr_to_std_string )
{
    using namespace eswitch_v4;

    std::string actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> []{ return "hello"; }
        >> case_( _2 == new_jersey ) >> []{ return std::string( "world" ); }
        >> in_place_return_ );
    
    ASSERT_EQ( actual_result, "hello" );
}

TEST(eswitch_v4_in_place_return, 2nd_match_lambda_return_conversion_from_const_char_ptr_to_std_string )
{
    using namespace eswitch_v4;

    std::string actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> []{ return "hello"; }
        >> case_( _2 == new_jersey ) >> []{ return std::string( "world" ); }
        >> in_place_return_ );
    
    EXPECT_TRUE(actual_result == "world" );
}

TEST(eswitch_v4_in_place_return, 1st_match_to_lambda_return_plus_in_place_return_dynamic_int_array )
{
    using namespace eswitch_v4;

    std::unique_ptr< int > actual_result( eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> []{ return new int(10); }
        >> case_( _2 == new_jersey ) >> []{ return nullptr; }
        >> case_( _3 == new_york )   >> []{ return nullptr; }
        >> in_place_return_ );
    
    EXPECT_TRUE(actual_result != nullptr );
}

TEST(eswitch_v4_in_place_return, 2nd_match_to_lambda_return_plus_in_place_return_dynamic_int_array )
{
    using namespace eswitch_v4;

    std::unique_ptr< int > actual_result( eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> []{ return nullptr; }
        >> case_( _2 == new_jersey ) >> []{ return new int(10); }
        >> case_( _3 == new_york )   >> []{ return nullptr; }
        >> in_place_return_ );
    
    EXPECT_TRUE(actual_result != nullptr );
}

TEST(eswitch_v4_in_place_return, 3rd_match_to_lambda_return_plus_in_place_return_dynamic_int_array )
{
    using namespace eswitch_v4;

    std::unique_ptr< int > actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> []{ return nullptr; }
        >> case_( _2 != new_jersey ) >> []{ return new int(10); }
        >> case_( _3 == new_york )   >> []{ return nullptr; }
        >> in_place_return_ );
    
    EXPECT_TRUE(actual_result == nullptr );
}

TEST(eswitch_v4_in_place_return, 1st_match_return_conversion_from_const_char_ptr_to_std_string )
{
    using namespace eswitch_v4;

    std::string actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> to_return( "hello" )
        >> case_( _2 == new_jersey ) >> to_return( std::string( "world" ) )
        >> in_place_return_ );
    
    EXPECT_TRUE(actual_result == "hello" );
}

TEST(eswitch_v4_in_place_return, 2nd_match_return_conversion_from_const_char_ptr_to_std_string )
{
    using namespace eswitch_v4;

    std::string actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> to_return( "hello" )
        >> case_( _2 == new_jersey ) >> to_return( std::string( "world" ) )
        >> in_place_return_ );
   
    EXPECT_TRUE(actual_result == "world" );
}


struct base
{ 
    virtual ~base() = default; 
    virtual int foo() { return 10; }
};

struct derived : public base
{
     virtual int foo() override { return 20; }
};

TEST(eswitch_v4_return, 1st_match_return_conversion_from_base_to_base )
{
    using namespace eswitch_v4;

    std::unique_ptr< base > actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> to_return( new base() )
        >> case_( _2 == new_jersey ) >> to_return( new derived() )
        >> in_place_return_ );
    
    ASSERT_EQ( actual_result->foo(), 10 );
}

TEST(eswitch_v4_return, 2nd_match_return_conversion_from_derived_to_base )
{
    using namespace eswitch_v4;

    std::unique_ptr< base > actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> to_return( new base() )
        >> case_( _2 == new_jersey ) >> to_return( new derived() )
        >> in_place_return_ );
    
    ASSERT_EQ( actual_result->foo(), 20 );
}

TEST(eswitch_v4_return, 3rd_match_return_conversion_from_nullptr_to_base )
{
    using namespace eswitch_v4;

    std::unique_ptr< base > actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> to_return( new base() )
        >> case_( _2 != new_jersey ) >> to_return( new derived() )
        >> case_( _3 == new_york )   >> to_return( nullptr )
        >> in_place_return_ );
    
    EXPECT_TRUE( actual_result == nullptr );
}

TEST(eswitch_v4_return, 1st_match_return_conversion_from_base_to_base_reverse )
{
    using namespace eswitch_v4;

    std::unique_ptr< base > actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _2 == new_jersey ) >> to_return( new derived() )
        >> case_( _1 == washington ) >> to_return( new base() )
        >> in_place_return_ );
    
    ASSERT_EQ( actual_result->foo(), 20 );
}

TEST(eswitch_v4_return, 2nd_match_return_conversion_from_derived_to_base_reverse )
{
    using namespace eswitch_v4;

    std::unique_ptr< base > actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _2 != new_jersey ) >> to_return( new derived() )
        >> case_( _1 == washington ) >> to_return( new base() )
        >> in_place_return_ );
    
    ASSERT_EQ( actual_result->foo(), 10 );
}

TEST(eswitch_v4_return, 3rd_match_return_conversion_from_nullptr_to_base_reverse )
{
    using namespace eswitch_v4;

    std::unique_ptr< base > actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _2 != new_jersey ) >> to_return( new derived() )
        >> case_( _1 != washington ) >> to_return( new base() )
        >> case_( _3 == new_york )   >> to_return( nullptr )
        >> in_place_return_ );
    
    EXPECT_TRUE( actual_result == nullptr );
}

TEST(eswitch_v4_return, 23rd_match_return_conversion_from_nullptr_to_base_reverse )
{
    using namespace eswitch_v4;

    std::unique_ptr< base > actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _3 != new_york )   >> to_return( nullptr )
        >> case_( _2 != new_jersey ) >> to_return( new derived() )
        >> case_( _1 == washington ) >> to_return( new base() )
        >> in_place_return_ );
    
    EXPECT_TRUE( actual_result != nullptr );
    EXPECT_TRUE( actual_result->foo() == 10 );
}
struct custom
{
    custom() = default;
    custom( int i ) : i( i ){}
    int i;
};

TEST(eswitch_v4_return, 1st_match_no_default_ctor_at_return_type )
{
    using namespace eswitch_v4;

    auto actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> to_return( custom( 1 ) )
        >> case_( _2 == new_jersey ) >> to_return( custom( 2 ) )
        >> case_( _3 == new_york )   >> to_return( custom( 3 ) )
        >> in_place_return_ );
    
    ASSERT_EQ( actual_result.i, 1 );
}

TEST(eswitch_v4_return, 2nd_match_no_default_ctor_at_return_type )
{
    using namespace eswitch_v4;

    auto actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> to_return( custom( 1 ) )
        >> case_( _2 == new_jersey ) >> to_return( custom( 2 ) )
        >> case_( _3 == new_york )   >> to_return( custom( 3 ) )
        >> in_place_return_ );
    
    ASSERT_EQ( actual_result.i, 2 );
}

TEST(eswitch_v4_return, 3rd_match_no_default_ctor_at_return_type )
{
    using namespace eswitch_v4;

    auto actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> to_return( custom( 1 ) )
        >> case_( _2 != new_jersey ) >> to_return( custom( 2 ) )
        >> case_( _3 == new_york )   >> to_return( custom( 3 ) )
        >> in_place_return_ );
    
    ASSERT_EQ( actual_result.i, 3 );
}


TEST(eswitch_v4_return, 1st_match_non_copyable_at_return_type_unique_ptr )
{
    using namespace eswitch_v4;

    auto actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> to_return( std::make_unique< int >( 10 ) )
        >> case_( _2 == new_jersey ) >> to_return( nullptr )
        >> case_( _3 == new_york )   >> to_return( nullptr )
        >> in_place_return_ );

   EXPECT_TRUE( actual_result != nullptr );    
   EXPECT_TRUE( *actual_result == 10 );
}

TEST(eswitch_v4_return, return_just_nullptr )
{
    using namespace eswitch_v4;

    auto actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _2 == new_jersey ) >> to_return( nullptr )
        >> in_place_return_ );

   EXPECT_TRUE( actual_result == nullptr );
}
TEST(eswitch_v4_return, 1st_match_non_copyable_at_return_type )
{
    using namespace eswitch_v4;

    auto actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _2 == new_jersey ) >> to_return( nullptr )
        >> case_( _1 == washington ) >> to_return( std::make_unique< int >( 10 ) )
        >> case_( _3 == new_york )   >> to_return( nullptr )
        >> in_place_return_ );

   EXPECT_TRUE( actual_result == nullptr );
}


TEST(eswitch_v4_in_place_return, 1st_match_lambda_return_conversion_from_base_to_base )
{
    using namespace eswitch_v4;

    std::unique_ptr< base > actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> []{ return new base(); }
        >> case_( _2 == new_jersey ) >> []{ return new derived(); }
        >> in_place_return_ );
    
    ASSERT_EQ( actual_result->foo(), 10 );
}

TEST(eswitch_v4_in_place_return, 2nd_match_lambda_return_conversion_from_derived_to_base )
{
    using namespace eswitch_v4;

    std::unique_ptr< base > actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> []{ return new base(); }
        >> case_( _2 == new_jersey ) >> []{ return new derived(); }
        >> in_place_return_ );
    
    ASSERT_EQ( actual_result->foo(), 20 );
}

TEST(eswitch_v4_in_place_return, 3rd_match_lambda_return_conversion_from_nullptr_to_base )
{
    using namespace eswitch_v4;

    std::unique_ptr< base > actual_result( 
        eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> []{ return new base(); }
        >> case_( _2 != new_jersey ) >> []{ return new derived(); }
        >> case_( _3 == new_york )   >> []{ return nullptr; }
        >> in_place_return_ );
    
    EXPECT_TRUE( actual_result == nullptr );
}


TEST(eswitch_v4_return, 1st_match_and_return )
{
    using namespace eswitch_v4;

    Place actual_result = unknown;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> [&](){ return washington; }
        >> case_( _2 == new_jersey ) >> [&](){ return new_jersey; }
        >> case_( _3 == new_york )   >> [&](){ return new_york; }
        >> default_ >> [](){ FAIL(); }
        >> handle_return( [&]( const auto & res ){ actual_result = res; } );
    
    ASSERT_EQ(actual_result, washington );
}

TEST(eswitch_v4_return, 2nd_match_and_return )
{
    using namespace eswitch_v4;

    Place actual_result = unknown;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ return washington; }
        >> case_( _2 == new_jersey ) >> [&](){ return new_jersey; }
        >> case_( _3 == new_york )   >> [&](){ return new_york; }
        >> default_ >> [](){ FAIL(); }
        >> handle_return( [&]( const auto & res ){ actual_result = res; } );
    
    ASSERT_EQ(actual_result, new_jersey );
}

TEST(eswitch_v4_return, 3rd_match_and_return )
{
    using namespace eswitch_v4;

    Place actual_result = unknown;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ return washington; }
        >> case_( _2 != new_jersey ) >> [&](){ return new_jersey; }
        >> case_( _3 == new_york )   >> [&](){ return new_york; }
        >> default_ >> [](){ FAIL(); }
        >> handle_return( [&]( const auto & res ){ actual_result = res; } );
    
    ASSERT_EQ(actual_result, new_york );
}

TEST(eswitch_v4_return, default_match_and_return )
{
    using namespace eswitch_v4;

    Place actual_result = new_jersey;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ return washington; }
        >> case_( _2 != new_jersey ) >> [&](){ return new_jersey; }
        >> case_( _3 != new_york )   >> [&](){ return new_york; }
        >> default_ >> [](){ return unknown; }
        >> handle_return( [&]( const auto & res ){ actual_result = res; } );
    
    ASSERT_EQ(actual_result, unknown );
}

TEST(eswitch_v4_return, with_cases_just_default_match_and_return )
{
    using namespace eswitch_v4;

    Place actual_result = new_jersey;

    eswitch( washington, new_jersey, new_york )
        >> default_ >> [](){ return unknown; }
        >> handle_return( [&]( const auto & res ){ actual_result = res; } );
    
    ASSERT_EQ(actual_result, unknown );
}

TEST(eswitch_v4_return, 1st_match_regardless_fallthrough )
{
    using namespace eswitch_v4;

    Place actual_result = new_jersey;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> [&](){ return washington; } >> fallthrough_
        >> case_( _2 == new_jersey ) >> [&](){ return new_jersey; } >> fallthrough_
        >> case_( _3 == new_york )   >> [&](){ return new_york; }   >> fallthrough_
        >> default_ >> [](){ return unknown; }
        >> handle_return( [&]( const auto & res ){ actual_result = res; } );
    
    ASSERT_EQ(actual_result, washington );
}

TEST(eswitch_v4_return, 2nd_match_regardless_fallthrough )
{
    using namespace eswitch_v4;

    Place actual_result = new_jersey;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ return washington; } >> fallthrough_
        >> case_( _2 == new_jersey ) >> [&](){ return new_jersey; } >> fallthrough_
        >> case_( _3 == new_york )   >> [&](){ return new_york; }   >> fallthrough_
        >> default_ >> [](){ return unknown; }
        >> handle_return( [&]( const auto & res ){ actual_result = res; } );
    
    ASSERT_EQ(actual_result, new_jersey );
}

TEST(eswitch_v4_return, 3rd_match_regardless_fallthrough )
{
    using namespace eswitch_v4;

    Place actual_result = new_jersey;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ return washington; } >> fallthrough_
        >> case_( _2 != new_jersey ) >> [&](){ return new_jersey; } >> fallthrough_
        >> case_( _3 == new_york )   >> [&](){ return new_york; }   >> fallthrough_
        >> default_ >> [](){ return unknown; }
        >> handle_return( [&]( const auto & res ){ actual_result = res; } );
    
    ASSERT_EQ(actual_result, new_york );
}

TEST(eswitch_v4_return, 1st_match_cases_with_and_without_return )
{
    using namespace eswitch_v4;

    Place actual_result = unknown;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> [&](){ executed = true; }
        >> case_( _2 == new_jersey ) >> [&](){ return new_jersey; }
        >> case_( _3 == new_york )   >> [&](){ return new_york; }
        >> default_ >> [](){ FAIL(); }
        >> handle_return( [&]( const auto & res ){ actual_result = res; } );

    ASSERT_EQ(executed, true );
    ASSERT_EQ(actual_result, unknown );
}

TEST(eswitch_v4_return, 2nd_match_cases_with_and_without_return )
{
    using namespace eswitch_v4;

    Place actual_result = unknown;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ return washington; }
        >> case_( _2 == new_jersey ) >> [&](){ executed = true; }
        >> case_( _3 == new_york )   >> [&](){ return new_york; }
        >> default_ >> [](){ FAIL(); }
        >> handle_return( [&]( const auto & res ){ actual_result = res; } );

    ASSERT_EQ(executed, true );
    ASSERT_EQ(actual_result, unknown );
}

TEST(eswitch_v4_return, 3rd_match_cases_with_and_without_return )
{
    using namespace eswitch_v4;

    Place actual_result = unknown;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ return washington; }
        >> case_( _2 != new_jersey ) >> [&](){ return new_jersey; }
        >> case_( _3 == new_york )   >> [&](){ executed = true; }
        >> default_ >> [](){ FAIL(); }
        >> handle_return( [&]( const auto & res ){ actual_result = res; } );

    ASSERT_EQ(executed, true );
    ASSERT_EQ(actual_result, unknown );
}

TEST(eswitch_v4_return, default_match_cases_with_and_without_return )
{
    using namespace eswitch_v4;

    Place actual_result = unknown;

    bool executed = false;

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ return washington; }
        >> case_( _2 != new_jersey ) >> [&](){ return new_jersey; }
        >> case_( _3 != new_york )   >> [&](){ return new_york; }
        >> default_ >> [&](){ executed = true; }
        >> handle_return( [&]( const auto & res ){ actual_result = res; } );

    ASSERT_EQ(executed, true );
    ASSERT_EQ(actual_result, unknown );
}




TEST(eswitch_v4_return_in_place, 1st_match_and_return )
{
    using namespace eswitch_v4;

    Place actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> [&](){ return washington; }
        >> case_( _2 == new_jersey ) >> [&](){ return new_jersey; }
        >> case_( _3 == new_york )   >> [&](){ return new_york; }
        >> default_ >> [](){ FAIL(); }
        >> in_place_return_;
    
    ASSERT_EQ(actual_result, washington );
}

TEST(eswitch_v4_return_in_place, 2nd_match_and_return )
{
    using namespace eswitch_v4;

    Place actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ return washington; }
        >> case_( _2 == new_jersey ) >> [&](){ return new_jersey; }
        >> case_( _3 == new_york )   >> [&](){ return new_york; }
        >> default_ >> [](){ FAIL(); }
        >> in_place_return_;
    
    ASSERT_EQ(actual_result, new_jersey );
}

TEST(eswitch_v4_return_in_place, 3rd_match_and_return )
{
    using namespace eswitch_v4;

    Place actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ return washington; }
        >> case_( _2 != new_jersey ) >> [&](){ return new_jersey; }
        >> case_( _3 == new_york )   >> [&](){ return new_york; }
        >> default_ >> [](){ FAIL(); }
        >> in_place_return_;
    
    ASSERT_EQ(actual_result, new_york );
}

TEST(eswitch_v4_return_in_place, default_match_and_return )
{
    using namespace eswitch_v4;

    Place actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ return washington; }
        >> case_( _2 != new_jersey ) >> [&](){ return new_jersey; }
        >> case_( _3 != new_york )   >> [&](){ return new_york; }
        >> default_ >> [](){ return unknown; }
        >> in_place_return_;
    
    ASSERT_EQ(actual_result, unknown );
}

TEST(eswitch_v4_return_in_place, no_match_and_exception )
{
    using namespace eswitch_v4;

    bool executed = false;
    try
    {
    Place actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ return washington; }
        >> case_( _2 != new_jersey ) >> [&](){ return new_jersey; }
        >> case_( _3 != new_york )   >> [&](){ return new_york; }
        >> in_place_return_;

        FAIL();

    } catch( const std::logic_error & le )
    {
        executed = true;
    }

    ASSERT_EQ( executed, true );
    
}

TEST(eswitch_v4_return_in_place, 1st_match_regardless_fallthrough )
{
    using namespace eswitch_v4;

    Place actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> [&](){ return washington; } >> fallthrough_
        >> case_( _2 == new_jersey ) >> [&](){ return new_jersey; } >> fallthrough_
        >> case_( _3 == new_york )   >> [&](){ return new_york; }   >> fallthrough_
        >> default_ >> [](){ return unknown; }
        >> in_place_return_;
    
    ASSERT_EQ(actual_result, washington );
}

TEST(eswitch_v4_return_in_place, 2nd_match_regardless_fallthrough )
{
    using namespace eswitch_v4;

    Place actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ return washington; } >> fallthrough_
        >> case_( _2 == new_jersey ) >> [&](){ return new_jersey; } >> fallthrough_
        >> case_( _3 == new_york )   >> [&](){ return new_york; }   >> fallthrough_
        >> default_ >> [](){ return unknown; }
        >> in_place_return_;
    
    ASSERT_EQ(actual_result, new_jersey );
}

TEST(eswitch_v4_return_in_place, 3rd_match_regardless_fallthrough )
{
    using namespace eswitch_v4;

    Place actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ return washington; } >> fallthrough_
        >> case_( _2 != new_jersey ) >> [&](){ return new_jersey; } >> fallthrough_
        >> case_( _3 == new_york )   >> [&](){ return new_york; }   >> fallthrough_
        >> default_ >> [](){ return unknown; }
        >> in_place_return_;
    
    ASSERT_EQ(actual_result, new_york );
}

TEST(eswitch_v4_return_in_place, 1st_match_cases_with_and_without_return )
{
    using namespace eswitch_v4;

    bool executed_catch = false;
    bool executed_case = false;

    try { 
    auto actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> [&](){ executed_case = true; }
        >> case_( _2 == new_jersey ) >> [&](){ return new_jersey; }
        >> case_( _3 == new_york )   >> [&](){  }
        >> default_ >> [](){ FAIL(); }
        >> in_place_return_;
        
        FAIL();
    } catch( const std::logic_error & le )
    {
        executed_catch = true;
    }

    ASSERT_EQ(executed_case, true );
    ASSERT_EQ(executed_catch, true );
}

TEST(eswitch_v4_return_in_place, 2nd_match_cases_with_and_without_return )
{
    using namespace eswitch_v4;

    bool executed_catch = false;
    bool executed_case = false;

    try { 
    auto actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ return washington; }
        >> case_( _2 == new_jersey ) >> [&](){ executed_case = true; }
        >> case_( _3 == new_york )   >> [&](){  }
        >> default_ >> [](){ FAIL(); }
        >> in_place_return_;
        
        FAIL();
    } catch( const std::logic_error & le )
    {
        executed_catch = true;
    }

    ASSERT_EQ(executed_case, true );
    ASSERT_EQ(executed_catch, true );
}

TEST(eswitch_v4_return_in_place, 3rd_match_cases_with_and_without_return )
{
    using namespace eswitch_v4;

    bool executed_catch = false;
    bool executed_case = false;

    try { 
    auto actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ return washington; }
        >> case_( _2 != new_jersey ) >> [&](){  }
        >> case_( _3 == new_york )   >> [&](){  executed_case = true; }
        >> default_ >> [](){ FAIL(); }
        >> in_place_return_;

        FAIL();
    } catch( const std::logic_error & le )
    {
        executed_catch = true;
    }

    ASSERT_EQ(executed_case, true );
    ASSERT_EQ(executed_catch, true );
}

TEST(eswitch_v4_return_in_place, default_match_cases_with_and_without_return )
{
    using namespace eswitch_v4;

    bool executed_catch = false;
    bool executed_case = false;

    try{
    auto actual_result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 != washington ) >> [&](){ return washington; }
        >> case_( _2 != new_jersey ) >> [&](){  }
        >> case_( _3 != new_york )   >> [&](){  }
        >> default_ >> [&](){ executed_case = true; }
        >> in_place_return_;

        FAIL();
    } catch( const std::logic_error & le )
    {
        executed_catch = true;
    }

    ASSERT_EQ(executed_case, true );
    ASSERT_EQ(executed_catch, true );
}










TEST(eswitch_v4_predicate, rvalue_lambda )
{
    using namespace eswitch_v4;

    bool executed = false;
    
    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> [&](){ executed = true; };
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_predicate, rvalue_lambda_and_return )
{
    using namespace eswitch_v4;

    bool executed = false;
    
    auto result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> [&](){ executed = true; return "washington"; } >> in_place_return_;
    
    ASSERT_EQ( executed, true );
    ASSERT_EQ( result, std::string{ "washington" } );
}

TEST(eswitch_v4_predicate, lvalue_lambda )
{
    using namespace eswitch_v4;

    bool executed = false;
    
    auto handler = [&](){ executed = true; };

    eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> handler;
    
    ASSERT_EQ(executed, true );
}

TEST(eswitch_v4_predicate, lvalue_lambda_and_return )
{
    using namespace eswitch_v4;

    bool executed = false;
    
    auto handler = [&](){ executed = true; return "washington"; };

    auto result = eswitch( washington, new_jersey, new_york )
        >> case_( _1 == washington ) >> handler >> in_place_return_;
    
    ASSERT_EQ( executed, true );
    ASSERT_EQ( result, std::string{ "washington" } );
}

TEST(eswitch_v4_any, not_equal )
{
    using namespace eswitch_v4;
    
    EXPECT_TRUE( ( 10 != extension::any() ) == false );
}

TEST(eswitch_v4_any, equal )
{
    using namespace eswitch_v4;
    
    EXPECT_TRUE( ( 10 == extension::any() ) == true );
    EXPECT_TRUE( ( "Hello" == extension::any() ) == true );
    EXPECT_TRUE( ( 'c' == extension::any() ) == true );
    EXPECT_TRUE( ( std::string( "" ) == extension::any() ) == true );
    EXPECT_TRUE( ( std::vector<int>{ 1, 2 } == extension::any() ) == true );
}

TEST(eswitch_v4_return, return_after_fallthrough )
{
    using namespace eswitch_v4;

    auto result = eswitch( washington, new_jersey, new_york ) >> 
        case_( _1 == washington ) >> []{} >> fallthrough_ >> 
        case_( _1 == new_jersey ) >> to_return( new_jersey ) >> 
        in_place_return_;
    
    EXPECT_TRUE( result == new_jersey );
}


TEST(eswitch_v4_return, ternary_operator_eval_first )
{
    using namespace eswitch_v4;

    bool flag = true;

    auto result = eswitch( washington, new_jersey, new_york ) >> 
        case_( _1 == washington ) >> [&]{ return flag ? new_york : unknown; } >> 
        case_( _1 == new_jersey ) >> to_return( new_jersey ) >> 
        in_place_return_;
    
    EXPECT_TRUE( result == new_york );
}


TEST(eswitch_v4_in_place_return, ternary_operator_eval_first )
{
    using namespace eswitch_v4;

    bool flag = true;

    auto result = eswitch( washington, new_jersey, new_york ) >> 
        case_( _1 == washington ) >> to_return( flag ? new_york : unknown ) >> 
        case_( _1 == new_jersey ) >> to_return( new_jersey ) >> 
        in_place_return_;
    
    EXPECT_TRUE( result == new_york );
}


TEST(eswitch_v4_return, ternary_operator_eval_second )
{
    using namespace eswitch_v4;

    bool flag = false;

    auto result = eswitch( washington, new_jersey, new_york ) >> 
        case_( _1 == washington ) >> [&]{ return flag ? new_jersey : washington; } >> 
        case_( _1 == new_jersey ) >> to_return( new_jersey ) >> 
        in_place_return_;
    
    EXPECT_TRUE( result == washington );
}


TEST(eswitch_v4_in_place_return, ternary_operator_eval_second )
{
    using namespace eswitch_v4;

    bool flag = false;

    auto result = eswitch( washington, new_jersey, new_york ) >> 
        case_( _1 == washington ) >> to_return( flag ? new_jersey : washington ) >> 
        case_( _1 == new_jersey ) >> to_return( new_jersey ) >> 
        in_place_return_;
    
    EXPECT_TRUE( result == washington );
}

bool _isNegative = true;
bool _isZero = true;

struct cf
{
    cf( bool first, bool second )
    {
        _isNegative = first;
        _isZero = second;
    }

    bool isZero() { return _isZero; }
    bool isNegative() { return _isNegative; }

};

TEST(eswitch_v4_in_place_return, eval_expression )
{
    using namespace eswitch_v4;

    {
    cf CF( true, true );

    auto result = eswitch( washington, new_jersey, new_york ) >> 
        case_( _1 == washington ) >> to_return( CF.isNegative() && CF.isZero() ) >> 
        in_place_return_;
    
    EXPECT_TRUE( result == true );
    }

    {
    cf CF( false, true );

    auto result = eswitch( washington, new_jersey, new_york ) >> 
        case_( _1 == washington ) >> to_return( CF.isNegative() && CF.isZero() ) >> 
        in_place_return_;
    
    EXPECT_TRUE( result == false );
    }

    {
    cf CF( false, false );

    auto result = eswitch( washington, new_jersey, new_york ) >> 
        case_( _1 == washington ) >> to_return( !CF.isNegative() && !CF.isZero() ) >> 
        in_place_return_;
    
    EXPECT_TRUE( result == true );
    }

    {
    cf CF( true, false );

    auto result = eswitch( washington, new_jersey, new_york ) >> 
        case_( _1 == washington ) >> to_return( CF.isNegative() && !CF.isZero() ) >> 
        in_place_return_;
    
    EXPECT_TRUE( result == true );
    }

    {
    cf CF( true, false );

    auto result = eswitch( washington, new_jersey, new_york ) >> 
        case_( _1 != washington ) >> to_return( CF.isNegative() && !CF.isZero() ) >> 
        case_( _2 == new_jersey ) >> to_return( false ) >>         
        in_place_return_;
    
    EXPECT_TRUE( result == false );
    }
}

TEST(eswitch_v4_return, stringstream_eval_plus_operator_comma )
{
    using namespace eswitch_v4;

    bool flag = false;

    std::stringstream ss;
    
    auto result = eswitch( washington, new_jersey, new_york ) >> 
        case_( _1 == washington ) >> to_return( ( ss << "Hello " << std::to_string( 1 ), ss.str() ) ) >> 
        in_place_return_;
    
    EXPECT_TRUE( result == "Hello 1" );
}

TEST(eswitch_v4_return, bitwise_operation_right_shift )
{
    using namespace eswitch_v4;

    int ch = 0b0000'1111;
    {
    auto result = eswitch( washington, new_jersey, new_york ) >> 
        case_( _1 == washington ) >> to_return( ch >> 1 ) >> 
        in_place_return_;
    
    EXPECT_TRUE( result == 0b0000'0111 );
    }

    {
    auto result = eswitch( washington, new_jersey, new_york ) >> 
        case_( _1 != washington ) >> to_return( ch >> 1 ) >> 
        case_( _2 == new_jersey ) >> to_return( ch >> 3 ) >> 
        in_place_return_;
    
    EXPECT_TRUE( result == 0b0000'0001 );
    }
}

TEST(eswitch_v4_return, bitwise_operation_combination )
{
    using namespace eswitch_v4;

    const int ch = 0b11001000;

    auto result = eswitch( washington, new_jersey, new_york ) >> 
        case_( _1 == washington ) >> to_return( ch ^ 0b10111000 ) >> 
        in_place_return_;
    
    EXPECT_TRUE( result == 0b01110000 );
}

TEST(eswitch_v4_return, long_expression_evaluation )
{
    using namespace eswitch_v4;
    
    std::vector< int > v;

    auto result = eswitch( washington, new_jersey, new_york ) >> 
        case_( _1 == washington ) >> to_return( ( v = std::vector< int >{ 1, 2, 3 }, v.push_back( 4 ), v.push_back( 5 ), v.push_back( 4 ), v ) ) >> 
            in_place_return_;

    std::vector< int > expected_result{ 1, 2, 3, 4, 5, 4 };

    EXPECT_TRUE( result == expected_result );
}