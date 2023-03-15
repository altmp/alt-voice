set_xmakever("2.7.2")
includes("packages/**.lua")

set_project("alt-voice")

set_arch("x64")
set_languages("cxx20", "cxx2a")

set_optimize("none")
add_cxxflags("/MP /GR- /EHsc")

if is_mode("debug") then
    set_symbols("debug")
    add_cxxflags("/Zi /Ob0 /Od")
elseif is_mode("release") then
    set_symbols("hidden")
    set_strip("all")
    add_cxxflags("/GL /Zi /O2")
end

if is_mode("debug") then
    set_runtimes("MDd")
else
    set_runtimes("MD")
end

add_requires("bass", "bassmix", "libopus")

target("alt-voice")
    set_default(true)
    set_kind("shared")
    set_filename("alt-voice.dll")
    add_files("src/**.cpp")
    add_headerfiles("src/**.h", "include/**.h")
    add_includedirs("src/", "include/", { public = true })
    add_packages("bass", "bassmix", "libopus")
    add_defines("ALT_VOICE_LIB")
    after_build(function (target)
        for pkg, pkg_details in pairs(target:pkgs()) do
            for i, dir in ipairs(pkg_details._INFO.libfiles) do
                os.cp(dir, path.join(target:targetdir(), dir:match("([^/\\]+)$")))
            end
        end
    end)

target("devicetests")
    set_default(false)
    set_kind("binary")
    set_group("examples")
    add_files("examples/devicetests.cpp")
    add_packages("bass", "bassmix", "libopus")
    add_deps("alt-voice")

add_rules("plugin.vsxmake.autoupdate")