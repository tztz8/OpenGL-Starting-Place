# ![cube](basicGLFW/res/icon/cube.png) OpenGL-Starting-Place
> OpenGL Starting Place, 
> A Template to get started making opengl projects and assignments,
> Mostly using c code in cpp files

| freeglut                               | glfw                           |
|----------------------------------------|--------------------------------|
| ![freeGlut Window](freeglutWindow.gif) | ![GLFW Window](glfwWindow.gif) |


I Used [EWU](https://www.ewu.edu/) Windows computers and My [Manjaro](https://manjaro.org/) Linux Computer for testing.

Note the systems have [FreeGLUT](http://freeglut.sourceforge.net/), [GLM](https://github.com/g-truc/glm), [GLEW](http://glew.sourceforge.net/) and, often have [DevIL](http://openil.sourceforge.net/) installed

On EWU Windows computers uses [Visual Studio](https://visualstudio.microsoft.com/) 2019

On My Manjaro Linux computer I use [CLion](https://www.jetbrains.com/clion/)

## Nvidia GPU on Linux

To use Nvidia GPU insed of integrated GPU on Linux add 
```shell
__NV_PRIME_RENDER_OFFLOAD=1;
__GLX_VENDOR_LIBRARY_NAME=nvidia;
```
To environment variables

![Run Configurations in CLion](basicGLFWRunConInClion.png)

## Documentation

| OpenGL/GLSL                                                                                                          | GLFW                                      | freeGLUT                                                                 |
|----------------------------------------------------------------------------------------------------------------------|-------------------------------------------|--------------------------------------------------------------------------|
| [OpenGL (unofficial)](https://docs.gl/)                                                                              | [GLFW](https://www.glfw.org/docs/latest/) | [freeGLUT](http://freeglut.sourceforge.net/docs/api.php)                 |
| [OpenGL (4.5 Reference Pages)](https://www.khronos.org/registry/OpenGL-Refpages/gl4/)                                |                                           | [GLUT](https://www.opengl.org/resources/libraries/glut/spec3/spec3.html) |
| [OpenGL (opengl45-quick-reference-card)(Not quick)](https://www.khronos.org/files/opengl45-quick-reference-card.pdf) |                                           |                                                                          |


#### Note using icon from icons8

![cube](basicGLFW/res/icon/cube.png)

https://icons8.com/icon/XpIRMmWi64sU/cube

https://icons8.com/license
