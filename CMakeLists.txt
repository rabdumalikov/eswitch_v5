cmake_minimum_required(VERSION 3.5.1)

project(ESWITCH)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options( -std=c++20 -stdlib=libc++ )
        message("ClangCompiler")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options( -std=c++20 )
        message("GccCompiler")
    endif()

    message("ESWITCH Debug.build")
ELSE()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options( -O3 -std=c++20 -stdlib=libc++ )
        message("ClangCompiler")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options( -O3 -std=c++20 )
        #set(CMAKE_CXX_FLAGS_RELEASE "-std=c++20 -O3 -Wno-stringop-truncation")
        message("GccCompiler__")
    endif()

    message("ESWITCH Release.build")
ENDIF()

if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

include(DownloadProject.cmake)
download_project(PROJ                catchtest
                 GIT_REPOSITORY      https://github.com/catchorg/Catch2.git
                 GIT_TAG             v2.x
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

add_subdirectory(${catchtest_SOURCE_DIR} ${catchtest_BINARY_DIR})

download_project(PROJ                googlebench
                 GIT_REPOSITORY      https://github.com/google/benchmark.git
                 GIT_TAG             master
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

# Disable building tests for GoogleBenchmark
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

add_subdirectory(${googlebench_SOURCE_DIR} ${googlebench_BINARY_DIR})

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
endif()

add_executable(eswitch_tests tests/eswitch_tests.cpp)
target_include_directories(eswitch_tests PUBLIC $<BUILD_INTERFACE:${ESWITCH_SOURCE_DIR}/include/eswitch>)
#target_link_libraries(eswitch_tests catch2 catch2_main)
target_include_directories(eswitch_tests PUBLIC $<BUILD_INTERFACE:${catchtest_SOURCE_DIR}/single_include/>)
add_test(NAME eswitch_tests COMMAND eswitch_tests)


if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options( -Winline )
    message("GccCompiler")
endif()

add_executable(eswitch_benchmark benchmarks/eswitch_benchmark.cpp)
target_include_directories(eswitch_benchmark PUBLIC $<BUILD_INTERFACE:${ESWITCH_SOURCE_DIR}/include/eswitch>)
target_include_directories(eswitch_benchmark PUBLIC $<BUILD_INTERFACE:${googlebench_SOURCE_DIR}/include/>)
target_link_libraries(eswitch_benchmark benchmark)

function(build_source_file base_name )
  message(STATUS ${base_name})
  add_executable(${base_name} ${base_name}.cpp)
  target_include_directories(${base_name} PUBLIC $<BUILD_INTERFACE:${ESWITCH_SOURCE_DIR}/include/eswitch>)
endfunction(build_source_file)


 list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/)
 #include (gcovr)

 #add_subdirectory("examples")