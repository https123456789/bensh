#include "bensh.hpp"
#include <iostream>
#include <setjmp.h>
#include <signal.h>

extern sigjmp_buf env;
extern volatile sig_atomic_t jump_active;

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

void signalIntHandler(int signum) {
    if (!jump_active) {
        return;
    }
    siglongjmp(env, 42);
}