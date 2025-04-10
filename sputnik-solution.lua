solution "sputnik"
architecture "x64"

configurations
{
  "Debug",
  "Release"
}

-- outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
targetdir(outputdir)


externals               = {}
externals["glad"]       = "$(SolutionDir)engine/externals/glad"
externals["stb_image"]  = "$(SolutionDir)engine/externals/stb_image"
externals["cgltf"]      = "$(SolutionDir)engine/externals/cgltf"
externals["ramanujan"]  = "$(SolutionDir)engine/externals/ramanujan"
externals["imgui"]      = "$(SolutionDir)engine/externals/imgui"
externals["spdlog"]     = "$(SolutionDir)engine/externals/spdlog"
externals["imguizmo"]   = "$(SolutionDir)engine/externals/imguizmo"
externals["glfw"]       = "$(SolutionDir)engine/externals/glfw"
externals["glm"]       = "$(SolutionDir)engine/externals/glm"

include_dir              = {}
include_dir["glad"]      = "%{externals.glad}/include"
include_dir["stb_image"] = "%{externals.stb_image}"
include_dir["cgltf"]     = "%{externals.cgltf}"
include_dir["ramanujan"] = "%{externals.ramanujan}/source"
include_dir["imgui"]     = "%{externals.imgui}"
include_dir["spdlog"]    = "%{externals.spdlog}/include"
include_dir["imguizmo"]  = "%{externals.imguizmo}"
include_dir["glfw"]      = "%{externals.glfw}/include"
include_dir["glm"]       = "%{externals.glm}"

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
-- include "runtime/runtime.lua"
-- include "editor/editor.lua"

-- include the demo projects
-- include "demos/curve-plotting/curve-plotting.lua"
-- include "demos/animation-poses/animation-poses.lua"

group "demos-animation"
include "demos/vertex-skinning/vertex-skinning.lua"
include "demos/ik-ccd-solver-basic/ik-ccd-solver-basic.lua"
include "demos/ik-fabrik-solver-basic/ik-fabrik-solver-basic.lua"
group ""

group "demos-graphics"
-- include "demos/sandbox/sandbox.lua"
include "demos/graphics-sandbox/graphics-sandbox.lua"
include "demos/compute-image/compute-image.lua"
include "demos/compute-particles/compute-particles.lua"
include "demos/compute-cloth/compute-cloth.lua"
group ""


group "demos-physics"
-- include "demos/physics-basic-particles/physics-basic-particles.lua"
include "demos/physics-basic-mass-spring/physics-basic-mass-spring.lua"
include "demos/physics-rope-bridge/physics-rope-bridge.lua"
include "demos/physics-mass-spring-cube/physics-mass-spring-cube.lua"
include "demos/physics-mass-aggregate-rope/physics-mass-aggregate-rope.lua"
include "demos/physics-mass-aggregate-cloth/physics-mass-aggregate-cloth.lua"
include "demos/physics-mass-aggregate-cube/physics-mass-aggregate-cube.lua"
include "demos/physics-sandbox/physics-sandbox.lua"
-- include "demos/physics-mass-spring-rope/physics-mass-spring-rope.lua"
-- include "demos/physics-mass-aggregate-rope/physics-mass-aggregate-rope.lua"
group ""

