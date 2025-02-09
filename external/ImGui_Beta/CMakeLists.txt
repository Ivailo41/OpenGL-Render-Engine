add_library(ImGui_Beta STATIC)

target_include_directories(ImGui_Beta PRIVATE "../glew-2.1.0/include"
                                        "../glfw-3.3.8.bin.WIN64/include")

target_link_directories(ImGui_Beta PRIVATE "../glew-2.1.0/lib/Release/x64"
										"../glfw-3.3.8.bin.WIN64/lib-vc2022")

target_link_libraries(
	ImGui_Beta
	PRIVATE
        glew32s
        opengl32
        glfw3dll
        glfw3_mt
        glfw3
)

target_sources(ImGui_Beta PRIVATE "imgui.cpp"
                                    "imgui_demo.cpp"
                                    "imgui_draw.cpp"
                                    "imgui_impl_glfw.cpp"
                                    "imgui_impl_opengl3.cpp"
                                    "imgui_tables.cpp"   
                                    "imgui_widgets.cpp")