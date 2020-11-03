message("Minimum CMake 3.16")
cmake_minimum_required(VERSION 3.16)

message("Building Project: OLAS")
project(OLAS)

message("Set C++ 17 as standard")
set(CMAKE_CXX_STANDARD 17)

message("Listing all include directories")
include_directories(${OLAS_SOURCE_DIR}/src)
add_subdirectory(src)

message("Create executable: OLAS.exe")
add_executable(${PROJECT_NAME} main.cpp)
add_executable(OLAS_cv main_gl_cv.cpp)

message("Linking libraries to OLAS.exe")
target_link_libraries(${PROJECT_NAME} olib glfw3 imgui)
target_link_libraries(OLAS_cv olib glfw3 imgui)