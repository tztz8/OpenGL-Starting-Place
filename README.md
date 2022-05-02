# OpenGL-Starting-Place
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

#### Note using icon from icons8

https://icons8.com/icon/XpIRMmWi64sU/cube

https://icons8.com/license
