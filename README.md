# Run2D

A simple 2D Vulkan game engine.

# How to Build

The Run Game Engine uses premake5 to build. If you dont have premake5 installed go to this [link](https://premake.github.io/).

To clean the bin and bin-int folders call ```premake5 clean```. This wont remove generated project files (e.g. .sln files).

The repository provides .lib files and .dll files for its libraries, not .a or .so files. So if your on linux then you will have to get those.

# Libraries

The Run Engine uses [GLFW](https://www.glfw.org/) for windowing. Make sure to include glfw3.dll in your build folder, [ILog](https://github.com/Immanuel-C/ILog) ([ILog Docs](https://immanuel-c.github.io/ILog/frontPage.html)) for loging and GLM for math
