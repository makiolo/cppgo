project(cppgo CXX)
cmake_minimum_required(VERSION 3.2.2)

set(CMAKI_REPOSITORY "http://artifacts.myftp.biz:8080")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmaki/cmaki.cmake")
cmaki_setup()

cmaki_find_package(python)
cmaki_find_package(boost-python)
cmaki_find_package(design-patterns-cpp14)
cmaki_find_package(teelogging)

cmaki_library(foo tests/foo.cpp)
cmaki_library(fooA tests/fooA.cpp DEPENDS foo)
cmaki_library(fooB tests/fooB.cpp DEPENDS foo)
cmaki_python_library(foo_python tests/foo_python.cpp DEPENDS foo)

# cmaki_python_library(hello tests/test_01.cpp)
# cmaki_python_library(classes tests/test_02.cpp)
# cmaki_python_library(ctor tests/test_03.cpp)
# cmaki_python_library(member tests/test_04.cpp)
# cmaki_python_library(inheritance tests/test_05.cpp)

# install python
get_filename_component(PYTHON3_DIR ${PYTHON3_EXECUTABLE} DIRECTORY)
get_filename_component(PYTHON3_PARENT_DIR ${PYTHON3_DIR} DIRECTORY)
cmaki_install_inside_dir(${PYTHON3_PARENT_DIR})

# call python since c++
cmaki_install_file(tests/cpp_embed.py)
cmaki2_gtest(python_embed tests/python_embed.cpp PTHREADS DEPENDS foo)
set_tests_properties(python_embed_exe PROPERTIES ENVIRONMENT "PYTHONPATH=${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}")

# call c++ since python
cmaki_install_file(tests/fooA_python.py)
cmaki_install_file(tests/fooB_python.py)
enable_testing()
add_test(	NAME cpp_embed_test
			COMMAND ./bin/python3 cpp_embed.py
			WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE})
set_tests_properties(cpp_embed_test PROPERTIES ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}")

