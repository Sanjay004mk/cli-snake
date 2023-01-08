workspace "cli_snake"
    architecture "x64"
    startproject "cli_snake"
    configurations { "Debug", "Release" }

    project "cli_snake"
        location "%{wks.location}"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"

        targetdir ("%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}/")
        objdir ("%{wks.location}/bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}/")

        files { "Source.cpp" }

        staticruntime "off"
        systemversion "latest"

        filter "configurations:Debug"
            symbols "on"
            runtime "Debug"

        filter "configurations:Release"
            optimize "on"
            runtime "Release"