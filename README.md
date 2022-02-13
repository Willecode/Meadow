# OpenGL_playground

**Right now the project may not build on some machines, since the assimp dll file is built for my machine. I'll try to build assimp as a static library in the future, so that viewers may clone and run the project without hassle.**

This is my OpenGL learning place, where I push demos I have made with the help of https://learnopengl.com/. The project involves some source code provided by the website. Right now this is just a sandbox, but it will potentially develop into a render engine.

Each demo contains gif(s and some insight about what I found interesting or difficult, what time was spent on, what I learned etc. while making it.

# First demo: General OpenGL stuff
![](https://github.com/Willecode/OpenGL_playground/blob/2dfa82c6d87e0cb0e0101d795676d0cf87300143/gifs/movement_demo.gif)

Involves **camera movement**, **perspective projection**, **texture mixing**, **rotating** objects randomly.

The bulk of the time spent on this went to understanding buffers, binding, and the state machine design of OpenGL. VAOs, VBOs and EBOs (IBOs) aren't very intuitive at first, I find: Some of the OpenGL functions that handle them take relatively exotic arguments, for example. The vertex buffer object is the simplest. You first create the VBO, and OpenGL gives you it's ID. Then, you must bind the VBO, which is to tell OpenGL that this VBO is now the "active" one, and will be the target of all the following function calls that target the array buffer. Furthermore, the bound VBO's data will be used in the following draw calls (when we're drawing shapes on the screen). To populate the VBO's data storage with data, you pass in an array (of floats, usually). This data can then be parsed into arguments for the **vertex shader**. How can this parsing be done? The answer is the VAO.

The purpose of the VAO (vertex array object), is to provide information regarding how OpenGL should handle the data you've stored in your VBO's storage (the VBO's storage is just a buffer in your GPU's memory). You may for example tell OpenGL that the elements in the array that you passed to the VBO are 3D coordinates of vertices. In that case you'd configure your VAO so that the first three elements represents the first vertex, the next three represent the next one etc. The practical usage of the VAO is similar to the VBO: you first create the VAO, you bind it, and then use it's methods to configure it.

EBOs (element buffer objects) are optional, and used for telling OpenGL which vertices should be connected with an edge (aka. a line). All shapes are drawn as combinations of triangles, and three vertices are needed to form a triangle. By default, OpenGL decides that the first three vertices in your VBO data are connected, so are the next three etc. This means that to create connected shapes you'd need to put vertices on top of each other, since each vertex can only be part of one triangle by default. This is where the EBO comes in. You may pass vertex indices to the EBO, telling OpenGL where to draw edges. This way, if you want to draw, say, a square plane, for which you need two triangles, you only need four vertices, and an EBO to tell how to connect these vertices to form the two triangles. Without the EBO you'd need six vertices: three for the first triangle and three for the other. Therefore, the EBO is excellent for optimization.

Another obstacle was the more general - not OpenGL specific - mathematics involved in operations such as scaling, rotation and translation of objects, and transforming vertices to other coordinate systems. These operations are performed using vectors, matrices and trigonometry, and I won't be discussing them in detail for now. The GLM-math library was used to ease these operations.

The textures objects, like the aforementioned buffer objects, need to be first created and then bound. After binding, they can be configurated with parameters to adjust texture wrapping and filtering, and the actual texture data can be passed on to it. To create the texture data from an image file, an image loader was used. The loader takes the path to the image file as an argument, and converts the image to the correct format to pass on to OpenGL. Once the texture is passed, the texture can be sampled from fragment shaders by associating a sampler2D uniform with the texture. This means, that the texture data can then be sampled in the fragment shader, which means that if texture coordinates are provided to the fragment shader, it can use those coordinates to take color information from the texture, and use it to color the final fragment. OpenGL makes passing texture coordinates very easy: the coordinates can be first passed to the vertex shader as attributes in the VBO. This means, that now the VBO will store texture coordinates in addition to the vertex coordinates. Therefore, now each vertex will have some texture coordinates associated with it. Now comes the magical part: when you pass variables from the vertex shader to the fragment shader, for each fragment, the fragment shader will receive values which are **interpolated from the vertex shaders associated with the vertices of the triangle that contains the fragment.** Theis means that if you have a square plane and want to map a texture onto it, you only need to specify which vertex of the plane should be in which corner of the triangle.

# Second demo: Lighting
![](https://github.com/Willecode/OpenGL_playground/blob/2dfa82c6d87e0cb0e0101d795676d0cf87300143/gifs/specular-diffuse-map.gif)

Light revealing fingerprints on a shiny wooden surface. Uses phong shading with specular and diffuse maps and materials. Writing more about this soon.

![](https://github.com/Willecode/OpenGL_playground/blob/2dfa82c6d87e0cb0e0101d795676d0cf87300143/gifs/point-light.gif)

Point light with intensity depending on distance (attenuation)

Right now I'm integrating assimp model loader into the project so that I can render more exiting shapes, also doing major refactoring and designing...
#Third demo: 3d model loading and rendering
![](https://github.com/Willecode/OpenGL_playground/blob/12d66da38d8c9cf90866180bb0f67b7a598d0eb4/gifs/spec-diffuse-map-3dmodel.gif)
Model loaded using Assimp library. 3d model by Berk Gedik, https://sketchfab.com/3d-models/survival-guitar-backpack-low-poly-799f8c4511f84fab8c3f12887f7e6b36.
