require "buildconf"

solution "Infinity"
    location        "projects"
    configurations  { "Debug", "Release" }
    platforms       { "native", "x64", "x32" }

    sample {
        name = "UIDemo"
    }