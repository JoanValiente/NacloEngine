# Welcome

This is the Naclo Engine web page. Here you will find all important information you need to know about our engine
and about us, and a link to the repository and the lastest release download.

# Index

1. [About the engine](#about)
2. [Members](#members)
3. [Video](#video)
4. [Engine subsystems](#subsystems)
 - [GameObjects & Components](#go)
 - [Importers & own binary format](#importers)
 - [Camera](#camera)
 - [Space partitioning](#quadtree)
 - [Scene serialization](#serialization)
 - [Mouse picking](#picking)
 - [Game Mode](#gamemode)
5. [UI system](#ui)
6. [Controls](#controls)
7. [Download](#download)
8. [License](#license)



# About the engine <a name="about"></a>

Naclo Engine is a 3D game engine being developed for the "game engines" subject of GameDev grade of CITM (Terrasa).
The objective of this subject is to develop a game engine in c++, create the main systems of game engines and learn how they work.

Project GitHub repository [here](https://github.com/JoanValiente/NacloEngine)



# Members <a name="members"></a>

David Lozano. [GitHub](https://github.com/DavidTheMaaster)

![Captura3](https://raw.githubusercontent.com/JoanValiente/NacloEngine/master/docs/Captura3.PNG)

Joan Valiente. [GitHub](https://github.com/JoanValiente/)

![Captura2](https://raw.githubusercontent.com/JoanValiente/NacloEngine/master/docs/Captura2.PNG)


# Video <a name="video"></a>



# Engine sub-systems <a name="subsystems"></a>

Our engine contain the next sub-systems:

## Game objects and components <a name="go"></a>

A game objects hierarchy system with its components. There are 12 different components.

 - The main ones: transform, mesh, material and camera.
 - And the UI ones: canvas, interactive, rect_transform, image, button, label, checkbox and inputbox.

Game objects have hierarchy showed in the hierarchy panel, on the left of the screen. You can select game objects by clicking them in this panel, or change the hierarchy by dragging them.
We can create empty game objects with Create->GameObject->CreateEmpty.

Selecting a gameobject will show its inspector panel. Here you can find the main information and options about the game object (name, static value, option to delete it, etc), and the information and options of its components.
You can add components to a game object by clicking the "Add Component" button on the inspector.

## Importers and own binary format for files <a name="importers"></a>

You can import a file (dragging it into the engine), and our own binary format for that files will be generated in library folder. You can import textures or meshes (in .FBX).

The modules that are resposible for importing the files save the useful data in 2 types of file: .ncl for meshes and .dds for textures. They save all the useful data for load that mesh or texture
and charge it in the scene faster.

## Camera <a name="camera"></a>

Both the main camera and the editor camera, works with the camera component (whose is handled by a frustum). 
Our camera module has a ComponentCamera and it handles unity-like camera motion and mouse picking (see below).

You can change some values of the frustum like the Fov, the far and near plane distance, the speed (for editor camera), etc. Main camera options can be changed in the inspector of its GameObject, and editor camera options can be changed on the configuration window.

You can also activate the "culling" option that will discard the render of meshes out the frustum, every mesh have an AABB and if it is outside camera frustum it is not drew, in order to optimize the engine.

## Space Partitioning / Quadtree <a name="quadtree"></a>

Static objects are accelerated using a quadtree. The quadtree organize the scene in a tree to discard big portions of it ASAP, and avoid expensive collision tests.
It is used with mouse picking and the renderer.

## Scene serialization <a name="serialization"></a>

The scene can be saved in MainMenu->Save. The scene will be serialized and can be loaded with MainMenu->Load. The scene is also saved when game is played.
When a scene is loaded, the current hierarchy is replaced by the saved one.

## Mouse picking <a name="picking"></a>

When you click on the screen, a ray is casted from mouse to the far plane of camera frustrum. All objects that intersect with that ray are tested and the nearest to near plane for frustrum is picked. 
This is used to have another way to select game objects than with the hierarchy panel.

## Game mode <a name="gamemode"></a>

On the top of the window we can find a bar with "play", "pause" and "stop" buttons, and a time scale option.
We can activate the game mode by clicking the Play button on the top of the screen. This will change the camera responsible for render to the Main Camera. Pause button will pause the game mode but it won't stop.
Stop button returns to the editor mode, and every change on scene will be reseted to the state saved when we clicked the Play button. 
Time scale modify the time multiplier for game mode. Next to this bar we can see the runtime when we change to game mode.

# UI system <a name="ui"></a>
We have implemented a UI system based on Unity UI. It is based on 3 basic componets: canvas, rect transform and interactive; 
and different components for each ui element: button, image, label, etc.

### Rect transform

The Rect Transform component is the 2D layout counterpart of the Transform component. Rect Transform
represent a rectangle that a UI element can be placed inside. If the parent of a Rect Transform is also a
Rect Transform, the child Rect Transform can also specify how it should be positioned and sized relative to
the parent rectangle.

As properties it has:
- Position.
- Rotation.
- Size.
- Width.
- Height.
- Pivot.
- MinAnchor.
- MaxAnchor.
- Local Matrix.
- Global Matrix.

### Canvas

This game object is used to create an area where all UI elements should be and it must be the parent of
all UI gameobjects in the scene hierarchy. This game object adapts itself to the screen when you start the game mode,
making all its children do the same thanks to the rect transform functionality. 

### Interactive

## UI Elements

## Image
The image component creates a plane (using the rect transform values of its container) where you can put a texture to show. This can be use to create icons, windows, HUD, etc.
It inherits from interactive component, so you can get input events like clicks or hovers, or make it draggable.
The render of this component also handles transparency. 

As properties it has:
- Texture.
- ImagePlane.
- Color.
- Size. 

## Button


As properties it has:
- Texture.
- ImagePlane.
- Color.
- Size. 

# Controls <a name="controls"></a>

* Mouse Right button: Look around.
* Left Alt + Mouse Left button: Look aroud target.
* Mouse Right (hold) + WASD: forward, left, backward and right movement.
* Arrows: forward, left, backward and right movement.
* F: Focus camera to the geometry.
* Mouse wheel: zoom.
* Mouse wheel pressed: Up, right, left and down movement.
* ShortCuts:
	- 1: Open/Close Configuration Window.
	- 2: Open/Close Inspector.
	- 3: Open/Close Console.
	- 4: Open/Close About Window.
	- Shift + C: Enable/Disable Checkers mode.
	- Shift + M: Enable/Disable Wire mode.



# Download last release <a name="download"></a>
NacloEngine 1.0


# License <a name="license"></a>

Copyright 2018 MIT.
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.