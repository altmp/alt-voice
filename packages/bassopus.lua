package("bassopus")
    if is_plat("windows") then
        add_urls("https://www.un4seen.com/files/bassopus$(version).zip")
        add_versions("24", "bb0d762aba3f3f7d940a3f14da64e46b96958a5d071930a7d959e66141e8bd8d")
    end

    if is_plat("linux|x86_64") then
        add_urls("https://www.un4seen.com/files/bassopus$(version)-linux.zip")
        add_versions("24", "357b875a0ef7cb44f0402bfc9765545e176bacf9822817e322f488baee81d66e")
    end

    on_install("windows", function (package)
        os.cp("c/x64/bassopus.lib", package:installdir("lib"))
        os.cp("x64/bassopus.dll", package:installdir("bin"))
        os.cp("c/bassopus.h", package:installdir("include"))
    end)

    on_install("linux|x86_64", function (package)
        os.cp("libs/x86_64/libbassopus.so", package:installdir("bin"))
        os.cp("bassopus.h", package:installdir("include"))
    end)