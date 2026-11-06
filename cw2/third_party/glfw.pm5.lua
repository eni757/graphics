newoption {
	trigger = "no-way",
	description = "Disable support for Wayland. Use this if wayland-scanner is unavailable."
};

project "x-glfw-wayland"
	kind "Utility"

	location "."

	files {
		"glfw/deps/wayland/*.xml"
	}

	filter {"system:linux", "files:glfw/deps/wayland/**.xml", "not options:no-way"}
		buildmessage "Wayland: %{file.name}"
		buildoutputs {
			"%{cfg.objdir}/../wayland-protocol/%{file.basename}-client-protocol.h",
			"%{cfg.objdir}/../wayland-protocol/%{file.basename}-client-protocol-code.h"
		}
		buildcommands {
			'{MKDIR} %[%{cfg.objdir}/../wayland-protocol]',
			'wayland-scanner client-header %[%{file.abspath}] %[%{cfg.objdir}/../wayland-protocol/%{file.basename}-client-protocol.h]',
			'wayland-scanner private-code %[%{file.abspath}] %[%{cfg.objdir}/../wayland-protocol/%{file.basename}-client-protocol-code.h]'
		}

	--TODO: cleancommands with action:gmake

	usage "INTERFACE"
		filter "system:linux"
			includedirs "%{cfg.objdir}/../wayland-protocol"
		filter "*"

project "x-glfw"
	kind "StaticLib"

	location "."

	-- Configuration
	filter "system:linux"
		defines { "_GLFW_X11=1" }
	filter {"system:linux", "not options:no-way"}
		defines { "_GLFW_WAYLAND=1" }

	filter "system:windows"
		defines { "_GLFW_WIN32=1" }

	filter "system:macosx"
		-- completely untested.
		defines { "_GLFW_COCOA=1" }

	-- Suppress warnings in GLFW
	filter "toolset:msc-*"
		buildoptions {
			"/wd4100", -- unreferenced formal parameter
			"/wd4201", -- nonstandard extension: nameless struct/union
			"/wd4204", -- nonstandard extension: non-constant aggregate
			"/wd4244", -- conversion from X to Y, possible loss of data
			"/wd4706", -- assignment within condition expression
		}

	filter "*"


	files {
		"glfw/src/context.c",
		"glfw/src/egl_context.c",
		"glfw/src/init.c",
		"glfw/src/input.c",
		"glfw/src/internal.h",
		"glfw/src/mappings.h",
		"glfw/src/monitor.c",
		"glfw/src/null_init.c",
		"glfw/src/null_joystick.c",
		"glfw/src/null_joystick.h",
		"glfw/src/null_monitor.c",
		"glfw/src/null_platform.h",
		"glfw/src/null_window.c",
		"glfw/src/platform.c",
		"glfw/src/platform.h",
		"glfw/src/vulkan.c",
		"glfw/src/window.c",
		"glfw/src/osmesa_context.c"
	};

	filter "system:linux"
		files {
			"glfw/src/posix_*",
			"glfw/src/x11_*", 
			"glfw/src/xkb_*",
			"glfw/src/glx_*",
			"glfw/src/linux_*"
		};
	filter {"system:linux", "not options:no-way"}
		files {
			"glfw/src/wl_*",
		};
	filter "system:windows"
		files {
			"glfw/src/win32_*",
			"glfw/src/wgl_*", 
		};
	filter "system:macosx"
		-- WARNING: this is completely untested!
		files {
			"glfw/src/cocoa_*",
			"glfw/src/nsgl_context.m",
			"glfw/src/posix_thread.*",
			"glfw/src/posix_module.*",
			"glfw/src/macos_time.*"
		};
	
	filter "*"

	uses "x-glfw-wayland"

	usage "PUBLIC"
		includedirs "glfw/include"

	usage "INTERFACE"
		links "x-glfw"

		-- Additional dependencies required by GLFW on MacOS.
		filter "system:macosx"
			links {
				"Cocoa.framework", 
				"OpenGL.framework", 
				"IOKit.framework", 
				"CoreVideo.framework" ,
				"QuartzCore.framework"
			};
		filter "*"

project ()
