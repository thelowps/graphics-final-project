CSE 40166 Computer Graphics
Fall 2012
David Lopes

Description:
    This program depicts a cat lifting its paw to its mouth. It demonstrates
    the use of shaders to deform meshes and performing advanced lighting
    techniques (in this case, cel-shading, or toon shading).

Usage: 
    The user can click with the left mouse button and drag to affect the
    current angluar directionof the camera; left <--> right affects the
    camera's theta value ("horizontal" angle), and up <--> down affects
    the camera's phi value ("vertical" angle). 
    
    Pressing the 'i' and 'k' keys on the keyboard causes the camera 
    to move forward or backward, respectively. The keys can be held down
    to move constantly, and mouse motion can be used at the same time
    for a smoother flying experience.

    The user can press the 'r' key to animate the lights.

    Pressing the 'a', 's', 'd', 'w' keys moves the object along the X and
    Z axes.

    The user can also press the 'q' key to quit the program.    

Compilation Instructions:
    Simply navigate to the directory and type 'make.' Only main.cpp needs
    to be linked with the OpenGL / freeglut libraries as well as the 
    CSE40166 library.  CSE40166 also depends on OpenAL, freealut, and
    SOIL.
