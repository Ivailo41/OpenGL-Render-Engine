# 3D Render Engine using OpenGL
This project is a 3D rendering engine implemented using `OpenGL`, created as part of my exploration into computer graphics and low-level programming. It serves as a practical learning tool to deepen my understanding of rendering pipelines, graphics APIs, and performance optimization, while also improving my overall software development skills.

<img width="1919" height="1035" alt="RE_scrn4" src="https://github.com/user-attachments/assets/a1e177cd-1d43-4a74-9900-63e86bb2a766" />

# Features:
- PBR shading
- 3D models loading
- multithreaded textures loading
- Materials editing
- post-processing
- GUI
- Windows & Linux support

# Build and How to run the project
The project can be built with CMake. All the libraries are present in the repository so no extra downloads are needed.

The project uses resources from a hardcoded path to the folder `assets`. <br />
It is not mandatory to have this folder as only the 3D models and textures are loaded from there. <br />
You can load models and textures from the UI and set the materials. <br />

# Navigation

**Hold right click inside the window to enter the camera control <br />
Move mouse to rotate the camera <br />
W, A, S, D to move camera around**<br />
When an object is selected a gizmo will appear: <br />
W - Translation gizmo <br />
E - Rotation gizmo <br />
R - Scale gizmo <br />

# Load your own 3D model
`The engine uses a parser written by me and it has its' limitations.` <br />
The parser can read ONLY OBJ files at the moment. <br />
If you want to use the UI loading, navigate to the `Import Model` button in the `Settings` pannel and find your model. <br />
If you want to hardcode the model loading (which later will be moved to Scene loading) inside the src/Core/Engine.cpp navigate to line 90 <br />
```cpp
if (resourceManager.loadOBJ("../assets/AK203/AK203.obj"))
{
  std::vector<std::string> texturePaths = {..}

  resourceManager.loadTexture(texturePaths);
}
```
Set the path to the model location (preferably in the `assets` folder) <br />

# Load your textures

Loading textures can be done either from the button `Import Texture` under the `Import Model` <br />
Or they can be batch-loaded with this function inside the if block from model loading above
```cpp
 std::vector<std::string> texturePaths = { 
                                            "../assets/AK203/Set1_Base.png",
                                            "../assets/AK203/Set1_ORM.png",
                                            "../assets/AK203/Set1_Normal.png",
                                         };

resourceManager.loadTextures(texturePaths);
```

# Libraries
I made use of these libraries:
- ImGui - UI library
- ImGuizmo - Used for model manipulation with gizmos
- glfw - Window creation
- glew - OpenGL API
- glm - Math functions with matrices and vectors
- stbi_image - Image loader
- tinyfiledialogs - OS agnostic file dialogs

# More Screenshots

## The AK 3D model is made by me, here is a link to renders done using Marmoset https://www.artstation.com/artwork/9NOP8q
<img width="1920" height="1036" alt="RE_scrn7" src="https://github.com/user-attachments/assets/1a97797e-915a-410b-99d4-b3fa04686c29" />
<img width="1920" height="1034" alt="RE_scrn6" src="https://github.com/user-attachments/assets/7a7ba8f2-34c3-403a-bb2f-0d120b9c8794" />
<img width="1919" height="1027" alt="RE_scrn3" src="https://github.com/user-attachments/assets/27ac1e58-cf0a-472e-9315-ddf06c05ad8c" />
<img width="1920" height="1036" alt="RE_scrn5" src="https://github.com/user-attachments/assets/a1e0d0c4-e3f0-41f0-9c80-d7720224bb2b" />

