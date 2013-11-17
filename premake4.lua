
solution "Infinity"
    location        "projects"
    configurations  { "Debug", "Release" }
    platforms       { "native", "x64", "x32" }

    project "UIDemo"
        kind        "ConsoleApp"
        language    "C++"
        files {
            "Samples/UIDemo/*.c",
            "Samples/UIDemo/*.cpp",
            "Samples/UIDemo/*.h"
        }
        excludes {
            "Samples/UIDemo/stb_image.c"
        }
        includedirs {
            "SDK",
            "SDK/include",
            "SDK/framework",
            "SDK/VG",
            "SDK/utils",
            "SDK/framework/SOIL",
            "SDK/External",
            "SDK/External/FreeType/include",
        }
        libdirs {
            "SDK/lib"
        }

        configuration "windows"
            links       { "glu32","opengl32", "gdi32", "winmm", "user32" }

        configuration "Debug"
            links {
                "scintilla_d",
                "utils_d",
                "physfs_d",
                "framework_d",
                "sdl2_d",
                "sdl2main_d",
                "vg_d",
                "freetype_d",
                "zlib_d"
            }
            defines         { "DEBUG" }
            objdir          "temp/Infinity/samples/UIDemo/debug"
            targetdir       "debug"
            flags           { "EnableSSE2", "Symbols", "ExtraWarnings"}

        configuration "Release"
            links {
                "scintilla",
                "utils",
                "physfs",
                "framework",
                "sdl2",
                "sdl2main",
                "vg",
                "freetype",
                "zlib"
            }            defines         { "NDEBUG" }
            objdir          "temp/Infinity/samples/UIDemo/release"
            targetdir       "release"
            flags           { "EnableSSE2", "OptimizeSize", "Symbols", "ExtraWarnings"}    
