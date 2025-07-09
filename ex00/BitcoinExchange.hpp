#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstdlib>
#include <sstream>
#include <algorithm>

class BitcoinExchange {
private:
	std::map<std::string, float> btc_data;

	// Private helper methods
	bool isAllDigits(const std::string &str) const;
	bool isValidDate(const std::string &date) const;
	int isValidValue(const std::string &value) const;
	float calculateResult(const std::string &date, const std::string &value) const;

public:
	// Orthodox Canonical Form
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &other);
	~BitcoinExchange();
	BitcoinExchange &operator=(const BitcoinExchange &other);

	// Main functionality
	void loadDatabase(const std::string &filename);
	void processInput(const std::string &filename) const;
};

#endif