add_library(external STATIC)

add_subdirectory("glfw")
add_subdirectory("glew/build/cmake")
add_subdirectory("ImGui_Beta")

target_link_libraries(external PUBLIC ImGui_Beta)

target_include_directories(external PUBLIC "ImGuizmo" "ImGui_Beta")

target_sources(external PUBLIC "ImGuizmo/ImGuizmo.cpp" "ImGuizmo/ImGuizmo.h")
