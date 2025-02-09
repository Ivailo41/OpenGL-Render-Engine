add_library(ImGui_Beta STATIC)

target_link_libraries(
	ImGui_Beta
	PRIVATE
        glew_s
        glfw
)

target_sources(ImGui_Beta PRIVATE "imgui.cpp"
                                    "imgui_demo.cpp"
                                    "imgui_draw.cpp"
                                    "imgui_impl_glfw.cpp"
                                    "imgui_impl_opengl3.cpp"
                                    "imgui_tables.cpp"   
                                    "imgui_widgets.cpp")