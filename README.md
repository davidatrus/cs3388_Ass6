Final Project for CS3388, which aims to generate a basic water shader that uses tesselation,
geometry shaders, and displacement maps.
At a high-level, your program will:
• Take a simple mesh of triangles as input.
• Tesselate that mesh to get more vertices to play with in the geometry shader.
• Manipulate the mesh in the geometry shader to create waves.
• Add some other texture/depth to the generated waves to make it look good.
The end product of this assignment is to have a program which just renders a patch of water.


+ Designed and implemented a real-time water simulation using C++ and OpenGL, employing advanced techniques such as tessellation, geometry shaders, and displacement mapping to render dynamic waves and a boat.
+ Developed a modular architecture by separating shader management, water-rendering logic, and camera controls, enhancing code maintainability and scalability—key for building robust test utilities and automated systems.
+ Implemented interactive keyboard controls to adjust tessellation levels and camera zoom in real-time, demonstrating rapid prototyping and user-focused design.
+ Integrated Gerstner wave algorithms within the geometry shader to simulate realistic water movement, and manually computed surface normals for accurate lighting using Phong shading techniques.
+ Diagnosed and resolved asset-loading challenges with BMP textures through effective debugging and workaround solutions, highlighting strong troubleshooting skills.
