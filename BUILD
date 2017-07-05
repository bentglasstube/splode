package(default_visibility = ["//visibility:public"])

load("@mxebzl//tools/windows:rules.bzl", "pkg_winzip")

config_setting(
    name = "windows",
    values = {
        "crosstool_top": "@mxebzl//tools/windows:toolchain",
    }
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
    name = "screens",
    srcs = [
        "game_screen.cc",
        "high_score_screen.cc",
        "title_screen.cc",
    ],
    hdrs = [
        "game_screen.h",
        "high_score_screen.h",
        "title_screen.h",
    ],
    deps = [
        "@libgam//:audio",
        "@libgam//:graphics",
        "@libgam//:screen",
        "@libgam//:sprite",
        "@libgam//:text",
        ":level",
        ":ship",
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
    linkopts = select({
        ":windows": [ "-mwindows", "-lSDL2main" ],
        "//conditions:default": [],
    }) + [
        "-lSDL2",
        "-lSDL2_image",
        "-lSDL2_mixer",
        "-lSDL2_ttf",
        "-static-libstdc++",
        "-static-libgcc",
    ],
    srcs = ["main.cc"],
    deps = [
        "@libgam//:game",
        ":screens",
    ],
)

pkg_winzip(
    name = "splode-windows",
    files = [
        ":splode",
        "//content",
    ]
)
