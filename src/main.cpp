#include "bensh.hpp"

#include <cerrno>
#include <climits>
#include <csignal>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

std::vector<std::string> parse_command(std::string s);
bool print_command_error(int e, std::string cs);
void run_command(std::vector<std::string> command);
void restore_term(struct termios *save);

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

    // Setup IO variables
    std::string input = "";
    bool prompt = true;
    bool flushLine = false;

    // Shell env variables
    std::string cwd = "";

    // Load cwd
    {
        char buf[PATH_MAX];
        if (getcwd(buf, sizeof(buf)) != NULL) {
            std::cout << "\x1b[36m[DEBUG] CWD: " << buf << "\x1b[0m\n\r";
        } else {
            // Error
            std::cerr << "\x1b[31mFatal Error: " << std::strerror(errno)
                      << "\x1b[0m\n\r";
            restore_term(&term_save);
            _Exit(1);
        }
    }

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
        if (c >= 0x20 && c <= 0x7e) {
            std::cout << c << std::flush;
        }

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
            std::cout << "\n\r";

            std::vector<std::string> command = parse_command(input);

            if (command[0] == std::string("exit")) {
                goto endLabel;
            } else if (command[0] == std::string("cd")) {
                if (cd(command) < 0) {
                    std::cerr << "\x1b[31mFailed to change working directory.\x1b[0m\n\r";
                }
            } else {
                run_command(command);
            }

            input = "";
        }
    }

endLabel:
    // Restore terminal state
    restore_term(&term_save);
    return 0;
}

int cd(std::vector<std::string> command) {
    if (command.size() < 2) {
        if (command.size() == 1) {
            command.push_back("~");
        } else {
            command[1] == std::string("~");
        }
    }
    return chdir(command[1].c_str());
}

void restore_term(struct termios *save) {
    if (tcsetattr(fileno(stdin), TCSANOW, &(*save)) < 0) {
        std::cerr << "Unable to restore terminal mode" << std::endl;
        _Exit(1);
    }
}