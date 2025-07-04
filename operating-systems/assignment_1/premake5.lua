workspace "assignment_1"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	outputdir = "%{cfg.buildcfg}"

project "server"
	kind "ConsoleApp"
	language "C"
	cdialect "Default"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"server.c",
		"kissdb/**.c"
	}

	includedirs
	{
		"kissdb",
	}

	links
	{
		"pthread"
	}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"
		optimize "off"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		symbols "on"
		optimize "on"

	filter "configurations:Dist"
		defines "DIST"
		runtime "Release"
		symbols "off"
		optimize "on"

project "client"
	kind "ConsoleApp"
	language "C"
	cdialect "Default"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"client.c"
	}

	links
	{
		"pthread"
	}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"
		optimize "off"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		symbols "on"
		optimize "on"

	filter "configurations:Dist"
		defines "DIST"
		runtime "Release"
		symbols "off"
		optimize "on"