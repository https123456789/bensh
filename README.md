# BenSH

A shell written with my use cases in mind.

## Building

```
meson setup build
meson compile -C build
```

## Running

```
./build/bensh
```

## Just

If you have just installed, you can use the following commands:

- `just setup`: Setup Meson
- `just build`: Build
- `just run`: Build and run
- `just debug`: Build and launch with lldb
