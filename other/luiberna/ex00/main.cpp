#include "BitcoinExchange.hpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Error: Incorrect number of arguments." << std::endl;
        std::cout << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    std::string inputFile = argv[1];
    BitcoinExchange exchange;
    exchange.loadDataBase();
    exchange.execute(inputFile);
    return 0;
}