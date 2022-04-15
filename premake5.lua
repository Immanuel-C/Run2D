WORKSPACE_NAME = "Run"

workspace (WORKSPACE_NAME)
    configurations {"Debug", "Release"}

    VULKAN_SDK = os.getenv("VULKAN_SDK")
    ENGINE_NAME = "RunEngine"
    APPLICATION_NAME = "Application"
    GLFW_NAME = "glfw"

    startproject (APPLICATION_NAME)

    if VULKAN_SDK == nil then
        error "The vulkan sdk path is not set!"
    end

    architecture "x86_64"

    flags {
		"MultiProcessorCompile"
	}

    newaction {
        trigger = "clean",
        description = "Clean the binaries",
        execute = function ()
           print "Cleaning the build..."
           os.remove ("./bin/" .. ENGINE_NAME.. "/Debug/" .. ENGINE_NAME .. ".lib")
           os.remove ("./bin/" .. ENGINE_NAME.. "/Debug/" .. ENGINE_NAME .. ".dll")
           os.remove ("./bin/" .. ENGINE_NAME.. "/Debug/" .. ENGINE_NAME .. ".pdb")

           os.remove ("./bin/" .. ENGINE_NAME.. "/Release/" .. ENGINE_NAME .. ".lib")
           os.remove ("./bin/" .. ENGINE_NAME.. "/Release/" .. ENGINE_NAME .. ".dll")
           os.remove ("./bin/" .. ENGINE_NAME.. "/Release/" .. ENGINE_NAME .. ".pdb")

           os.remove ("./bin/" .. APPLICATION_NAME.. "/Debug/" .. APPLICATION_NAME .. ".exe")
           os.remove ("./bin/" .. APPLICATION_NAME.. "/Debug/" .. APPLICATION_NAME .. ".pdb")

           os.remove ("./bin/" .. APPLICATION_NAME.. "/Release/" .. APPLICATION_NAME .. ".exe")
           os.remove ("./bin/" .. APPLICATION_NAME.. "/Release/" .. APPLICATION_NAME .. ".pdb")

           os.remove ("./bin/" .. GLFW_NAME.. "/Debug/" .. GLFW_NAME .. ".lib")
           os.remove ("./bin/" .. GLFW_NAME.. "/Debug/" .. GLFW_NAME .. ".dll")
           os.remove ("./bin/" .. GLFW_NAME.. "/Debug/" .. GLFW_NAME .. ".pdb")

           os.remove ("./bin/" .. GLFW_NAME.. "/Release/" .. GLFW_NAME .. ".lib")
           os.remove ("./bin/" .. GLFW_NAME.. "/Release/" .. GLFW_NAME .. ".dll")
           os.remove ("./bin/" .. GLFW_NAME.. "/Release/" .. GLFW_NAME .. ".pdb")


           os.rmdir "./bin-int"
           print "Done!"
        end
    }

    newaction {
        trigger = "cleanproj",
        description = "clean the project files (e.g. MakeFile)",
        execute = function() 
            print "Cleaning project files..."

            -- Visual Studio
            os.remove ("src/" .. ENGINE_NAME .. ".vcxproj")
            os.remove ("src/" .. ENGINE_NAME .. ".vcxproj.filters")
            os.remove ("src/" .. ENGINE_NAME .. ".vcxproj.user")
            os.remove ("application/" .. APPLICATION_NAME .. ".vcxproj")
            os.remove ("application/" .. APPLICATION_NAME .. ".vcxproj.filters")
            os.remove ("application/" .. APPLICATION_NAME .. ".vcxproj.user")
            os.remove ("GLFW/" .. GLFW_NAME .. ".vcxproj")
            os.remove ("GLFW/" .. GLFW_NAME .. ".vcxproj.filters")
            os.remove ("GLFW/" .. GLFW_NAME .. ".vcxproj.user")
            os.remove (WORKSPACE_NAME .. ".sln")

            -- Make
            os.remove "src/MakeFile"
            os.remove "MakeFile"
            os.remove "application/MakeFile"
            os.remove "GLFW/MakeFile"

            print "Done!"
        end
    }

    function defaultBuildCfg()
        filter "configurations:Debug"
            defines { "DEBUG" }
            symbols "On"
            runtime "Debug"
		    optimize "Debug"

        filter "configurations:Release"
            defines { "NDEBUG" }
            symbols "Off"
            runtime "Release"
		    optimize "Speed"
    end

    -- Dynamic Library
    project (ENGINE_NAME)
        kind "SharedLib"
        language "C++"
        cppdialect "C++17";
        location "src"


        files {"src/**.h", "src/**.cpp", "include/**.cpp"}
        includedirs {
            "include/GLFW/include", 
            "include/ILog", 
            "include/glm", 
            "src", 
            VULKAN_SDK .. "/Include",
            "src/engine/utils",
            "src/engine/core",
            "src/engine/input",
            "src/engine/math",
        }

        defines {"GLFW_INCLUDE_VULKAN", "RN_EXPORT_DLL"}

        libdirs {VULKAN_SDK .. "/Lib"}
        links {"vulkan-1", GLFW_NAME}

        
        architecture "x86_64"
        
       -- postbuildcommands { "copy ../bin/" .. ENGINE_NAME .. "/%{cfg.buildcfg}/" .. ENGINE_NAME .. ".dll ../bin/" .. APPLICATION_NAME .. "/%{cfg.buildcfg}/"} 

        targetdir ("bin/" .. ENGINE_NAME .. "/%{cfg.buildcfg}")
        objdir ("bin-int/"  .. ENGINE_NAME .. "/%{cfg.buildcfg}")

        defaultBuildCfg()

        
        
    project (APPLICATION_NAME)
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17";
        location "application"

        files {"application/**.h", "application/**.cpp"}
        includedirs {
            "include/GLFW/include", 
            "include/ILog", 
            "include/glm", 
            "src", 
            VULKAN_SDK .. "/Include",
            "src/engine/utils",
            "src/engine/core",
            "src/engine/input",
            "src/engine/math",
        }

        architecture "x86_64"
        libdirs {VULKAN_SDK .. "/Lib"}
        links {ENGINE_NAME, GLFW_NAME, "vulkan-1"}
        
        targetdir ("bin/" .. APPLICATION_NAME .. "/%{cfg.buildcfg}")
        objdir ("bin-int/"  .. APPLICATION_NAME .. "/%{cfg.buildcfg}")

        defaultBuildCfg()

    project (GLFW_NAME)
        kind "StaticLib"
        language "C"
        cdialect "C99"
        location "GLFW"

        targetdir ("bin/" .. GLFW_NAME .. "/%{cfg.buildcfg}")
        objdir ("bin-int/"  .. GLFW_NAME .. "/%{cfg.buildcfg}")

        files {
            "%{prj.location}/src/context.c",
            "%{prj.location}/src/egl_context.c",
            "%{prj.location}/src/egl_context.h",
            "%{prj.location}/src/init.c",
            "%{prj.location}/src/input.c",
            "%{prj.location}/src/internal.h",
            "%{prj.location}/src/monitor.c",
            "%{prj.location}/src/osmesa_context.c",
            "%{prj.location}/src/osmesa_context.h",
            "%{prj.location}/src/vulkan.c",
            "%{prj.location}/src/window.c"
        }
    
        filter "system:windows"
            files {
                "%{prj.location}/src/wgl_context.c",
                "%{prj.location}/src/wgl_context.h",
                "%{prj.location}/src/win32_init.c",
                "%{prj.location}/src/win32_joystick.c",
                "%{prj.location}/src/win32_joystick.h",
                "%{prj.location}/src/win32_monitor.c",
                "%{prj.location}/src/win32_platform.h",
                "%{prj.location}/src/win32_thread.c",
                "%{prj.location}/src/win32_time.c",
                "%{prj.location}/src/win32_window.c"
            }
    
            defines  { 
                "_GLFW_WIN32",
                "_CRT_SECURE_NO_WARNINGS"
            }

            links  {
                "User32",
                "Gdi32",
                "Shell32"
            }
    
        filter "system:linux"
            files
            {
                "%{prj.location}/src/glx_context.c",
                "%{prj.location}/src/glx_context.h",
                "%{prj.location}/src/linux_joystick.c",
                "%{prj.location}/src/linux_joystick.h",
                "%{prj.location}/src/posix_time.c",
                "%{prj.location}/src/posix_time.h",
                "%{prj.location}/src/posix_thread.c",
                "%{prj.location}/src/posix_thread.h",
                "%{prj.location}/src/x11_init.c",
                "%{prj.location}/src/x11_monitor.c",
                "%{prj.location}/src/x11_platform.h",
                "%{prj.location}/src/x11_window.c",
                "%{prj.location}/src/xkb_unicode.c",
                "%{prj.location}/src/xkb_unicode.h"
            }

            links {
                "dl",
                "pthread",
                "X11"
            }
    
            defines  { 
                "_GLFW_X11"
            }

            defaultBuildCfg()