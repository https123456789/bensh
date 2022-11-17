#include "bensh.hpp"
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

void print_centered_text_line(std::string s) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    for (int i = 0; i < (w.ws_col - s.length()) / 2; i++) {
        std::cout << " ";
    }
    std::cout << s << "\n\r";
}