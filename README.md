# 3D Render Engine using OpenGL
> This is a project with which I aim to learn OpenGL and graphics rendering at all. <br />
> At this moment the project is not running like a normal application as it is still early in development. <br />
> My goal is to achieve something close to the "Marmoset toolbag" rendering software. <br />

## How to run the project
The project is build using CMake.

At this moment the project uses resources from a hardcoded path to the folder `Source/Application/resources`. <br />
It is not mandatory to have this folder as only the 3D models and textures are loaded from there.

You can download this folder from google drive [Here](https://drive.google.com/file/d/1Y9gyZOd7ii43Vctqx7i_DajzkOxWePFW/view?usp=sharing) <br />

## Navigation
When you run the project you will be greated with a black screen. Don't worry, the object will pop up the moment you start moving the camera <br />

### Camera controls:
**Hold right click inside the window to enter the camera control <br />
Move mouse to rotate the camera <br />
W, A, S, D to move camera around**<br />


### User Interface
I am making use of ImGui for the UI. <br />
For now it will pop up as separated window from the renderer where you can find a `Scene tree`, `Object Properties` and `Camera Properties`

![Screenshot_2](https://github.com/Ivailo41/OpenGL-Render-Engine/assets/115023277/49bf90c2-9e72-4e14-92b0-744536fab3a5)

Here is a screenshot from the renderer <br />
![Screenshot_1](https://github.com/Ivailo41/OpenGL-Render-Engine/assets/115023277/c94ddd9d-745d-4b83-a24d-c2a825d6a27c) 

## Load your own 3D model
For now the engine only reads `.obj` files. I wrote the parser myself and for now there are a few limitations. <br />
First you need to export your 3D model `triangulated` as `.obj`. Also make sure that the materials are included. Add the model with its textures to the `resources` folder. <br />

At this point it is not possible to use the UI to load the model so you will need to make little changes in the `main.cpp` file. <br />
You will need to load the textures, then load the model and finally set the textures in the model material <br />
```
Texture::loadTexture("resources/[your texture name]");
Texture::loadTexture("resources/[your texture name]"); //loads these 2 textures to a texture array

mainScene.loadObject("resources/[your model name]"); //loads the model with the given name

Material::getMaterial(0)->setTexture(Texture::textures[0], 0); //gets the material with index 0 and sets the first texture to the 0th slot (0th slot is for base texture)
Material::getMaterial(0)->setTexture(Texture::textures[1], 2); //gets the material with index 0 and sets the second texture to the 2nd slot (2nd slot is for normal texture)
```
Depending on how many materials your model has, this much will be created to the material array, with the last 2 lines you can add textures to the materials by specifying the texture and the material slot. <br />
the 1st material slot is for Occolusion, Roughness, Metallic texture which is combined.

## Footer
Not much to be seen, the project is still early in development <br />
I made use of these libraries:
- ImGui
- glfw
- glew

# Developing branch
> Here I will put current features that are being worked on

## New viewport
Working on making the viewport as a UI layer that can be dragged and doced on the main window.
![engine_preview](https://github.com/Ivailo41/OpenGL-Render-Engine/assets/115023277/7820df2c-25a3-482b-a4ad-b10b6f536ec0)

## Shader corrections
Fixing some issues with the shaders which were calculating the reflections incorrectly
![renderEngine1](https://github.com/Ivailo41/OpenGL-Render-Engine/assets/115023277/4c395c17-dd62-4126-a725-81590ad9c492)
![renderEngine2](https://github.com/Ivailo41/OpenGL-Render-Engine/assets/115023277/2982ac31-47ed-46fd-a1ee-87ca07619335)

