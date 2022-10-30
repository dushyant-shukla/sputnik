------------------------------------------------------------- RENDERER LIBRARY PROJECT CONFIGURATION ------------------------------------------------------

project "sputnik"
kind "StaticLib"
language "C++"
characterset("MBCS")

targetdir("$(SolutionDir)_output/bin/" .. outputdir .. "/%{prj.name}")
objdir("$(SolutionDir)_output/bin-intermediate/" .. outputdir .. "/%{prj.name}")

pchheader "pch.h"
pchsource "source/pch.cpp"

files
{
  "source/**.h",
  "source/**.cpp",
  "source/**.inl",

  -- external source files
  "%{externals.glad}/src/glad.c",   -- "%{externals.glad}/src/**.c", -- (**.c) does not work for some reason
  "%{externals.stb_image}/stb_image.h",
  "%{externals.stb_image}/stb_image.cpp",
  "%{externals.cgltf}/cgltf.h",
  "%{externals.cgltf}/cgltf.c",
  "%{externals.imgui}/backends/imgui_impl_glfw.h",
  "%{externals.imgui}/backends/imgui_impl_glfw.cpp",
  "%{externals.imgui}/backends/imgui_impl_opengl3.h",
  "%{externals.imgui}/backends/imgui_impl_opengl3.cpp",
  "%{externals.imguizmo}/ImGuizmo.h",
  "%{externals.imguizmo}/ImGuizmo.cpp"
}

includedirs
{
  "source"
}

externalincludedirs
{
  "%{include_dir.glad}",
  "%{include_dir.stb_image}",
  "%{include_dir.cgltf}",
  "%{include_dir.ramanujan}",
  "%{include_dir.imgui}",
  "%{include_dir.spdlog}",
  "%{include_dir.imguizmo}",
  "%{include_dir.glfw}"
}

links
{
  "%{static_libs.opengl}",
  "ramanujan",
  "ImGui",
  "GLFW"
}

filter "system:windows"
defines
{
  "GLFW_INCLUDE_NONE"
}

filter { "files:**.c" }
compileas "C++"
flags { "NoPCH" }

-- Do not expect pch for this file
-- filter "files:%{externals.glad}/src/glad.c" -- this does not work
-- filter "files:**.c"
-- flags { "NoPCH" }

filter "files:$(SolutionDir)sputnik/externals/stb_image/stb_image.cpp"
flags { "NoPCH" }

filter "files:$(SolutionDir)sputnik/externals/imgui/backends/imgui_impl_glfw.cpp"
flags { "NoPCH" }

filter "files:$(SolutionDir)sputnik/externals/imgui/backends/imgui_impl_opengl3.cpp"
flags { "NoPCH" }

filter "files:$(SolutionDir)sputnik/externals/ImGuizmo/ImGuizmo.cpp"
flags { "NoPCH" }

-- Do not expect pch for this file
filter "files:source/platform/win32/win32_window.cpp" -- $(SolutionDir)sputnik/source/main/windows/windows_main.cpp does not work
flags { "NoPCH" }

filter "files:source/graphics/glcore/utilitiy/gl_api_wrapper.cpp"
flags { "NoPCH" }
