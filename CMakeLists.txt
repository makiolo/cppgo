project(cppgo CXX)
cmake_minimum_required(VERSION 3.2.2)

set(CMAKI_REPOSITORY "http://artifacts.myftp.biz:8080")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmaki/cmaki.cmake")
cmaki_setup()

cmaki_find_package(teelogging)
cmaki_find_package(design-patterns-cpp14)
cmaki_install_python()

# c++ implementations
cmaki_library(foo tests/foo.cpp)
cmaki_library(fooA tests/fooA.cpp DEPENDS foo)
cmaki_library(fooB tests/fooB.cpp DEPENDS foo)
# python implementations
cmaki_install_file(tests/fooA_python.py)
cmaki_install_file(tests/fooB_python.py)

# need for do "import foo_python" since python
cmaki_python_library(foo_python tests/foo_python.cpp DEPENDS foo)

# c++ program using factory
cmaki_python_gtest(python_embed tests/python_embed.cpp DEPENDS foo)

# python program using factory
cmaki_python_test(cpp_embed cpp_embed.py)
