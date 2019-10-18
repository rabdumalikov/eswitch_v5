if(USE_GCOV)
    message("USE_GCOV...")
    SET(GCOVR /usr/bin/gcovr)

    SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
    SET(GCC_COVERAGE_LINK_FLAGS "-lgcov")
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )               

    add_custom_command(OUTPUT _run_gcovr_parser
    POST_BUILD
    COMMAND ${GCOVR} -r ${CMAKE_SOURCE_DIR} --branches --exclude-unreachable-branches --html --html-details -o coverage.html
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

    add_custom_target (coverage DEPENDS _run_gcovr_parser)
endif(USE_GCOV)