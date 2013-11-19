require "buildconf"

solution "Infinity"
    location        "projects"
    configurations  { "Debug", "Release" }
    platforms       { "native", "x32" }

    library {
        name = "framework",
        src = {
            "SDK/framework/*.h",
            "SDK/framework/*.cpp",
            "SDK/framework/*.c",
            "SDK/framework/SOIL/*.h",
            "SDK/framework/SOIL/*.c",
        }
    }    
    
    library {
        name = "utils",
        src = {
            "SDK/include/mjson.h",
            "SDK/include/vi.h",
            "SDK/include/vi.inl",
            "SDK/utils/ml.h",
            "SDK/utils/utils.h",
            "SDK/utils/ml.cpp",
            "SDK/utils/utils.cpp",
            "SDK/utils/mjson.c"
        }
    }

    library {
        name = "vg",
        src = {
            "SDK/VG/**.h",
            "SDK/VG/**.cpp"
        }
    }
    
    sample {
        name = "Anima",
        src = {
            "Samples/Anima/anima.cpp",
            "Samples/Anima/md5.cpp",
            "Samples/Anima/model.cpp",
            "Samples/Anima/md5.h",
            "Samples/Anima/model.h"
        }
    }

    sample {
        name = "AO",
        src = {
            "Samples/AO/AOSample.cpp"
        }
    }

    sample {
        name = "Clock",
        src = {
            "Samples/Clock/Clock.cpp"
        }
    }

    sample {
        name = "exest",
        src = {
            "Samples/exest/CDLODTerrain.cpp",
            "Samples/exest/main.cpp",
            "Samples/exest/CDLODTerrain.h"
        }
    }

    sample {
        name = "frest",
        src = {
            "Samples/frest/DebugDraw.cpp",
            "Samples/frest/main.cpp",
            "Samples/frest/MeshLoaderObj.cpp",
            "Samples/frest/DebugDraw.h",
            "Samples/frest/MeshLoaderObj.h"
     }
    }
    
    sample {
        name = "physis",
        src  = {
            "Samples/physis/lighting.h",
            "Samples/physis/perlin.h",
            "Samples/physis/pp.h",
            "Samples/physis/color_guided_filter.cpp",
            "Samples/physis/guided_filter.cpp",
            "Samples/physis/lighting.cpp",
            "Samples/physis/physis.cpp",
            "Samples/physis/pp.cpp",
        }
    }

    sample {
        name = "SchedulerDev",
        src  = {
            "Samples/SchedulerDev/main.cpp",
        }
    }

    sample {
        name = "SVGRendering",
        src  = {
            "Samples/SVGRendering/nanosvg.h",
            "Samples/SVGRendering/main.cpp",
            "Samples/SVGRendering/nanosvg.c",
        }
    }

    sample {
        name = "UIDemo",
        src  = {
            "Samples/UIDemo/fontstash.h",
            "Samples/UIDemo/glnanovg.h",
            "Samples/UIDemo/nanovg.h",
            "Samples/UIDemo/stb_truetype.h",
            "Samples/UIDemo/nanovg.c",
            "Samples/UIDemo/uidemo.cpp"
        }
    }
    
    sample {
        name = "VGResearch",
        src  = {
            "Samples/VGResearch/VGSample.cpp",
        }
    }

    sample {
        name = "VGTest",
        src  = {
            "Samples/VGTest/Path.h",
            "Samples/VGTest/VGExp.h",
            "Samples/VGTest/main.cpp",
            "Samples/VGTest/VGExp.cpp",
        }
    }

    sample {
        name = "Video",
        src  = {
            "Samples/Video/VideoSample.cpp",
        }
    }
