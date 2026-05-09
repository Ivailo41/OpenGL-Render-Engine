add_executable(Application
		Resources/ResourceManager.cpp
		Resources/ResourceManager.h
		Resources/Mesh.cpp
		Resources/Mesh.h
		Resources/Raw/RawStructs.cpp
		Resources/Raw/RawStructs.h
		Resources/Model.cpp
		Resources/Model.h
		UI/Layers/UI_AssetBrowser.cpp
		UI/Layers/UI_AssetBrowser.h
		UI/IconsFontAwesome7.h
		UI/Layers/UI_Materials.h
		UI/Layers/UI_Materials.cpp "Scene/TransformComponent.h" "Scene/TransformComponent.cpp")

include_directories("../external/glew/include" "../external/glfw/include" "../external/glm" "../external/stb_image" "../external/ImGui_Beta")

target_link_libraries(Application PUBLIC external glew_s glfw)

#the target source is Application
target_sources(
    Application 
    PRIVATE
        "Scene/SceneNode.cpp"
        "Scene/Camera.cpp"
        "Core/FileManager.cpp"
        "Main.cpp"
        "Resources/Material.cpp"
        "Resources/Shader.cpp"
        "Resources/Texture.cpp"
        "UI/EngineUI.cpp"
        "Scene/Lights/Light.cpp"
        "Scene/ObjectMesh.cpp"
        "Scene/Lights/PointLight.cpp"
        "Scene/Scene.cpp"
        "UI/Layers/UI_CameraProperties.cpp"
        "UI/Layers/UI_ObjectProperties.cpp"
        "UI/Layers/UILayer.cpp"
        "UI/Layers/UISceneTree.cpp"
        "UI/Layers/UI_Scene.h" 	"UI/Layers/UI_Scene.cpp"
	    "Renderer/FrameBuffer.cpp" "Core/Window.h" "Core/Window.cpp" 
        "UI/Layers/UI_Settings.h" "UI/Layers/UI_Settings.cpp" 
        "Renderer/DebugShapes.h" "Renderer/DebugShapes.cpp" 
        "Renderer/LinesContainer.h" "Renderer/LinesContainer.cpp" 
        "Renderer/PostProccess/Bloom.h" "Renderer/PostProccess/Bloom.cpp"
        "Renderer/FrameQuad.h" "Renderer/FrameQuad.cpp"
        "Renderer/Cubemap.h" "Renderer/Cubemap.cpp"
        "Scene/Skybox.h" "Scene/Skybox.cpp" 
        "Math/Math.cpp" "Math/Math.h" 
        "Renderer/Renderer.h" "Renderer/Renderer.cpp" 
        "Core/Engine.h" "Core/Engine.cpp"
        "Core/Logger.h" "Core/Logger.cpp" "UI/Layers/UI_Console.h" "UI/Layers/UI_Console.cpp")