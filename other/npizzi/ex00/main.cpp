#include "BitcoinExchange.hpp"

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "Usage error : please provide a txt file to show how much you lost on bitcoins!\n";
        return (1);
    }
    std::string filename = argv[1];
    if (filename.find(".txt", filename.length() - 4) == filename.npos)
    {
        std::cout << "Nooo, provide a txt file please!\n";
        return (2);
    }
    try
    {
        BitcoinExchange bp("data.csv");
        bp.printValues(filename);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}