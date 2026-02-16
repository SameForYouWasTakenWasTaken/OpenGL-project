# My OpenGL Project!

## Introduction 👋
Hello! Welcome to my OpenGL project! This project features a set of new knowledge I have acquired after working with OpenGL and GLFW. I learned a lot of useful stuff, such as 3D architexture, the VBO/VAO/EBO system aswell as shaders, shader programs, and more!
This project also includes `ImGUI`, where you can create different objects such as rectangles, cubes and circles aswell as triangles! There is a customizable camera system, where you can tweak it inside the ImGUI menu. It includes things such as FOV, speed, and such.

## Prerequisites ✅ 
- You need a [PC](https://en.wikipedia.org/wiki/Personal_computer).
- You need [git](https://git-scm.com/) installed.
- You need [CMake](https://cmake.org/) installed.
- A compatible C++ compiler for CMake, for this project I use the one provided by the [VSC tutorial](https://code.visualstudio.com/docs/cpp/config-mingw#_prerequisites).
- A [frontal lobe](https://en.wikipedia.org/wiki/Frontal_lobe).

## Building 🛠️
To build this, it is straight forward. First start by cloning the repo.

```command
git clone https://github.com/SameForYouWasTakenWasTaken/OpenGL-project/
cd OpenGL-Project
```
Once you have run these set of commands, you should be inside a folder called OpenGL-Project. Now, we have to build it. 

❗ If you downloaded the C++ Compiler linked by the tutorial above, then you probably have the GCC/Clang compiler. If you have a different compiler, please select a different build system, which will be mentioned below.

Let's start by making a simple build folder to store all of the messy files CMake will generate.
```command
mkdir build
cd build
```
Your CMD line should be looking like this or similar: `C:\Users\USER\...\...\OpenGL-Project\build>`

If everything worked, then we should start the building process. I use the GCC compiler for standard C++ compilation. If you are unsure of which build system you are using, the default should automatically be run. But I do it a different way for a little bit of safety.

If don't know which compiler you are using or don't know which build type to use, you could try: 
```command
cmake -S .. -B .
```
This executes whatever is default.

If you are using the same compiler I use, then you should do:
```command
cmake -G "MinGW Makefiles" -S .. -B .
```
Finally, we can finish the building process by doing:

```command
cmake --build .
```
^ Be aware, this *is* a tedious and long process, it's not your fault if it's slow. This project contains *a lot* of files that CMake has to build for the program to work.

❗If you encounter this or a similar error, even though you have the compiler:
```
CMake Error: CMake was unable to find a build program corresponding to "MinGW Makefiles".  CMAKE_MAKE_PROGRAM is not set.  You probably need to select a different build tool.
CMake Error: CMAKE_CXX_COMPILER not set, after EnableLanguage
-- Configuring incomplete, errors occurred!
```
❗Then your terminal might not have found the location of your compiler. Try running the compiler' commands, such ass `gcc -v`. If the terminal cannot find it or doesn't recognize `gcc`, then that is your issue. Another fix might be to change the build type, because you probably are using the default selection. (i.e, not using the -G flag)
