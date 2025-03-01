# BenSH

A shell written with my use cases in mind.

## Why?

This project mostly serves as a way to teach myself how to write very fault-tolerant C code.
I expect that my shell should never crash and be relatively minimal, thus making it the perfect balance between a need for reliablity and low complexity.

## Building

```
just setup
just build
```

## Running

```
./build/bensh
#or
just run
```

## Testing

Note that this project uses [Unity](https://www.throwtheswitch.org/unity/) and therefore needs ruby installed to generate the test runners.

On Arch Linux, you can install the following packages: `ruby`, `ruby-erb`.

```
meson test -C build
```

## Just

If you have just installed, you can use the following commands:

- `just setup`: Setup Meson
- `just build`: Build
- `just run`: Build and run
- `just debug`: Build and launch with lldb
- `just test`: Build and test
- `just coverage`: Generate a code coverage report
