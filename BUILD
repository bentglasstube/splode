package(default_visibility = ["//visibility:public"])

cc_library(
    name = "game_screen",
    srcs = ["game_screen.cc"],
    hdrs = ["game_screen.h"],
    deps = [
        "@libgam//:audio",
        "@libgam//:graphics",
        "@libgam//:screen",
        "@libgam//:text",
        ":level",
        ":ship",
    ],
)

cc_library(
    name = "geometry",
    srcs = ["geometry.cc"],
    hdrs = ["geometry.h"],
    deps = [
        "@libgam//:graphics",
    ],
)

cc_library(
    name = "level",
    srcs = ["level.cc"],
    hdrs = ["level.h"],
    deps = [
        "@libgam//:graphics",
        ":geometry",
    ],
)

cc_library(
    name = "particle",
    srcs = ["particle.cc"],
    hdrs = ["particle.h"],
    deps = [
        "@libgam//:graphics",
        ":geometry",
    ],
)

cc_library(
    name = "ship",
    srcs = ["ship.cc"],
    hdrs = ["ship.h"],
    deps = [
        "@libgam//:audio",
        "@libgam//:graphics",
        ":geometry",
        ":particle",
    ],
)

cc_binary(
    name = "splode",
    data = ["//content"],
    linkopts = [
        "-lSDL2",
        "-lSDL2_image",
        "-lSDL2_mixer",
        "-lSDL2_ttf",
        "-static-libstdc++",
        "-static-libgcc",
    ],
    srcs = ["main.cc"],
    deps = [
        ":title_screen",
        "@libgam//:game",
    ],
)

cc_library(
    name = "title_screen",
    srcs = ["title_screen.cc"],
    hdrs = ["title_screen.h"],
    deps = [
        "@libgam//:screen",
        "@libgam//:sprite",
        "@libgam//:text",
        ":game_screen",
    ],
)

