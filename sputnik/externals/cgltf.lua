project "cgltf"
	location "cgltf"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("cgltf/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("cgltf/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"cgltf/cgltf.h",
		"cgltf/cgltf.c"
	}

	filter { "files:**.c" }
		compileas "C++"
		flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		staticruntime "off"
		runtime "Debug"

	filter "configurations:Release"
		optimize "On"
		staticruntime "off"
		runtime "Release"