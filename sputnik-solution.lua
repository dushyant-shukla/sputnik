solution "sputnik"
architecture "x64"

configurations
{
  "Debug",
  "Release"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

externals               = {}
externals["glad"]       = "$(SolutionDir)engine/externals/glad"
externals["stb_image"]  = "$(SolutionDir)engine/externals/stb_image"
externals["cgltf"]      = "$(SolutionDir)engine/externals/cgltf"
externals["ramanujan"]  = "$(SolutionDir)engine/externals/ramanujan"
externals["imgui"]      = "$(SolutionDir)engine/externals/imgui"
externals["spdlog"]     = "$(SolutionDir)engine/externals/spdlog"
externals["imguizmo"]   = "$(SolutionDir)engine/externals/imguizmo"
externals["glfw"]       = "$(SolutionDir)engine/externals/glfw"

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
include "engine/externals/ramanujan/ramanujan-project.lua"
include "engine/externals/imgui"
include "engine/externals/glfw.lua"
include "engine/externals/cgltf.lua"
include "engine/externals/stb_image.lua"
include "engine/externals/imguizmo.lua"
group ""

-- include the renderer project
include "engine/engine-project.lua"

-- include the demo projects
-- include "demos/sandbox/sandbox.lua"
-- include "demos/curve-plotting/curve-plotting.lua"
-- include "demos/animation-poses/animation-poses.lua"

group "animation-demos"
include "demos/vertex-skinning/vertex-skinning.lua"
include "demos/ik-ccd-solver/ik-ccd-solver.lua"
include "demos/fabrik-ik-solver-demo/fabrik-ik-solver-demo.lua"
group ""

group "physics-demos"
include "demos/physics/particles/basic/basic-particles.lua"
group ""

