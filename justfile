alias b := build
alias r := run
alias d := debug

[private]
default:
    just --list

setup:
    meson setup build --wipe -Db_coverage=false

build:
    meson compile -C build

run: build
    ./build/bensh

run-examples: build
    ./build/bensh examples/hello_world.bensh

debug: build
    lldb ./build/bensh

test:
    meson test -C build

coverage:
    meson setup build --wipe -Db_coverage=true
    meson test -C build
    ninja coverage-html -C build
