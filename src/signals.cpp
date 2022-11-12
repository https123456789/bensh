#include "bensh.hpp"

void signalHandler(int signum) {
    switch (signum) {
    case 2: // CTRL C
        std::cout << "^C" << std::flush;
        break;
    default:
        std::cout << "\nInterrupt signal (" << signum << ") received.\n";
        _Exit(signum);
        break;
    }
}