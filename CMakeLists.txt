cmake_minimum_required(VERSION 2.8.2)

project(ESWITCH)

include(CTest)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message("ESWITCH Debug.build")
ELSE()
    add_compile_options( -O3 )
    message("ESWITCH Release.build")
ENDIF()


#set( header_path "${ESWITCH_SOURCE_DIR}/include/eswitch" )
#set( header ${header_path}/eswitch.hpp )


if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

include(DownloadProject.cmake)
download_project(PROJ                googletest
                 GIT_REPOSITORY      https://github.com/google/googletest.git
                 GIT_TAG             master
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)



# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

download_project(PROJ                googlebench
                 GIT_REPOSITORY      https://github.com/google/benchmark.git
                 GIT_TAG             master
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

add_subdirectory(${googlebench_SOURCE_DIR} ${googlebench_BINARY_DIR})


# When using CMake 2.8.11 or later, header path dependencies
# are automatically added to the gtest and gmock targets.
# For earlier CMake versions, we have to explicitly add the
# required directories to the header search path ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include"
			"${gbench_SOURCE_DIR}/include")
endif()

# Trivial example using gtest and gmock


add_executable(eswitch_tests tests/eswitch_v4_tests.cpp)

target_include_directories(eswitch_tests PUBLIC $<BUILD_INTERFACE:${ESWITCH_SOURCE_DIR}/include/eswitch>)

target_link_libraries(eswitch_tests gtest gmock_main)

add_test(NAME eswitch_tests COMMAND eswitch_tests)


if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gbench_SOURCE_DIR}/include")
endif()


add_executable(eswitch_benchmark benchmarks/eswitch_v4_benchmark.cpp)
target_include_directories(eswitch_benchmark PUBLIC $<BUILD_INTERFACE:${ESWITCH_SOURCE_DIR}/include/eswitch>)
target_include_directories(eswitch_benchmark PUBLIC $<BUILD_INTERFACE:${googlebench_SOURCE_DIR}/include/>)

target_link_libraries(eswitch_benchmark benchmark)

add_executable(eswitch_example example/example1.cpp)
target_include_directories(eswitch_example PUBLIC $<BUILD_INTERFACE:${ESWITCH_SOURCE_DIR}/include/eswitch>)


