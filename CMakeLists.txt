project(cppgo CXX)
cmake_minimum_required(VERSION 3.0)

include("$ENV{CMAKI_PWD}/node_modules/cmaki/cmaki.cmake")

cmaki_find_package(teelogging)
cmaki_find_package(design-patterns-cpp14)
cmaki_python_install()

# c++ implementations
cmaki_simple_library(foo tests/foo.cpp)
cmaki_simple_library(fooA tests/fooA.cpp DEPENDS foo)
cmaki_simple_library(fooB tests/fooB.cpp DEPENDS foo)
# python implementations
cmaki_install_file(tests/fooA_python.py)
cmaki_install_file(tests/fooB_python.py)

# need for do "import foo_python" since python
cmaki_python_library(foo_python tests/foo_python.cpp DEPENDS foo)

# c++ program using factory
cmaki_boost_python_test(python_embed tests/python_embed.cpp DEPENDS foo)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# python program using factory
	cmaki_python_test(cpp_embed cpp_embed.py)
	cmaki_install_file(tests/cpp_embed.py)
endif()
