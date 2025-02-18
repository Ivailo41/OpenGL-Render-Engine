add_executable(Application)

include_directories("../external/glew/include" "../external/glfw/include" "../external/glm" "../external/stb_image" "../external/ImGui_Beta")

target_link_libraries(Application PRIVATE ImGui_Beta glew_s glfw)

#the target source is Application
target_sources(
    Application 
    PRIVATE
        "Scene/BaseObject.cpp"
        "Scene/Camera.cpp"
        "FileManager.cpp"
        "Main.cpp"
        "Renderer/Material.cpp"
        "Renderer/Shader.cpp"
        "Renderer/Texture.cpp"
        "UI/EngineUI.cpp"
        "Scene/Lights/Light.cpp"
        "Scene/Mesh.cpp"
        "Scene/Lights/PointLight.cpp"
        "Scene/Scene.cpp"
        "UI/Layers/UI_CameraProperties.cpp"
        "UI/Layers/UI_ObjectProperties.cpp"
        "UI/Elements/UIElement.cpp"
        "UI/Layers/UILayer.cpp"
        "UI/Layers/UISceneTree.cpp"
        "UI/Elements/UISceneNode.cpp" "UI/Layers/UI_Scene.h" 	"UI/Layers/UI_Scene.cpp"
	    "Renderer/FrameBuffer.cpp" "Core/Window.h" "Core/Window.cpp" 
        "UI/Layers/UI_Settings.h" "UI/Layers/UI_Settings.cpp" 
        "Renderer/DebugShapes.h" "Renderer/DebugShapes.cpp" "Renderer/LinesContainer.h" "Renderer/LinesContainer.cpp" 
        "Renderer/PostProccess/Bloom.h" "Renderer/PostProccess/Bloom.cpp" "Renderer/FrameQuad.h" "Renderer/FrameQuad.cpp" "Renderer/Cubemap.h" "Renderer/Cubemap.cpp" "Scene/Skybox.h" "Scene/Skybox.cpp")