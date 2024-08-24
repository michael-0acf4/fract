-- premake5.lua
workspace "arithmetic"
   configurations { "Debug", "Release" }

project "fract"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"


    targetdir "%{wks.location}/bin/%{cfg.buildcfg}/%{prj.name}"
    objdir "%{wks.location}/obj/%{cfg.buildcfg}/%{prj.name}"

    files { "src/main.cpp" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"