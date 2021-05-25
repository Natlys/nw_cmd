--==<native_code>==--
workspace("native_code")
	--general--
	configurations{"work", "play"}
	characterset("ascii")
	floatingpoint("default")
	removeflags{"excludefrombuild"}
	--environment--
	cpp_comp_name = "F:\\dev\\gcc\\bin\\g++.exe"
	cxx_comp_name = "F:\\dev\\gcc\\bin\\gcc.exe"
	lua_comp_name = "F:\\dev\\lua\\bin\\luaj.exe"
	premake.tools.ncc = premake.tools.gcc
	premake.tools.ncc.getlinks = premake.tools.gcc.getlinks
	premake.tools.ncc.getcflags = premake.tools.gcc.getcflags
	premake.tools.ncc.getcxxflags = premake.tools.gcc.getcxxflags
	premake.tools.ncc.getcppflags = premake.tools.gcc.getcppflags
	premake.tools.ncc.getldflags = premake.tools.gcc.getldflags
	premake.tools.ncc.getdefines = premake.tools.gcc.getdefines
	premake.tools.ncc.getincludedirs = premake.tools.gcc.getincludedirs
	premake.tools.ncc.getlibrarydirectories = premake.tools.gcc.getlibrarydirectories
	premake.tools.ncc.getmakesettings = premake.tools.gcc.getmakesettings
	premake.tools.ncc.getcforceincludes = premake.tools.gcc.getcforceincludes
	premake.tools.ncc.gettoolname = function(cfg, tool)
		if (tool == "cc") then return cxx_comp_name end
		if (tool == "cxx") then return cpp_comp_name end
		if (tool == "ar") then return "ar" end
		return "default"
		end
	toolset("ncc")
	debugger("gdb")
	warnings("off")
	--operational_system--
	architecture ("x64")
	platforms("win64")
	--language--
	--language("c++")
	language("c")
	--filters--
	--==platform_filters==--
	filter("platforms:win64")
		system("windows")
	--==system_filters==--
	filter("system:windows")
		systemversion("latest")
	--==config_filters==--
	filter("configurations:work")
		symbols("on")
		floatingpoint("strict")
		runtime("debug")
	filter("configurations:play")
		optimize("full")
		floatingpoint("fast")
		runtime("release")
	--==language_filters==--
	filter("language:c++")
		--general--
		--entrypoint("entry")
		cppdialect("c++17")
		rtti("off")
		buildoptions{ "-fpermissive" }
		--source--
		files{"%{prj.name}/src_cpp/**.cpp", "%{prj.name}/src_cpp/**.hpp"}
		includedirs {"%{prj.name}/src_cpp/"}
		--binary--
		targetdir ("%{prj.name}/bin_cpp/")
		objdir ("%{prj.name}/bin_cpp/")
	filter("language:c")
		--standard--
		--entrypoint("entry")
		cdialect("c99")
		clr("off")
		--source--
		files{"%{prj.name}/src_cxx/**.cxx", "%{prj.name}/src_cxx/**.hxx"}
		includedirs {"%{prj.name}/src_cxx/"}
		--binary--
		targetdir ("%{prj.name}/bin_cxx/")
		objdir ("%{prj.name}/bin_cxx/")
	filter("language:c#")
		--standard--
		csversion("4.0.0")
		rtti("off")
	--==cxx_file_filters==--
	filter("files:**.cxx")
		compileas("c")
		pchheader("default")
		pchsource("default")
		flags{"nopch", "c99"}
	--==cpp_file_filters==--
	filter("files:**.cpp")
		compileas("c++")
		pchheader("nc_pch.hpp")
		pchsource("src_cpp/nc_pch.cpp")
		flags{"c++14"}
	--==csh_file_filters==--
	filter("files:**.csh")
	--==lua_file_filters==--
	filter("files:**.lua")
		buildinputs("%(files)")
		buildoutputs("%(cfg.objdir)/(file.basename).out")
		buildmessage("compiling %(file.relpath);")
		buildcommands("luac -o %(cfg.objdir)/%(file.basename).out \"%(file.relpath))\"")
		removeflags("excludefrombuild")
	--rules--
	--rules{"comp_cpp", "comp_cxx"}
	--rule("comp_cpp")
	--	fileextension(".cpp")
	--	buildmessage("compile %(filename) with "..cpp_comp_name)
	--	buildcommands("%(cpp_comp_name) -c %(fullpath) -o %(intdir)/%(filename).luac")
	--==<native_code_console>==--
	project("nc_cmd")
		--general--
		location("./")
		kind("staticlib")
		--libraries--
		links{ }
		libdirs{ }
		--filters--
		--==platform_filters==--
		filter("platforms:win64")
		--==system_filters==--
		filter("system:windows")
		--==language_filters==--
		filter("language:c++")
			--source--
			files{"src_cpp/**.cpp", "src_cpp/**.hpp"}
			includedirs {"src_cpp/"}
			--binary--
			targetdir ("bin_cpp/")
			objdir ("bin_cpp/")
		filter("language:c")
--end_of_file--