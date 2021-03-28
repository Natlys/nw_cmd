--==<console_project>==--
project("nw_cmd")
	kind("staticlib")
	staticruntime("on")
	language("c++")
	cppdialect("c++17")
	targetdir(dir_out_res)
	objdir(dir_out_int)
	pchheader("nw_cmd_pch.hpp")
	pchsource("src_cpp/nw_cmd_pch.cpp")
	files{ "%{dir_cpp.own}**.c**", "%{dir_cpp.own}**.h**" }
	includedirs{ "%{dir_cpp.own}", "%{dir_cpp.nw_lib}" }
	libdirs{ "%{dir_lib.nw_lib}" }
	links{ "nw_lib" }
--==</console_project>==--