package("bassmix")
    if is_plat("windows") then
        add_urls("https://www.un4seen.com/files/bassmix$(version).zip")
        add_versions("24", "004fc3fc20b38da2641523e01b026b570f63525f2ae4ca9566fe291fbd07242c")
    end

    if is_plat("linux") then
        add_urls("https://www.un4seen.com/files/bassmix$(version)-linux.zip")
        add_versions("24", "a31c4186c8de2ef528760d9238c0c7e30532e676b79f12d3a98901fa25db3a00")
    end

    on_install("windows", function (package)
        os.cp("c/x64/bassmix.lib", package:installdir("lib"))
        os.cp("x64/bassmix.dll", package:installdir("bin"))
        os.cp("c/bassmix.h", package:installdir("include"))
    end)

    on_install("linux", function (package)
        os.cp("libs/x86_64/libbassmix.so", package:installdir("bin"))
        os.cp("bassmix.h", package:installdir("include"))
    end)