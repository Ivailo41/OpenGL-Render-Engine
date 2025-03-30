# 3D Render Engine using OpenGL
This project is a 3D rendering engine implemented using `OpenGL`, created as part of my exploration into computer graphics and low-level programming. It serves as a practical learning tool to deepen my understanding of rendering pipelines, graphics APIs, and performance optimization, while also improving my overall software development skills.

`This image is from the latest push in the Developing branch`
![RenderEngine](https://github.com/user-attachments/assets/6eef5678-496a-42fd-9ca7-5d1011d6a4eb)

# Features:
- PBR shading
- 3D models loading
- multithreaded textures loading
- post-processing
- GUI

# Build and How to run the project
The project can be built with CMake. All the libraries are present in the repository so no extra downloads are needed.

The project uses resources from a hardcoded path to the folder `assets`. <br />
It is not mandatory to have this folder as only the 3D models and textures are loaded from there. <br />
You can load models and textures from the UI and set the materials. <br />
### (!!Currently, there is an issue with textures vector so pleace comment out line 165 in Main.cpp if you don't intend to load textures from there)
```cpp
fileManager.loadTextures(texturePaths);
```

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
The parser can read ONLY OBJ files at the moment. They need to be exported as triangulated and have a material. <br />
`Currently there is no support for multi-material mesh` <br />
If you want to use the UI loading, navigate to the `Import Model` button in the `Settings` pannel and find your model. <br />
If you want to hardcode the model loading (which later will be moved to Scene loading) inside the Main.cpp navigate to line 168 <br />
```cpp
if (fileManager.loadOBJ("../assets/AK203/AK203.obj"))
{
  mainScene.materials[2]->setTexture(mainScene.textures[0], 0);
  mainScene.materials[2]->setTexture(mainScene.textures[1], 1);
  mainScene.materials[2]->setTexture(mainScene.textures[2], 2);
}
```
Set the path to the model location (preferably in the `assets` folder) <br />
The code inside the if block is to set the textures of the materials manually by specifying the texture and the material slot. <br />
the 1st material slot is for Occolusion, Roughness, Metallic texture which is combined.

# Load your textures

Loading textures can be done either from the button `Import Texture` under the `Import Model` <br />
Or they can be batch-loaded with this function at line 151-165
```cpp
 std::vector<std::string> texturePaths = { 
                                            "../assets/AK203/Set1_Base.png",
                                            "../assets/AK203/Set1_ORM.png",
                                            "../assets/AK203/Set1_Normal.png",
                                         };

fileManager.loadTextures(texturePaths);
```

# Libraries
I made use of these libraries:
- ImGui - UI library
- ImGuizmo - Used for model manipulation with gizmos
- glfw - Window creation
- glew - OpenGL API
- glm - Math functions with matrices and vectors
- stbi_image - Image loader

# More Screenshots

## The AK 3D model is made by me, here is a link to renders done using Marmoset https://www.artstation.com/artwork/9NOP8q
![RenderEngine2](https://github.com/user-attachments/assets/c78f693a-14a7-4509-8e63-6a446a2dbac6)

![RenderEngine3](https://github.com/user-attachments/assets/692df58e-1ad8-4b80-a145-1503feaf9c13)

![Bloom2](https://github.com/user-attachments/assets/58929429-59f5-454f-9eb0-49c80a655c56)

![RenderEngine4](https://github.com/user-attachments/assets/7fe92eab-8b4a-4869-bc61-7912920bf808)
