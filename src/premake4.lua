#!lua

-- Additional Linux libs: "X11", "Xxf86vm", "Xi", "Xrandr", "stdc++"

includeDirList = {
    "../shared",
    "../shared/include",
    "../shared/gl3w",
    "../shared/glew-1.13.0",
    "../shared/imgui",
    "../shared/lodepng"
}

libDirectories = { 
    "../lib"
}


if os.get() == "macosx" then
    linkLibs = {
        "cs488-framework",
        "imgui",
        "glfw3",
        "lua",
	    "glew"
    }
end

if os.get() == "linux" then
    linkLibs = {
            "cs488-framework",
            "imgui",
            "glew",
            "glfw3",
            "lua",
            "GL",
            "Xinerama",
            "Xcursor",
            "Xxf86vm",
            "Xi",
            "Xrandr",
            "X11",
            "stdc++",            
            "pthread",
            "SDL2",
            "SDL2_mixer",
            "lodepng",
            "dl"
    }
end

-- Build Options:
if os.get() == "macosx" then
    linkOptionList = { "-framework IOKit", "-framework Cocoa", "-framework CoreVideo", "-framework OpenGL" }
end

buildOptions = {"-std=c++11 -w"}

solution "Blockycraft"
    configurations { "Debug", "Release" }

    project "Blockycraft"
        kind "ConsoleApp"
        language "C++"
        location "build"
        objdir "build"
        targetdir "."
        buildoptions (buildOptions)
        libdirs (libDirectories)
        links (linkLibs)
        linkoptions (linkOptionList)
        includedirs (includeDirList)
        files { "*.cpp" }

    configuration "Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    configuration "Release"
        defines { "NDEBUG" }
        flags { "Optimize" }
