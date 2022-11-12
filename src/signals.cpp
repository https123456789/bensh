#include "bensh.hpp"

void signalHandler(int signum) {
    switch (signum) {
    case 2: // CTRL C
        std::cout << "\nCTRL C\n";
        break;
    default:
        std::cout << "\nInterrupt signal (" << signum << ") received.\n";
        _Exit(signum);
        break;
    }
}