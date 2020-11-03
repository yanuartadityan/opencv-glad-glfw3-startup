message("Set OpenCV directory")
set("OpenCV_DIR" "C:\\mingw64-8.0.0-r3\\x86_64-w64-mingw32\\x64\\mingw\\lib")
find_package(OpenCV REQUIRED)

add_subdirectory(imgui)
add_subdirectory(glm)

#include_directories(${OLAS_SOURCE_DIR}/include)
add_library(olib
        adder.cpp
        cv_wrapper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/glad/glad.c)

target_link_libraries(olib ${OpenCV_LIBS} glm)