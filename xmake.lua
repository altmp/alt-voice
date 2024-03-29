set_xmakever("2.7.2")
includes("packages/**.lua")

set_project("alt-voice")

set_arch("x64")
set_languages("cxx20", "cxx2a")
set_runtimes(is_mode("debug") and "MDd" or "MD")
set_symbols("debug")

add_repositories("local-repo localpkg")

add_requires("bass", {verify = false})
add_requires("bass-fx", {verify = false})
add_requires("libopus", {verify = false, configs = { avxSupported = false, opusx86MayHaveAvx = false }})
add_requires("rnnoise f75e7dd", {verify = false})

target("alt-voice")
    set_default(false)
    set_kind("static")
    set_prefixname("")
    add_files("src/**.cpp")
    add_headerfiles("src/**.h", "include/**.h")
    add_includedirs("src/", "include/", { public = true })
    add_packages("bass", "bass-fx", "libopus", "rnnoise")
    add_rules("plugin.vsxmake.autoupdate")

    after_build(function (target)
        for pkg, pkg_details in pairs(target:pkgs()) do
            if os.isdir(pkg_details._INFO.installdir) then
                os.cp(pkg_details._INFO.installdir .. "/**.so", target:targetdir())
                os.cp(pkg_details._INFO.installdir .. "/**.dll", target:targetdir())
                os.cp(pkg_details._INFO.installdir .. "/**.lib", target:targetdir())
                os.cp(pkg_details._INFO.installdir .. "/**.a", target:targetdir())
            end
        end
    end)

target("devicetests")
    set_default(true)
    set_kind("binary")
    add_files("examples/devicetests.cpp")
    add_packages("bass", "bass-fx", "libopus")
    add_deps("alt-voice")
    add_rules("plugin.vsxmake.autoupdate")