#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <iomanip>
#include <exception>
#include <vector>
#include <cstdlib>


class BitcoinExchange
{
    private:
        std::map<int, float> dateToPrice;
        void    checkDate(const std::string& str) const;
        void    splitAndPrint(const std::string& line) const;

    public:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange& ); 
        BitcoinExchange& operator=(const BitcoinExchange& );
        BitcoinExchange(const std::string& filename);
        ~BitcoinExchange(); 

        void    printValues(const std::string& filename) const;
        float   calcTot(int date, float val) const;
};

#endif // BITCOINEXCHANGE.HPP