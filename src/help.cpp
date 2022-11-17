#include "bensh.hpp"
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ioctl.h>

namespace fs = std::filesystem;

void print_help(void) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    for (int i = 0; i < (w.ws_col / 2) - 10; i++) {
        std::cout << " ";
    }
    std::cout << "\u250C\u2500\u2500\u2500\u2500\u2500\u2500\u2510\n\r";
    for (int i = 0; i < (w.ws_col / 2) - 10; i++) {
        std::cout << " ";
    }
    std::cout << "\u2502 Help \u2502\n\r";
    for (int i = 0; i < (w.ws_col / 2) - 10; i++) {
        std::cout << " ";
    }
    std::cout << "\u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2518\n\r";

    std::cout << "cd - Change the current directory\n\r";
    std::cout << "help - Show this help message\n\r";
}

std::vector<std::string> get_path_commands(void) {
    std::vector<std::string> commands = {};
    std::vector<std::string> paths = {};
    const char delim = ':';

    // Get a list of all dirs on path
    const char* env_p = std::getenv("PATH");
    if (!env_p) {
        std::cerr << "\x1b[31mError: failed to load PATH.\x1b[0m\n";
        return commands;
    }
    std::string env_path = std::string(env_p);

    std::string scop = std::string(env_path);
    std::string seg = "";
    size_t pos = scop.find(delim);
    
    while (pos != std::string::npos) {
        seg = scop.substr(0, pos);
        scop = scop.substr(pos + 1);
        pos = scop.find(delim);
        paths.push_back(seg);
    }
    paths.push_back(scop);

    // Search each dir on the path
    for (int i = 0; i < paths.size(); i++) {
        // Check if the dir exists
        if (!fs::is_directory(fs::path(paths[i]))) {
            std::cerr << "\x1b[93mWarning: directory '" << paths[i] << "' is not a valid file or directory\x1b[0m\n\r";
            continue;
        }
        std::vector<std::string> files = {};
        for (const auto & entry : fs::directory_iterator(paths[i])) {
            files.push_back(entry.path());
        }
        for (int j = 0; j < files.size(); j++) {
            std::string fn = fs::path(files[j]).filename();
            // Exclude files with non-letter first characters
            if (fn.c_str()[0] != '.' && ((fn.c_str()[0] <= 122 && fn.c_str()[0] >= 97) || (fn.c_str()[0] >= 65 && fn.c_str()[0] <= 90))) {
                commands.push_back(fn);
            }
        }
    }
    
    return commands;
}