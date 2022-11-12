#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <vector>

int main(int argc, char **argv) {
    std::cout << "BenSH - Ben's Shell\nRun exit to quit." << std::endl;

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
    /*term.c_lflag &= (~ICANON & ~ECHO);
    term.c_cc[VTIME] = 0;
    term.c_cc[VMIN] = 1;

    term.c_iflag &=
        ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    term.c_oflag &= ~OPOST;
    term.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    term.c_cflag &= ~(CSIZE | PARENB);
    */

    if (tcsetattr(fileno(stdin), TCSANOW, &term) < 0) {
        std::cerr << "Unable to set terminal to change terminal configuration"
                  << std::endl;
        return -1;
    }

    std::string input = "";
    bool prompt = true;
    bool flushLine = false;

    // Read single characters from cin.
    std::streambuf *pbuf = std::cin.rdbuf();
    bool done = false;
    while (!done) {
        if (prompt) {
            std::cout << ">> ";
            prompt = false;
        }

        char c;
        if (pbuf->sgetc() == EOF) {
            done = true;
        }

        c = pbuf->sbumpc();

        if (c == 0xa || c == 0xd) {
            flushLine = true;
        } else {
            input += c;
            std::cout << c;
        }

        if (flushLine) {
            flushLine = false;
            prompt = true;
            std::cout << "\n\r" << input;

            if (input == std::string("exit")) {
                std::cout << "\n";
                break;
            }

            input = "";
        }
    }

    /*while (1) {
        if (prompt) {
            std::cout << ">> ";
            prompt = false;
        }
        int c = 0;
        // c = getch();
        switch (c) {
        default:
            input += c;
            std::cout << input;
            break;
        }
        // std::getline(std::cin, s);
        if (flushLine) {
            std::cout << input;
        }
    }*/

    // Restore terminal state
    if (tcsetattr(fileno(stdin), TCSANOW, &term_save) < 0) {
        std::cerr << "Unable to restore terminal mode" << std::endl;
        return -1;
    }

    return 0;
}