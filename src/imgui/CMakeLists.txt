list(APPEND IMGUI_SOURCES
        imgui.cpp
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_impl_glfw.cpp
        imgui_impl_opengl3.cpp
        imgui_widgets.cpp)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC ../../ext/glfw/include)