git_repository(
    name = "libgam",
    remote = "https://github.com/bentglasstube/gam.git",
    commit = "c1a3be7b8d80f6bf37b09f1def7967bad18278a0",
)

git_repository(
    name = "mxebzl",
    remote = "https://github.com/cfrantz/mxebzl.git",
    tag = "20170703_RC03",
)

load("@mxebzl//tools:repository.bzl", "mxe_compilers")
mxe_compilers(
    deps = [
        "compiler",
        "SDL2",
        "SDL2-extras",
        "bzip2",
        "freetype-bootstrap",
        "gettext",
        "glib",
        "harfbuzz",
        "libiconv",
        "pcre",
        "xz",
    ],
)
