project(project_name CXX)
cmake_minimum_required(VERSION 3.2.2)

set(CMAKI_REPOSITORY "http://artifacts.myftp.biz:8080")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmaki/cmaki.cmake")
cmaki_setup()

cmaki_find_package(python)
cmaki_find_package(boost-python)

cmaki2_library(hello tests/test_01.cpp)
set_target_properties(hello PROPERTIES PREFIX "_")
cmaki_install_file(tests/hello.py)

# for testing
cmaki_find_package(entityx)
cmaki_find_package(sdl2)
cmaki_find_package(ois)

