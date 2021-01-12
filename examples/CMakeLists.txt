# CMake build examples

message( STATUS "Runtimes are stored in ${CMAKE_CURRENT_BINARY_DIR}" )

set(run_examples_list
  stringify_enum
  default
  implicit_break
  explicit_fallthrough
  match_by_multiple_parameters
  any_from
  matching_arguments_in_any_order
  partial_match
  match_by_predicates
  match_by_predicate_as_free_function
  match_mixed_condition
  match_via_regex
  match_via_regex_with_match_output
  match_in_range
  match_for_std_any
  match_for_std_variant
  match_for_polymorphic_types
  match_for_std_pair
  match_for_std_tuple
  extension_floating_point_comparison
  extension_value_and_type_transferring
)

foreach(example_name ${run_examples_list})
  build_source_file(${example_name})
  set_target_properties(${example_name} PROPERTIES FOLDER "run examples")
endforeach(example_name)
