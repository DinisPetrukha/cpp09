#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
}

int convertDate(std::string toSplit, char delimiter)
{
    std::string val;
    std::size_t pos;
    int         ret = 0;
    int         state = 10;

    do
    {
        pos = toSplit.find(delimiter);
        val = toSplit.substr(0, pos);
        if (!val.empty())
            ret |= (std::atoi(val.data()) << state);
        state -= 5;
        toSplit.erase(0, pos + 1);
    } while (pos != toSplit.npos);
    return (ret);
}


BitcoinExchange::BitcoinExchange(const std::string& filename)
{
    std::ifstream   f(filename.c_str());
    std::string     line;
    int             date;
    float           value;

    if (f.bad())
        throw std::runtime_error("Error in opening the file provided");
    getline(f, line); //skip first line
    while (getline(f, line, ','))
    {
        checkDate(line);
        date = convertDate(line, '-');
        getline(f, line);
        value = std::atof(line.data());
        dateToPrice.insert(std::make_pair(date, value));
    }
    #ifdef VERBOSE
        std::cout << "BitcoinExchange constructor called" << std::endl;
    #endif
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& btc) : dateToPrice(btc.dateToPrice)
{
    #ifdef VERBOSE
        std::cout << "BitcoinExchange copy constructor called" << std::endl;
    #endif
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& bitcoinexchange)
{
    dateToPrice = bitcoinexchange.dateToPrice;
    #ifdef VERBOSE
        std::cout << "BitcoinExchange Copy assignment operator called" << std::endl;
    #endif
    return (*this);
}

BitcoinExchange::~BitcoinExchange()
{
    dateToPrice.erase(dateToPrice.begin(), dateToPrice.end());
    #ifdef VERBOSE
        std::cout << "BitcoinExchange destructor called" << std::endl;
    #endif
}

void BitcoinExchange::checkDate(const std::string& dateStr) const
{
    if (dateStr.length() != 10)
        throw std::runtime_error("Error: bad lenght => " + dateStr);

    for (u_int index = 0; index < dateStr.length(); ++index)
    {
        if (!isdigit(dateStr[index]))
        {
            if ((index == 4 || index == 7))
            {
                if (dateStr[index] != '-')
                    throw std::runtime_error("Error: bad separator => " + dateStr);
            }
            else
                throw std::runtime_error("Error: bad char => " + dateStr);
        }
    }
    int year = std::atoi(dateStr.data());
    if (year < 1990 || year > 2030)
        throw std::runtime_error("Error: bad year => " + dateStr);
    int month = std::atoi(&dateStr[5]);
    if (month < 1 || month > 12)
        throw std::runtime_error("Error: bad month => " + dateStr);
    int day = std::atoi(&dateStr[8]);
    if (day < 1 || day > 31)
        throw std::runtime_error("Error: bad day => " + dateStr);
}


float BitcoinExchange::calcTot(int date, float val) const
{
    std::map<int, float>::const_iterator it;

    if ((it = dateToPrice.find(date)) == dateToPrice.end())
    {
        it = dateToPrice.lower_bound(date);
        if (it == dateToPrice.begin())
            throw std::runtime_error("Error: there's no matching date to the one provided");
        --it;
    }
    return(it->second * val);
}

void BitcoinExchange::splitAndPrint(const std::string& line) const
{
    std::string dateStr;
    size_t      pos;
    int         date;
    double      value;

    if ((pos = line.find(" | ") ) == std::string::npos)
    {
            std::cout <<"Error : line not in format \'date | value\' error line => " + line << "\n";
            return;
    }
    dateStr = line.substr(0, pos);
    try
    {
        checkDate(dateStr);
        date = convertDate(line, '-');
        value = std::atof(&line[pos + 3]);
        if (value < 0 || value > __INT_MAX__)
        {
            std::cout << "Error : value not in range 0 - 1000 error line => " + line << "\n";
            return;
        }
        std::cout << std::fixed << std::setprecision(2);
        std::cout << dateStr << " => " << value << " = " << calcTot(date, value) << "\n";
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}

void BitcoinExchange::printValues(const std::string& filename) const
{
    std::ifstream   f;
    std::string     line;

    f.open(filename.c_str(), std::ifstream::in);
    if (f.fail())
    {
        throw std::runtime_error("Error in opening the file provided");
    }
    getline(f, line); //skip first line
    while (getline(f, line))
    {
        if (line.empty() == false)
            splitAndPrint(line);
    }
}
