#include <vector>
#include <string>

void signalHandler(int signum);
int cd(std::vector<std::string> command);
std::vector<std::string> parse_command(std::string s);
bool print_command_error(int e, std::string cs);
void run_command(std::vector<std::string> command);