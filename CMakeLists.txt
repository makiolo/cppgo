project(cppgo CXX)
cmake_minimum_required(VERSION 3.2.2)

set(CMAKI_REPOSITORY "http://artifacts.myftp.biz:8080")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmaki/cmaki.cmake")
cmaki_setup()

cmaki_find_package(python)
cmaki_find_package(boost-python)

cmaki2_library(hello tests/test_01.cpp)
set_target_properties(hello PROPERTIES PREFIX "")
cmaki2_library(classes tests/test_02.cpp)
set_target_properties(classes PROPERTIES PREFIX "")
cmaki2_library(ctor tests/test_03.cpp)
set_target_properties(ctor PROPERTIES PREFIX "")
cmaki2_library(member tests/test_04.cpp)
set_target_properties(member PROPERTIES PREFIX "")
cmaki2_library(inheritance tests/test_05.cpp)
set_target_properties(inheritance PROPERTIES PREFIX "")

cmaki_install_file(tests/hello.py)

enable_testing()
add_test(		NAME hello_test
			COMMAND ./bin/python3 hello.py
			WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE})
set_tests_properties(hello_test PROPERTIES ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}")

# install python
get_filename_component(PYTHON3_DIR ${PYTHON3_EXECUTABLE} DIRECTORY)
get_filename_component(PYTHON3_PARENT_DIR ${PYTHON3_DIR} DIRECTORY)
cmaki_install_inside_dir(${PYTHON3_PARENT_DIR})

# for testing
# cmaki_find_package(entityx)
# cmaki_find_package(sdl2)
# cmaki_find_package(ois)
