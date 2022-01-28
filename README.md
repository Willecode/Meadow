# OpenGL_playground

**ONLY FOR CODE BROWSING/VIEWING, THIS REPO DOESN'T INVOLVE ALL DEPENDENCIES FOR BUILDING IT YOURSELF**

This is my OpenGL learning place, where I push demos I have made with the help of https://learnopengl.com/. The project involves source code provided by the website. **My code is in src/*_demo.cpp and shaders/. This project is in it's early stages, more demos will come.**

# First demo
![](https://github.com/Willecode/OpenGL_playground/blob/300d6c4c5e4c979e3984dc671672613facb80b16/movement_demo.gif)

Involves **camera movement**, **perspective projection**, **texture mixing**, **rotating** objects randomly.

The bulk of the time spent on this went to understanding buffers, binding, and the state machine design of OpenGL. VAOs, VBOs and EBOs (IBOs) aren't very intuitive at first, I find: Some of the OpenGL functions that handle them take relatively exotic arguments, for example. The vertex buffer object is the simplest. You first create the VBO, and OpenGL gives you it's ID. Then, you must bind the VBO, which is to tell OpenGL that this VBO is now the "active" one, and will be the target of all the following function calls that target the array buffer. Furthermore, the bound VBO's data will be used in the following draw calls (when we're drawing shapes on the screen). To populate the VBO's data storage with data, you pass in an array (of floats, usually). This data can then be parsed into arguments for the **vertex shader**. How can this parsing be done? The answer is the VAO.

The purpose of the VAO (vertex array object), is to provide information regarding how OpenGL should handle the data you've stored in your VBO's storage (the VBO's storage is just a buffer in your GPU's memory). You may for example tell OpenGL that the elements in the array that you passed to the VBO are 3D coordinates of vertices. In that case you'd configure your VAO so that the first three elements represents the first vertex, the next three represent the next one etc. The practical usage of the VAO is similar to the VBO: you first create the VAO, you bind it, and then use it's methods to configure it.

EBOs (element buffer objects) are optional, and used for telling OpenGL which vertices should be connected with an edge (aka. a line). All shapes are drawn as combinations of triangles, and three vertices are needed to form a triangle. By default, OpenGL decides that the first three vertices in your VBO data are connected, so are the next three etc. This means that to create connected shapes you'd need to put vertices on top of each other, since each vertex can only be part of one triangle by default. This is where the EBO comes in. You may pass vertex indices to the EBO, telling OpenGL where to draw edges. This way, if you want to draw, say, a square plane, for which you need two triangles, you only need four vertices, and an EBO to tell how to connect these vertices to form the two triangles. Without the EBO you'd need six vertices: three for the first triangle and three for the other. Therefore, the EBO is excellent for optimization.

Another obstacle was the more general - not OpenGL specific - mathematics involved in operations such as scaling, rotation and translation of objects, and transforming vertices to other coordinate systems. These operations are performed using vectors, matrices and trigonometry, and I won't be discussing them in detail for now.

TODO: write about texture mapping

# Second demo
![](https://github.com/Willecode/OpenGL_playground/blob/75e4ed820efb91bd4aa46a51afe44852a004c8d0/light_demo_early.gif)

Phong shading implemented. Next up probably lighting with base color maps and specular maps.
