# Meadow
![](https://github.com/Willecode/Meadow/blob/c1b37014e97ff206c5079712d8631b7ba411b30f/docs/gifs/goofy_demo10-5.gif)
Demo from 10th of May 2022:
A 3D model created by me, rendered in Meadow :).

# About
Meadow is an interactive graphics software, it is an ongoing personal project for learning purposes.

# Features
- Original rendering engine, which uses OpenGL.
- Synchronous event system to handle input envents.
- Assets such as 3D models can be in various formats, and textures as well.
- Primitive shapes (cubes and spheres) can be generated.
- Basic mesh transformation
- Basic, flexible material system
- Point lights and directional lights
- FPS camera

# In the future
- Cool shaders using normal maps and such
- Opaque objects
- Shadows
- Basic collision and physics simulation
- Object selection by clicking an object in the 3D world
- Object highlighting, selection 

# Funky stuff
![](https://github.com/Willecode/Meadow/blob/a546cd7f2eda68c37171eb0044a7f5662d2e7939/docs/gifs/transparency_bug.gif)
Cool transparency bug:
- The windows are drawn in order: **Right background window**, **front window**, **left background window**
- When the **front window** is in front of the **right back window**, the **back window** gets drawn first, and then the **front window** get's drawn on top of it. Thus, the blending - directed by the translucency (alpha) of the **front window** - produces the desired result where the **back window** can be seen through the **front window**.
- When the **front window** is in front of the **left back window**, the **front window** gets drawn first. When the **back window** is drawn, the **front window** has already been drawn in front of it, and thus all of it's fragments fail their depth tests, and the **left back window** will not be drawn at all.

Solution:
Draw translucent objects last, and always in order from furthest to nearest from camera.
But what if an object is both in front of and behind another object? ....don't think about it if you wish not to lose sleep.
