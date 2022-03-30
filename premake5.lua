workspace "Run"
    configurations {"Debug", "Release"}

    VULKAN_SDK = os.getenv("VULKAN_SDK")
    ENGINE_NAME = "RunEngine"

    if VULKAN_SDK == nil then
        error "The vulkan sdk path is not set!"
    end

    newaction {
        trigger = "clean",
        description = "Clean the binaries",
        execute = function ()
           print "Cleaning the build..."
           os.remove ("./bin/" .. ENGINE_NAME.. "/Debug/" .. ENGINE_NAME .. ".exe")
           os.remove ("./bin/" .. ENGINE_NAME.. "/Debug/" .. ENGINE_NAME .. ".pdb")

           os.remove ("./bin/" .. ENGINE_NAME.. "/Release/" .. ENGINE_NAME .. ".exe")
           os.remove ("./bin/" .. ENGINE_NAME.. "/Release/" .. ENGINE_NAME .. ".pdb")
           os.rmdir "./bin-int"
           print "Done!"
        end
    }

    project (ENGINE_NAME)
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17";
        location "src"


        files {"**.h", "**.cpp"}
        includedirs {
            "include/GLFW/include", 
            "include/ILog", 
            "src", 
            VULKAN_SDK .. "/Include",
            "src/engine/utils",
            "src/engine/core",
            "src/engine/input",
            "src/engine/math"
        }

        defines {"_CRT_SECURE_NO_WARNINGS", "GLFW_INCLUDE_VULKAN"}

        libdirs {"include/Glfw/", VULKAN_SDK .. "/Lib"}
        links {"glfw3dll", "vulkan-1"}

        architecture "x86_64"

        filter "configurations:Debug"
            defines { "DEBUG" }
            targetdir ("bin/" .. ENGINE_NAME .. "/Debug")
            objdir ("bin-int/"  .. ENGINE_NAME .. "/Debug") 

        filter "configurations:Release"
            defines { "NDEBUG" }
            targetdir ("bin/"  .. ENGINE_NAME .. "/Release")
            objdir ("bin-int/"  .. ENGINE_NAME .. "/Release") 
