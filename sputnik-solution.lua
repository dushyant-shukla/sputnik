solution "sputnik"
architecture "x64"

configurations
{
  "Debug",
  "Release"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

externals               = {}
externals["glad"]       = "$(SolutionDir)sputnik/externals/glad"
externals["stb_image"]  = "$(SolutionDir)sputnik/externals/stb_image"
externals["cgltf"]      = "$(SolutionDir)sputnik/externals/cgltf"
externals["ramanujan"]  = "$(SolutionDir)sputnik/externals/ramanujan"
externals["imgui"]      = "$(SolutionDir)sputnik/externals/imgui"
externals["spdlog"]     = "$(SolutionDir)sputnik/externals/spdlog"
externals["imguizmo"]   = "$(SolutionDir)sputnik/externals/ImGuizmo"
externals["glfw"]       = "$(SolutionDir)sputnik/externals/glfw"

include_dir              = {}
include_dir["glad"]      = "%{externals.glad}/include"
include_dir["stb_image"] = "%{externals.stb_image}"
include_dir["cgltf"]     = "%{externals.cgltf}"
include_dir["ramanujan"] = "%{externals.ramanujan}/source"
include_dir["imgui"]     = "%{externals.imgui}"
include_dir["spdlog"]    = "%{externals.spdlog}/include"
include_dir["imguizmo"]  = "%{externals.imguizmo}"
include_dir["glfw"]      = "%{externals.glfw}/include"

static_libs           = {}
static_libs["opengl"] = "opengl32.lib"

externalwarnings "Off"
externalanglebrackets "On"

filter "system:windows"
cppdialect "C++20"
systemversion "latest"

filter "configurations:Debug"
symbols "On"
staticruntime "off"
runtime "Debug"
defines
{
  "SPUTNIK_DEBUG"
}

filter "configurations:Release"
optimize "On"
staticruntime "off"
runtime "Release"

-- include the dependencies
group "dependencies"
include "sputnik/externals/ramanujan/ramanujan-project.lua"
include "sputnik/externals/imgui"
include "sputnik/externals/glfw.lua"
group ""

-- include the renderer project
include "sputnik/sputnik-project.lua"

-- include the demo projects
group "demos"
include "demos/sandbox/sandbox.lua"
include "demos/curve-plotting/curve-plotting.lua"
include "demos/animation-poses/animation-poses.lua"
include "demos/vertex-skinning/vertex-skinning.lua"
group ""
