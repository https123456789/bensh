alias b := build
alias r := run
alias d := debug

[private]
default:
    just --list

setup:
    meson setup build

build:
    meson compile -C build

run: build
    ./build/bensh

debug: build
    lldb ./build/bensh
