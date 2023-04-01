message("Minimum CMake 3.16")
cmake_minimum_required(VERSION 3.16)

if(APPLE)
    set(CMAKE_C_COMPILER "/usr/local/bin/gcc-12")
    set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-12")
    set(CMAKE_CXX_FLAGS "-O3 -Wall -g")
    message("Set C++ 17 as standard")
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_BUILD_TYPE Debug)
endif()

include_directories(${OLAS_SOURCE_DIR}/src /usr/local/include)
link_directories(/usr/local/lib)

message("Building Project: OLAS")
project(OLAS)

message("Listing all include directories")
add_subdirectory(src)

message("Create executable: OLAS.exe")
add_executable(${PROJECT_NAME} main.cpp)
# add_executable(OLAS_cv main_gl_cv.cpp)

message("Linking libraries to OLAS.exe")
target_link_libraries(${PROJECT_NAME} olib glfw imgui)
# target_link_libraries(OLAS_cv olib glfw imgui)
