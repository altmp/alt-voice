package("bass")
    if is_plat("windows") then
        add_urls("https://www.un4seen.com/files/bass$(version).zip")
        add_versions("24", "2af3387fa4f0190ed7e9c28fdaf32f4d310a391a4b53a308186f069507e4d8ec")
    end

    if is_plat("linux") then
        add_urls("https://www.un4seen.com/files/bass$(version)-linux.zip")
        add_versions("24", "71142678cc848f0bbabcfaa18acb7e8fc74543dd11d08521c133a016dd4aa473")
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