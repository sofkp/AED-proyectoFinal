cmake_minimum_required(VERSION 3.26)
project(AED_proyectoFinal)

set(CMAKE_CXX_STANDARD 14)

# Crear el ejecutable
add_executable(c prueba3.cpp prueba3.h main.cpp)

find_package(Threads REQUIRED)
target_link_libraries(c PRIVATE Threads::Threads)

# Agregar las librerías externas
target_include_directories(c PRIVATE
        ${CMAKE_SOURCE_DIR}/external/cpp-httplib
        ${CMAKE_SOURCE_DIR}/external/json/single_include
)



