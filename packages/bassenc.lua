package("bassenc")
    if is_plat("windows") then
        add_urls("https://www.un4seen.com/files/bassenc$(version).zip")
        add_versions("24", "81fd812bc120fea5f14ca9786cde078c7522e77d75a2e227c5c8d638aa9fb68c")
    end

    if is_plat("linux|x86_64") then
        add_urls("https://www.un4seen.com/files/bassenc$(version)-linux.zip")
        add_versions("24", "107d81f67a4317479bfc1d85053cff3b801774a18b6e0f744288fc2f60f4db7a")
    end

    on_install("windows", function (package)
        os.cp("c/x64/bassenc.lib", package:installdir("lib"))
        os.cp("x64/bassenc.dll", package:installdir("bin"))
        os.cp("c/bassenc.h", package:installdir("include"))
    end)

    on_install("linux|x86_64", function (package)
        os.cp("libs/x86_64/libbassenc.so", package:installdir("bin"))
        os.cp("bassenc.h", package:installdir("include"))
    end)