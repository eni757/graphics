workspace "COMP3812-cw2"
	language "C++"
	cppdialect "C++23"

	platforms { "x64" }
	configurations { "debug", "release" }

	startproject "main"

	enablepch "Off"
	multiprocessorcompile "On"
	visibility "Hidden"
	vectorextensions "AVX2"

	debugdir "%{wks.location}"
	objdir "_build_/%{cfg.buildcfg}-%{cfg.platform}-%{cfg.toolset}"
	targetsuffix "-%{cfg.buildcfg}-%{cfg.platform}-%{cfg.toolset}"

	-- Default toolset options
	filter "toolset:gcc or toolset:clang"
		linkoptions { "-pthread" }
		buildoptions { "-march=native", "-Wall", "-pthread" }

		-- Variable-length arrays (VLAs) are an extension that GCC and clang
		-- have long supported. However, they are not part of the C++ standard.
		-- (MSVC will not compile code with VLAs.)
		buildoptions { "-Werror=vla" }

	filter "toolset:gcc" 

	filter "toolset:msc-*"
		warnings "extra" -- this enables /W4; default is /W3
		--buildoptions { "/W4" }
		buildoptions { "/utf-8" }
		buildoptions { "/permissive-" }
		defines { "_CRT_SECURE_NO_WARNINGS=1" }
		defines { "_SCL_SECURE_NO_WARNINGS=1" }
	
	filter "*"

	-- default libraries
	filter "system:linux"
		links { "dl" }
		links { "stdc++exp" }
	
	filter "system:windows"
		links { "OpenGL32", "gdi32", "Shlwapi" }

	filter "*"

	-- default outputs
	filter "kind:StaticLib"
		targetdir "lib/"

	filter "kind:ConsoleApp"
		targetdir "bin/"
		targetextension ".exe"
	
	filter "*"

	--configurations
	filter "debug"
		symbols "On"
		defines { "_DEBUG=1" }

	filter "release"
		optimize "On"
		defines { "NDEBUG=1" }

	filter "*"

-- Third party code
include "third_party"

-- Projects
project "main"
	kind "ConsoleApp"

	location "main"
	files {
		"main/**.cpp",
		"main/**.hpp"
	}

	uses "support"
	uses "draw2d"
	uses "vmlib"

	uses "x-glad"
	uses "x-glfw"
	uses "x-stb"

project "draw2d"
	kind "StaticLib"

	location "."
	files {
		"draw2d/**.cpp",
		"draw2d/**.hpp"
	}

	uses "vmlib"
	--uses "vmlib"

	uses "x-stb"

	includedirs "support/include"

	usage "PUBLIC"
		includedirs "draw2d/include"

	usage "INTERFACE"
		links "draw2d"

project "support"
	kind "StaticLib"

	location "."
	files {
		"support/**.hpp",
		"support/error.cpp",
		"support/checkpoint.cpp",
		"support/runconfig.cpp"
	}

	filter "system:not macosx"
		files "support/context.cpp"
	filter "system:macosx"
		files "support/context-oldgl.cpp"
	filter "*"

	uses "draw2d"

	uses "x-glad"
	uses "x-glfw"

	usage "PUBLIC"
		includedirs "support/include"

	usage "INTERFACE"
		links "support"

project "vmlib"
	kind "StaticLib"

	location "."
	files {
		"vmlib/**.cpp",
		"vmlib/**.hpp"
	}

	usage "PUBLIC"
		includedirs "vmlib/include"

	usage "INTERFACE"
		links "vmlib"


project "bench"
	kind "ConsoleApp"

	location "bench"
	files {
		"bench/*.cpp"
	}

	uses "vmlib"
	uses "draw2d"
	uses "support"

	uses "x-nanobench"
	uses "x-stb"

project "triangles-sandbox"
	kind "ConsoleApp"

	location "triangles-sandbox"
	files {
		"triangles-sandbox/**.cpp",
		"triangles-sandbox/**.hpp"
	}

	uses "support"
	uses "draw2d"
	uses "vmlib"

	uses "x-glad"
	uses "x-glfw"
	uses "x-stb"

project "sampler-sandbox"
	kind "ConsoleApp"

	location "sampler-sandbox"
	files {
		"sampler-sandbox/**.cpp",
		"sampler-sandbox/**.hpp"
	}

	uses "support"
	uses "draw2d"
	uses "vmlib"

	uses "x-glad"
	uses "x-glfw"
	uses "x-stb"

--EOF
