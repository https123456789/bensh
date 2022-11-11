#include <iostream>
#include <vector>

int main(int argc, char **argv) {
    std::cout << "BenSH - Ben's Shell" << std::endl;
    while (1) {
        std::cout << ">> ";
        std::string s;
        std::cin >> s;
        std::cout << s;
    }
    return 0;
}