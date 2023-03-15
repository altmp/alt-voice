package("bassenc_opus")
    if is_plat("windows") then
        add_urls("https://www.un4seen.com/files/bassenc_opus$(version).zip")
        add_versions("24", "6d8acc8d17021e81f4a769cce8e705bc07cfb6fbe283e31a31cd9bc2bccd1a3c")
    end

    if is_plat("linux|x86_64") then
        add_urls("https://www.un4seen.com/files/bassenc_opus$(version)-linux.zip")
        add_versions("24", "aad1f5d9502a07b4087a8759637ed9bb6e516a2f8549918a44333212afd5ce72")
    end

    on_install("windows", function (package)
        os.cp("c/x64/bassenc_opus.lib", package:installdir("lib"))
        os.cp("x64/bassenc_opus.dll", package:installdir("bin"))
        os.cp("c/bassenc_opus.h", package:installdir("include"))
    end)

    on_install("linux|x86_64", function (package)
        os.cp("libs/x86_64/libbassenc_opus.so", package:installdir("bin"))
        os.cp("bassenc_opus.h", package:installdir("include"))
    end)