--==<console_project>==--
project "nw_cmd"
	kind "staticlib" --.lib
	staticruntime "on"
	language "c++"
	cppdialect "c++17"
<<<<<<< HEAD
	targetdir (dir_out_res)
	objdir (dir_out_int)
=======
	targetdir ("bin/"..dir_out.."%{prj.name}")
	objdir ("bin/int/"..dir_out.."%{prj.name}")
>>>>>>> 81c478b764cd2559129d5a0decde748380435998
	pchheader "nwc_pch.hpp"
	pchsource "src_cpp/nwc_pch.cpp"
	files
	{
		"src_cpp/**.c**",
		"src_cpp/**.h**"
	}
	includedirs
	{
		"src_cpp/",
		"%{dir_cpp.nw_lib}"
	}
	libdirs
	{
		"%{dir_lib.nw_lib}"
	}
	links
	{
		"nw_lib"
	}
	filter "system:windows"
		systemversion "latest"
		defines { "CMD_PLATFORM_WINDOWS" }
	filter "configurations:debug"
		defines { "CMD_DEBUG" }
		symbols "on"
	filter "configurations:release"
		defines { "CMD_RELEASE" }
		optimize "on"
--==</console_project>==--