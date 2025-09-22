#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <string>
#include <iostream>
#include <map>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iterator>

class BitcoinExchange {
    public:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange &other);
        BitcoinExchange &operator=(const BitcoinExchange &other);
        ~BitcoinExchange();
        //Methods
        void loadDataBase();
        void execute(const std::string &inputFile);
        bool parseInputFile(const std::string &inputFile);
        bool dateCheck(const std::string &date);
        int myStoi(const std::string &str);
        double myStod(const std::string &str);
        std::string myStrTrim(const std::string &str);
    private:
        std::map<std::string, double> _dataBase;
        //Helper
        double stringToDouble(const std::string &str);

    
};

#endif