cmake_minimum_required(VERSION 3.23)
project(PracticaArqui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS_OFF)
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)

set(CMAKE_CXX_FLAGS_RELEASE "-march=native")

add_library(common file1−common.cpp file2−common.cpp file3−common.cpp)
add_library(aos file1−aos.cpp file2−aos.cpp)
add_library(soa file1−soa.cpp file2−soa.cpp)


add_executable(imgaos image-aos.cpp)
target_link_libraries(imgaos common.aos)
add_executable(imgsoa image-soa.cpp)
target_link_libraries(imgsoa common soa)
