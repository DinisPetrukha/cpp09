#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) {
    *this = other;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
    if (this != &other) {}
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

double BitcoinExchange::stringToDouble(const std::string &str) {
    std::stringstream ss(str);
    double value;
    ss >> value;
    if (ss.fail() || !ss.eof()) {
        std::cerr << "Error: Invalid number format in string '" << str << "'." << std::endl;
        throw std::invalid_argument("Invalid number format");
    }
    return value;
}

void BitcoinExchange::loadDataBase() {
    std::ifstream file("data.csv");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open data file." << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line != "date,exchange_rate") {
            std::string date = line.substr(0, line.find(','));
            std::string rateString = line.substr(line.find(',') + 1);
            double rate = stringToDouble(rateString);
            _dataBase[date] = rate;
        }
    }
    file.close();
}

int BitcoinExchange::myStoi(const std::string &str) {
    std::stringstream ss(str);
    int value;
    ss >> value;
    if (ss.fail() || !ss.eof()) {
        std::cerr << "Error: Invalid integer format in string '" << str << "'." << std::endl;
        throw std::invalid_argument("Invalid integer format");
    }
    return value;
}

bool BitcoinExchange::dateCheck(const std::string &date) {
    //Check if date is in valid format YYYY-MM-DD
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
        std::cerr << "Error: Invalid date format '" << date << "'. Expected format 'YYYY-MM-DD'." << std::endl;
        return false;
    }
    for (size_t i = 0; i < date.size() - 1; ++i) {
        if (i == 4 || i == 7)
            continue;
        if (!std::isdigit(date[i])) {
            std::cerr << "Error: Invalid character in date '" << date << "'." << std::endl;
            return false;
        }
    }
    //Check if year, month, and day are valid
    int year = myStoi(date.substr(0, 4));
    int month = myStoi(date.substr(5, 2));
    int day = myStoi(date.substr(8, 2));
    if (month < 1 || month > 12) {
        std::cerr << "Error: Invalid month in date '" << date << "'." << std::endl;
        return false;
    }
    if (day < 1 || day > 31) {
        std::cerr << "Error: Invalid day in date '" << date << "'." << std::endl;
        return false;
    }
    //Check for February and leap years
    if (month == 2) {
        if (day > 29) {
            std::cerr << "Error: Invalid day in February '" << date << "'." << std::endl;
            return false;
        }
        if (day == 29 && !(year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) {
            std::cerr << "Error: February 29 is not valid in year '" << year << "'." << std::endl;
            return false;
        }
    }
    //Check for months with 30 days
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        std::cerr << "Error: Invalid day in month '" << month << "'." << std::endl;
        return false;
    }
    return true;
}

double BitcoinExchange::myStod(const std::string &str) {
    std::stringstream ss(str);
    double value;
    ss >> value;
    if (ss.fail() || !ss.eof()) {
        std::cerr << "Error: Invalid number format in string '" << str << "'." << std::endl;
        throw std::invalid_argument("Invalid number format");
    }
    if (value < 0 || value > 1000) {
        std::cerr << "Error: Value out of range (0-1000) in string '" << str << "'." << std::endl;
        throw std::out_of_range("Value out of range");
    }
    return value;
}

std::string BitcoinExchange::myStrTrim(const std::string& str) {
    size_t first = 0;
    while (first < str.size() && std::isspace(str[first]))
        ++first;

    size_t last = str.size();
    while (last > first && std::isspace(str[last - 1]))
        --last;

    return str.substr(first, last - first);
}

bool BitcoinExchange::parseInputFile(const std::string &inputFile) {
    std::ifstream file(inputFile.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open input file." << std::endl;
        return false;
    }
    std::string line;
    std::getline(file, line); //Skip header line if it exists
    while (std::getline(file, line)) {
        if (line.empty()) //Skip empty
            continue;
        if (line.find('|') == std::string::npos) {
            std::cerr << "Error: bad input -> " << line << std::endl;
            continue;
        }
        try {
            std::string date = myStrTrim(line.substr(0, line.find('|')));
            std::string valueString = myStrTrim(line.substr(line.find('|') + 1));
            if (date.empty() || valueString.empty()) {
                std::cerr << "Error: bad input -> " << line << std::endl;
                continue;
            }
            if (!dateCheck(date))
                continue;
            double value = myStod(valueString);
            std::map<std::string, double>::iterator it = _dataBase.lower_bound(date);
            if (it == _dataBase.end() || it->first > date) {
                if (it != _dataBase.begin())
                    --it;
                else {
                    std::cerr << "Error: No data available for date '" << date << "'." << std::endl;
                    continue;
                }
            }
            std::cout << date << " -> " << std::fixed << std::setprecision(2)
                      << value << " = " << it->second * value << std::endl;
        }
        catch (const std::exception &) {
            continue;
        }
    }
    file.close();
    return true;
}


void BitcoinExchange::execute(const std::string &inputFile) {
    bool goodFile = parseInputFile(inputFile);
    if (!goodFile)
        return;
}