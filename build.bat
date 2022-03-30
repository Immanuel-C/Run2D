@echo off

set cppFiles=src/*.cpp src/engine/core/*.cpp include/ILog/*.cpp
set hFiles=-Isrc -Isrc/engine -Isrc/engine/utils -Isrc/engine/input -Isrc/engine/core -I%VULKAN_SDK%/Include -Iinclude/ILog -Iinclude/Glfw/include
set libFiles=%VULKAN_SDK%/Lib/vulkan-1.lib include/GLFW/glfw3dll.lib
set defines=-D_CRT_SECURE_NO_WARNINGS -DGLFW_INCLUDE_VULKAN

call clang++ -std=c++17 %defines% %cppFiles% %hFiles% %libFiles% -o RunVk.exe