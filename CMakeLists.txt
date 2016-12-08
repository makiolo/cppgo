project(cppgo CXX)
cmake_minimum_required(VERSION 3.2.2)

set(CMAKI_REPOSITORY "http://artifacts.myftp.biz:8080")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmaki/cmaki.cmake")
cmaki_setup()

cmaki_find_package(python)
cmaki_find_package(boost-python)

cmaki2_library(hello tests/test_01.cpp)
cmaki_install_file(tests/hello.py)

enable_testing()
add_test(	NAME hello_test
			COMMAND ${PYTHON3_EXECUTABLE} hello.py
			WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE})
set_tests_properties(hello_test PROPERTIES ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}")

# install python
# TODO: preserve permissions
get_filename_component(PYTHON3_DIR ${PYTHON3_EXECUTABLE} DIRECTORY)
cmaki_install_inside_dir(${PYTHON3_DIR}/..)

# for testing
cmaki_find_package(entityx)
cmaki_find_package(sdl2)
cmaki_find_package(ois)

