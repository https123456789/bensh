// Copyright 2023 Ben Landon

#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include "bensh.hpp"

void print_centered_text_line(std::string s) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    for (int i = 0; i < (w.ws_col - s.length()) / 2; i++) {
        std::cout << " ";
    }
    std::cout << s << "\n\r";
}
