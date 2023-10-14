workspace "Tennessine"
    configurations { "Debug", "Release" }

project "Tennessine"
    architecture "x64"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/"

    files { "third-party/glm/glm/glm/**.cpp", "third-party/dear_imgui/misc/cpp/*.cpp", "third-party/dear_imgui/*.cpp", "third-party/dear_imgui/backends/imgui_impl_opengl3.cpp", "third-party/dear_imgui/backends/imgui_impl_glfw.cpp", "third-party/glad/src/glad.c", "src/**.cpp", "src/**.h"}

    includedirs
    {
        "third-party/glfw/3.3.8/include",
        "third-party/glad/include",
        "third-party/glm/glm",
        "third-party/dear_imgui",
        "third-party/assimp/include",
        "third-party/dear_imgui/backends",
        "third-party/dear_imgui/misc/cpp",
        "src/**"
    }
    libdirs
    {
        "third-party/glfw/3.3.8/lib-vc2022",
        "third-party/assimp/bin/Release"
    }
    links
    {
        "glfw3",
        "assimp-vc143-mt.dll"
    }

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"

project "Tennessine.Editor"
    architecture "x64"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}"

        files { "editor/src/**.cpp", "editor/src/**.h" }

    includedirs
    {
        "editor/src/**",
        "src/**",
        "third-party/glfw/3.3.8/include",
        "third-party/glad/include",
        "third-party/glm/glm",
        "third-party/dear_imgui",
        "third-party/assimp/include",
        "third-party/dear_imgui/backends",
        "third-party/dear_imgui/misc/cpp",
    }

    libdirs
    {
        "/bin/"
    }

    links
    {
        "Tennessine"
    }

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"