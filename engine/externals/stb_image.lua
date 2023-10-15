project "stb_image"
	location "stb_image"
	kind "StaticLib"
	language "C++"
	staticruntime "on"

	targetdir ("stb_image/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("stb_image/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"stb_image/stb_image.h",
		"stb_image/stb_image.cpp"
	}

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