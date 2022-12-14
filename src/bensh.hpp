#include <vector>
#include <string>

void signalHandler(int signum);
void signalIntHandler(int signum);
int cd(std::vector<std::string> command);
std::vector<std::string> parse_command(std::string s);
bool print_command_error(int e, std::string cs);
void run_command(std::vector<std::string> command);
std::vector<std::string> get_path_commands(void);
void print_help(void);
void print_centered_text_line(std::string s);