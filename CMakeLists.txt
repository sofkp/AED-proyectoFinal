cmake_minimum_required(VERSION 3.26)
project(AED_proyectoFinal)

set(CMAKE_CXX_STANDARD 14)

# add_executable(AED_proyectoFinal main.cpp)
# add_executable(a prueba.cpp)
# add_executable(b prueba2.cpp)
add_subdirectory(external/cpp-httplib)
include_directories(.)
add_executable(c RBTree.cpp
        RBTree.h
        main.cpp)

target_link_libraries(c PRIVATE httplib::httplib)