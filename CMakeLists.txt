cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(kronrod)

set(BOOST_ROOT "C:\\Packages\\boost_1_84_0")
find_package(Boost REQUIRED)

add_executable(kronrod main.cpp kronrod.cpp)
add_executable(kronrod_test kronrod_test_T.cpp kronrod.cpp)
target_link_libraries(kronrod Boost::boost)
target_compile_options(kronrod PRIVATE -O3)

# kronrod.cpp kronrod.hpp 