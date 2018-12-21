cmake_minimum_required(VERSION 3.12)
project(2lab_cpp)

set(CMAKE_CXX_STANDARD 17)
#add_compile_options(-pthread)
find_package(Threads REQUIRED)

add_executable(2lab_cpp main.cpp matix_reader.cpp)

target_link_libraries(2lab_cpp ${CMAKE_THREAD_LIBS_INIT})


