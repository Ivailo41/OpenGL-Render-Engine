add_library(external STATIC)

add_subdirectory("glfw")
add_subdirectory("glew/build/cmake")
add_subdirectory("ImGui_Beta")

add_library(tinyfiledialogs tinyfiledialogs/tinyfiledialogs.c)

target_include_directories(tinyfiledialogs PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/tinyfiledialogs
)

target_link_libraries(external PUBLIC ImGui_Beta tinyfiledialogs)

target_include_directories(external PUBLIC "ImGuizmo" "ImGui_Beta")

target_sources(external PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImGuizmo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImGuizmo.h
)
