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

  "%{externals.glad}/src/glad.c",
  -- "%{externals.glad}/src/**.c", -- (**.c) does not work for some reason
  "%{externals.stb_image}/stb_image.h",
  "%{externals.stb_image}/stb_image.cpp",
  "%{externals.cgltf}/cgltf.h",
  "%{externals.cgltf}/cgltf.c"
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
  "%{include_dir.ramanujan}"
}

links
{
  "%{static_libs.opengl}",
  "ramanujan"
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

-- Do not expect pch for this file
filter "files:source/main/windows/windows_main.cpp" -- $(SolutionDir)sputnik/source/main/windows/windows_main.cpp does not work
flags { "NoPCH" }
