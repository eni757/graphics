project( "x-stb" )
	kind "StaticLib"

	location "."

	files( "stb/src/*.c" )
	files( "stb/include/*.h" )

	usage "PUBLIC"
		includedirs "stb/include"

	usage "INTERFACE"
		links "x-stb"

project ()

