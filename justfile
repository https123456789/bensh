alias b := build
alias r := run
alias d := debug

[private]
default:
    just --list

setup:
    meson setup build --wipe -Db_coverage=true

build:
    meson compile -C build

run: build
    ./build/bensh

debug: build
    lldb ./build/bensh

test:
    meson test -C build

coverage: setup test
    ninja coverage-html -C build
