project "ImGuizmo"
	location "ImGuizmo"
	kind "StaticLib"
	language "C++"
	staticruntime "on"

	targetdir ("ImGuizmo/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("ImGuizmo/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"ImGuizmo/GraphEditor.cpp",
		"ImGuizmo/GraphEditor.h",
		"ImGuizmo/ImCurveEdit.cpp",
		"ImGuizmo/ImCurveEdit.h",
		"ImGuizmo/ImGradient.cpp",
		"ImGuizmo/ImGradient.h",
		"ImGuizmo/ImGuizmo.cpp",
		"ImGuizmo/ImGuizmo.h",
		"ImGuizmo/ImSequencer.cpp",
		"ImGuizmo/ImSequencer.h",
		"ImGuizmo/ImZoomSlider.h",
	}

	includedirs
	{
		-- "ImGuizmo/example"
		"$(SolutionDir)engine/externals/imgui"
	}

	warnings "Off"

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