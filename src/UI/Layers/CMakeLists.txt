add_subdirectory(UI_CameraProperties)
add_subdirectory(UI_ObjectProperties)

target_sources(Layers PRIVATE "UILayer.cpp"
                                "UIScene.cpp")