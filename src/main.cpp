#include "bensh.hpp"

#include <csignal>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <vector>

std::vector<std::string> parse_command(std::string s);

int main(int argc, char **argv) {
    std::cout << "\u250C\u2500\u2500\u2500\u2500"
              << "\u2500\u2500\u2500\u2500\u2500"
              << "\u2500\u2500\u2500\u2500\u2500"
              << "\u2500\u2500\u2500\u2500\u2500"
              << "\u2500\u2500\u2510\n"
              << "\u2502 BenSH - Ben's Shell \u2502\n\u2514"
              << "\u2500\u2500\u2500\u2500\u2500"
              << "\u2500\u2500\u2500\u2500\u2500"
              << "\u2500\u2500\u2500\u2500\u2500"
              << "\u2500\u2500\u2500\u2500\u2500\u2500\u2518" << std::endl;
    std::cout << "Run exit to quit." << std::endl;

    // Terminal Configuration
    struct termios term;
    struct termios term_save; // Save of the state

    // Change the terminal config
    tcgetattr(fileno(stdin), &term);
    term_save = term;

    term.c_iflag &= IGNCR;

    term.c_cflag |= CS8; // 8 bits
    term.c_lflag &=
        (~ICANON & ~ECHO); // Disable canonical mode (if set) and echo on type

    term.c_cc[VMIN] = 1;  // Minimum of 1 characters
    term.c_cc[VTIME] = 0; // Time delay for input

    if (tcsetattr(fileno(stdin), TCSANOW, &term) < 0) {
        std::cerr << "Unable to set terminal to change terminal configuration"
                  << std::endl;
        return -1;
    }

    // Connect the signal handler
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    std::string input = "";
    bool prompt = true;
    bool flushLine = false;

    // Read single characters from cin.
    std::streambuf *pbuf = std::cin.rdbuf();
    bool done = false;
    while (!done) {
        if (prompt) {
            std::cout << "\x1b[32m>>\x1b[0m ";
            prompt = false;
        }

        char c;
        if (pbuf->sgetc() == EOF) {
            done = true;
        }

        c = pbuf->sbumpc();
        std::cout << c << std::flush;

        switch (c) {
        case 0x8:
        case 0x7f:
            if (input.length() > 0) {
                // Erase the character from the terminal
                std::cout << "\b \b" << std::flush;
                // Remove the item from the inut string
                input.pop_back();
            }
            break;
        case 0xa:
        case 0xd:
            flushLine = true;
            break;
        default:
            input += c;
            // std::cout << c;
            break;
        }

        if (flushLine) {
            flushLine = false;
            prompt = true;
            std::cout << "\n\r" << input;

            std::vector<std::string> command = parse_command(input);

            for (int i = 0; i < command.size(); i++) {
                if (command[i] == std::string("exit") && i == 0) {
                    std::cout << "\n";
                    goto endLabel;
                    break;
                }
                // std::cout << "\n--- " << command[i] << "\n" << std::flush;
            }

            if (input == std::string("exit")) {
            }

            input = "";
        }
    }

endLabel:
    // Restore terminal state
    if (tcsetattr(fileno(stdin), TCSANOW, &term_save) < 0) {
        std::cerr << "Unable to restore terminal mode" << std::endl;
        return -1;
    }

    return 0;
}

std::vector<std::string> parse_command(std::string s) {
    std::vector<std::string> command = {};
    const char delim = ' ';
    char c;
    int i = 0;
    bool inStr = false;
    std::string sect = "";
    std::vector<char> chars(s.begin(), s.end());

    while (i < chars.size()) {
        switch (chars[i]) {
        case '"':
        case '\'':
            inStr = !inStr;
            break;
        case delim:
            if (!inStr) {
                command.push_back(sect);
                sect = "";
            }
            break;
        default:
            sect += chars[i];
            break;
        }
        if (chars[i] == delim) {
        }
        i++;
    }
    command.push_back(sect);

    return command;
}