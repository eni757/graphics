project "x-glad"
	kind "StaticLib"

	location "."

	files { 
		"glad/src/glad.c" 
	}

	usage "PUBLIC"
		includedirs "glad/include"

	usage "INTERFACE"
		links "x-glad"

project ()
