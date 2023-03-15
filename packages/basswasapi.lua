package("basswasapi")
    if is_plat("windows") then
        add_urls("https://www.un4seen.com/files/basswasapi$(version).zip")
        add_versions("24", "d2c3303377ed4ba137103a3b0b496f5688f94a34340f2e36dc04c4693bfa9d8a")
    end

    on_install("windows", function (package)
        os.cp("c/x64/basswasapi.lib", package:installdir("lib"))
        os.cp("x64/basswasapi.dll", package:installdir("bin"))
        os.cp("c/basswasapi.h", package:installdir("include"))
    end)