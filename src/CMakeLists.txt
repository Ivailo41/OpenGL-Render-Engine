add_executable(Application)

include_directories("../external/glew/include" "../external/glfw/include" "../external/glm" "../external/stb_image")

target_link_libraries(Application PRIVATE ImGui_Beta glew_s glfw)

#the target source is Application
target_sources(
    Application 
    PRIVATE
        "BaseObject.cpp"
        "Camera.cpp"
        "FileManager.cpp"
        "Main.cpp"
        "Material.cpp"
        "Object.cpp"
        "Shader.cpp"
        "Texture.cpp"
        "UI/EngineUI.cpp"
        "Lights/Light.cpp"
        "Mesh.cpp"
        "Lights/PointLight.cpp"
        "Editor/Scene.cpp"
        "TransformGizmo/TransformGizmo.cpp" 
        "UI/Layers/UI_CameraProperties/UI_CameraProperties.cpp"
        "UI/Layers/UI_ObjectProperties/UI_ObjectProperties.cpp"
        "UI/Elements/UIElement.cpp"
        "UI/Layers/UILayer.cpp"
        "UI/Layers/UISceneTree.cpp"
        "UI/Elements/UISceneNode.cpp" "UI/Layers/UI_Scene/UI_Scene.h" 	"UI/Layers/UI_Scene/UI_Scene.cpp"
	    "GL_Objects/FrameBuffer.cpp" "GL_Objects/Window.h" "GL_Objects/Window.cpp" 
        "UI/Layers/UI_Settings/UI_Settings.h" "UI/Layers/UI_Settings/UI_Settings.cpp" 
        "Debug/DebugShapes.h" "Debug/DebugShapes.cpp" "Debug/LinesContainer.h" "Debug/LinesContainer.cpp" 
        "PostProccess/Bloom.h" "PostProccess/Bloom.cpp" "GL_Objects/FrameQuad.h" "GL_Objects/FrameQuad.cpp" "GL_Objects/Cubemap.h" "GL_Objects/Cubemap.cpp" "Objects/Skybox.h" "Objects/Skybox.cpp")