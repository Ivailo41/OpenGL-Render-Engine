add_library(glew SHARED IMPORTED)

target_include_directories(glew PRIVATE "Libraries/glew-2.1.0/include"
									   "Libraries/glfw-3.3.8.bin.WIN64/include")

target_link_directories(glew PRIVATE "Libraries/glew-2.1.0/lib/Release/x64"
									"Libraries/glfw-3.3.8.bin.WIN64/lib-vc2022")

target_link_libraries(
	glew
	PRIVATE
        glew32s
        opengl32
        glfw3dll
        glfw3_mt
        glfw3
)

#add_library(glfw SHARED IMPORTED)

#target_include_directories(glfw PRIVATE "Libraries/glfw-3.3.8.bin.WIN64/include")

#target_link_directories(glfw PRIVATE "Libraries/glfw-3.3.8.bin.WIN64/lib-vc2022")

#target_link_libraries(
#	glfw
#	PRIVATE
#        glfw3dll
#        glfw3_mt
#        glfw3
#)