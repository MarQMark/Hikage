project(Hikage)

add_executable(Hikage
        ./glew/glew.c

        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_impl_glfw.cpp
        imgui/imgui_impl_opengl3.cpp
        imgui/imgui_stdlib.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp

        main.cpp)

target_include_directories(Hikage
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../external/glfw/include
)
target_link_directories(Hikage PRIVATE external/glfw/src)

target_link_libraries(Hikage glfw)

add_compile_definitions(GLEW_STATIC)


find_package(OpenGL REQUIRED)
target_link_libraries(Hikage glut GL dl X11 pthread)