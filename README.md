# BenSH - Ben's Shell

A shell written with my common use cases in mind.

This project was heavily inspired by [indradhanush's](https://github.com/indradhanush) [blog post on shells](https://indradhanush.github.io/blog/writing-a-unix-shell-part-1) and the [Build Your Own X](https://github.com/codecrafters-io/build-your-own-x) project.

## Features

- Run any command on `PATH`.
- `exit`
- `cd`
- Find new and interesting bugs :sweat_smile:

## Known Bugs :bug: :beetle: :cricket:

- [#1 Arrow Keys Combined With Backspace Can Remove Prompt](https://github.com/https123456789/bensh/issues/1)
## To Do

- Add shell scripting
- Add configuration files
- Add pipes and I/O redirection
- Add `history` command
- Add `info` command
- Add up and down arrow history movement
- Add left and right arrow comamnd editing
- Fix prompt bug ([#1](https://github.com/https123456789/bensh/issues/1))

## Design

```mermaid
graph TD
    A[Shell Starts] --> B(Adjust terminal settings)
    B --> C(Show prompt)
    C --> D(User types input)
    D --> E(Check for interupt)
    E -->|Interupt| F(Handle interupt)
    E -->|No Interupt| G(Validate command)
    F --> C
    G -->|Exit Command| I(Restore terminal settings)
    G -->|Cd command| K(Change working directory)
    K --> C
    I --> J(Exit)
    G -->|Valid Command| H(Fork & execute)
    G -->|Invalid Command| C
    H --> C
```