cmake_minimum_required (VERSION 3.26)

project("OpenGL Renderer" VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Exe file is in the souce files directory because of temporary code in main.cpp
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)

add_subdirectory("src")
add_subdirectory("external/glfw")
add_subdirectory("external/glew/build/cmake")
add_subdirectory("external/ImGui_Beta")