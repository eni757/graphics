Third Party Software
====================

This code uses the following third party software/source code.

## Premake

- Where: https://premake.github.io/download
- What: Build configuration and generator
- License: BSD 3-Clause

Premake generates platform/toolchain specific build files, such as Visual
Studio project files or standard Makefiles. This project includes prebuilt
binaries for Linux, Windows and macOS.

## GLFW

- Where: https://www.glfw.org/
- What: Multi-platform lib for OpenGL & Vulkan development (desktop)
- License: Zlib/libpng

GLFW abstracts platform-specific functions, such as creating windows and
receiving/handling events. 

## GLAD

- Where: https://github.com/Dav1dde/glad  &  https://glad.dav1d.de/
- What: Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator.
- License: MIT, Apache-2

GLAD is an OpenGL loader generator. Only the generated loader is included. The
loader includes the core profile for OpenGL version up to 4.6, and the
following extensions:

- GL_ARB_bindless_texture
- GL_ARB_debug_output
- GL_ARB_gl_spirv
- GL_ARB_multi_draw_indirect
- GL_ARB_shader_clock
- GL_ARB_shader_ballot
- GL_ARB_shader_group_vote
- GL_ARB_texture_rgb10_a2ui
- GL_EXT_debug_label
- GL_EXT_debug_marker
- GL_EXT_EGL_image_storage
- GL_EXT_EGL_sync
- GL_EXT_memory_object
- GL_EXT_memory_object_fd
- GL_EXT_memory_object_win32
- GL_EXT_semaphore
- GL_EXT_semaphore_fd
- GL_EXT_semaphore_win32
- GL_EXT_texture_compression_{latc,rgtc,s3tc}
- GL_KHR_debug
- GL_KHR_shader_subgroup
- GL_KHR_texture_compression_{astc_hdr,astc_ldr}

## STB libraries

- Where: https://github.com/nothings/stb
- What: single-header libraries
- License: Public Domain / MIT License

Collection of single-header libraries by Sean Barrett. Included are:
- stb_image.h         (image loading)
- stb_image_write.h   (image writing)

Note: the files in src/ are custom.

## nanobench

- Where: https://nanobench.ankerl.com/ and https://github.com/martinus/nanobench
- What: "Simple, fast, accurate single-header microbenchmarking [...]"
- License: MIT

Nanobench is a small platform-independent library for microbenchmarking. It 
also has a funny cute logo.
