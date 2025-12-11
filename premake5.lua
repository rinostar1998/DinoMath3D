project "DinoMath3D"
    kind "StaticLib"
    language "C++"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "DinoMath3D"
    }
    
    filter "system:windows"
        systemversion "latest"
        defines { PLATFORM_WINDOWS }

    filter "system:macosx"
        systemversion "latest"
        defines { PLATFORM_MACOS }

    filter "system:linux"
        systemversion "latest"
        defines { PLATFORM_LINUX }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"