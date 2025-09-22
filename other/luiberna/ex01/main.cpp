#include "RPN.hpp"

int main (int argc, char **argv) {
    RPN rpn;
    if (argc != 2) {
        std::cerr << "Error: Incorrect number of arguments." << std::endl;
        std::cout << "Usage: " << argv[0] << " <input_expression>" << std::endl;
        return 1;
    }
    std::string input = argv[1];
    rpn.execute(input);
}

