cmake_minimum_required(VERSION 3.28)
project(424_HW2_Q2)
set(CMAKE_CXX_STANDARD 20)

# Add executable target
add_executable(${CMAKE_PROJECT_NAME} 424HW2_Q2.cpp)

# Add the include directory
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})