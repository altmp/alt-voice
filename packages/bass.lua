package("bass")
    if is_plat("windows") then
        add_urls("https://www.un4seen.com/files/bass$(version).zip")
        add_versions("24", "906dee15e357669b646c9a6bcb225e817203ba34ad921c627f7546237fec93b5")
    end

    if is_plat("linux") then
        add_urls("https://www.un4seen.com/files/bass$(version)-linux.zip")
        add_versions("24", "a72fdacb6bb23b899362d449ac0e36ada0b68d57825dce07b28cdcc91f76912c")
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