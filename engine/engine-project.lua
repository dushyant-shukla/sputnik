------------------------------------------------------------- RENDERER LIBRARY PROJECT CONFIGURATION ------------------------------------------------------

project "engine"
kind "StaticLib"
language "C++"
characterset("MBCS")

-- targetdir("$(SolutionDir)_output/bin/" .. outputdir .. "/%{prj.name}")
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

  "%{externals.glm}/glm/**.hpp",
  "%{externals.glm}/glm/**.inl"
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
  "%{include_dir.glfw}",
  "%{include_dir.glm}",
  "%{externals.imgui}/backends/imgui_impl_glfw.h",
  "%{externals.imgui}/backends/imgui_impl_opengl3.h"
}

links
{
  "%{static_libs.opengl}",
  "ramanujan",
  "ImGui",
  "GLFW",
  "cgltf",
  "stb_image",
  "ImGuizmo"
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

filter "files:$(SolutionDir)engine/externals/stb_image/stb_image.cpp"
flags { "NoPCH" }

filter "files:$(SolutionDir)engine/externals/imgui/backends/imgui_impl_glfw.cpp"
flags { "NoPCH" }

filter "files:$(SolutionDir)engine/externals/imgui/backends/imgui_impl_opengl3.cpp"
flags { "NoPCH" }

filter "files:$(SolutionDir)engine/externals/imguizmo/ImGuizmo.cpp"
flags { "NoPCH" }

-- Do not expect pch for this file
filter "files:source/platform/win32/win32_window.cpp" -- $(SolutionDir)sputnik/source/main/windows/windows_main.cpp does not work
flags { "NoPCH" }

filter "files:source/graphics/glcore/utilitiy/gl_api_wrapper.cpp"
flags { "NoPCH" }
