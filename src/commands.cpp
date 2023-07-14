// Copyright 2023 Ben Landon

#include <sys/wait.h>
#include <unistd.h>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <iostream>
#include "bensh.hpp"

pid_t childPid;

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
        i++;
    }
    command.push_back(sect);

    return command;
}

bool print_command_error(int e, std::string cs) {
    if (e == 2) {
        std::cerr << "\x1b[31mNo such command '" << cs << "'\x1b[0m\n\r";
        return true;
    }
    return false;
}

void run_command(std::vector<std::string> command) {
    pid_t waitRes;
    int statLoc;

    childPid = fork();
    if (childPid == 0) {
        // Convert the command to an array of char pointers
        char *strCommand[command.size() + 1];
        for (int i = 0; i < command.size(); i++) {
            strCommand[i] = const_cast<char *>(command[i].c_str());
        }
        strCommand[command.size()] = nullptr;

        // Execute the command
        // This will never return to the code if it works
        int r = execvp(command[0].c_str(), strCommand);
        std::string sc = "";
        for (int i = 0; i < command.size(); i++) {
            if (i != 0) {
                sc += " ";
            }
            sc += command[i];
        }
        if (print_command_error(errno, sc)) {
            exit(errno);
        }
        std::cerr << "execp failed " << r << " - errno " << errno << "\n"
                  << std::strerror(errno) << "\n";
        _Exit(errno);
    } else {
        waitRes = waitpid(childPid, &statLoc, WUNTRACED);
    }
}
