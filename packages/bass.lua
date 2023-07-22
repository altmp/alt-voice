package("bass")
    if is_plat("windows") then
        add_urls("https://www.un4seen.com/files/bass$(version).zip")
        add_versions("24", "bd6d5f41ddc3162084a876836b73ddde7ed1f19dd529d37db404157482ba9e5f")
    end

    if is_plat("linux") then
        add_urls("https://www.un4seen.com/files/bass$(version)-linux.zip")
        add_versions("24", "6ee779f4b85da821d4a5383cf3e8266592a2ba108ff59976754fc0f8a6b4e4fd")
    end

    on_install("windows", function (package)
        os.cp("c/x64/bass.lib", package:installdir("lib"))
        os.cp("x64/bass.dll", package:installdir("bin"))
        os.cp("c/bass.h", package:installdir("include"))
    end)

    on_install("linux", function (package)
        os.cp("libs/x86_64/libbass.so", package:installdir("bin"))
        os.cp("c/bass.h", package:installdir("include"))
    end)