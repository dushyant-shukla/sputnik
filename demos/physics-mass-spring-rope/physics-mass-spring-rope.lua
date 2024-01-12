------------------------------------------------------------- PROJECT BASIC-SCENE CONFIGURATION ------------------------------------------------------

project "physics-mass-spring-rope"
kind "WindowedApp"
language "C++"
characterset("MBCS")

-- targetdir("$(SolutionDir)_output/bin/" .. outputdir .. "/%{prj.name}")
objdir("$(SolutionDir)_output/bin-intermediate/" .. outputdir .. "/%{prj.name}")

files
{
  "source/**.h",
  "source/**.cpp"
}

-- includedirs
-- {
--   "$(SolutionDir)sputnik/source",
--   "%{include_dir.ramanujan}",


--   "%{include_dir.glad}",
--   "%{include_dir.stb_image}",
--   "%{include_dir.cgltf}",
-- }

externalincludedirs
{
  "$(SolutionDir)engine/source",
  "%{include_dir.ramanujan}",

  -- these imcludes should not be required here as we write a better abstraction over lower level libraries
  "%{include_dir.glad}",
  "%{include_dir.stb_image}",
  "%{include_dir.cgltf}",
  "%{include_dir.imgui}",
  "%{include_dir.spdlog}",
  "%{include_dir.imguizmo}"
}

links
{
  "engine"
}

-- In debug mode, the application should link to the console subsystem.
-- To make logging a bit easier, two windows will be open at the same time in debug mode.
-- One will be the standard Win32 window, and the other will be a console window for viewing logs.
-- filter "configurations:Debug"
-- kind "ConsoleApp"

-- In release mode, it should link to the window subsystem.
-- filter "configurations:Release"
-- kind "WindowedApp"