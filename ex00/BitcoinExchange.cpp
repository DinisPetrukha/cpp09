#include "BitcoinExchange.hpp"

// Orthodox Canonical Form
BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) {
	*this = other;
}

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
	if (this != &other) {
		this->btc_data = other.btc_data;
	}
	return *this;
}

// Helper methods
bool BitcoinExchange::isAllDigits(const std::string &str) const {
	if (str.empty())
		return false;

	for (size_t i = 0; i < str.size(); ++i) {
		if (!isdigit(str[i]) && str[i] != '.') {
			return false;
		}
	}
	return true;
}

bool BitcoinExchange::isValidDate(const std::string &date) const {
	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		return false;

	std::istringstream iss(date);
	int year, month, day;
	char dash1, dash2;

	if (!(iss >> year >> dash1 >> month >> dash2 >> day) || dash1 != '-' || dash2 != '-')
		return false;

	if (year < 2009 || month < 1 || month > 12 || day < 1 || day > 31)
		return false;

	if (month == 4 || month == 6 || month == 9 || month == 11) {
		if (day > 30) return false;
	} else if (month == 2) {
		bool isLeap = (year % 400 == 0) || (year % 100 != 0 && year % 4 == 0);
		if (day > (isLeap ? 29 : 28)) return false;
	}

	return true;
}

int BitcoinExchange::isValidValue(const std::string &value) const {
	if (value[0] == '-')
		return std::cout << "Error: not a positive number." << std::endl, 2;

	if (!isAllDigits(value))
		return 1;

	float num = std::atof(value.c_str());
	if (num > 1000)
		return std::cout << "Error: too large a number" << std::endl, 3;

	return 0;
}

float BitcoinExchange::calculateResult(const std::string &date, const std::string &value) const {
	std::map<std::string, float>::const_iterator it = btc_data.upper_bound(date);
	float float_value = atof(value.c_str());

	if (it == btc_data.begin())
		return it->second * float_value;

	if (it == btc_data.end())
		return btc_data.rbegin()->second * float_value;

	--it;
	return it->second * float_value;
}

// Main functionality
void BitcoinExchange::loadDatabase(const std::string &filename) {
	std::ifstream data_file(filename.c_str());
	if (!data_file.is_open()) {
		throw std::runtime_error("Error: could not open data file.");
	}

	std::string line;
	getline(data_file, line); // ignore header

	while (getline(data_file, line)) {
		size_t delimiter = line.find(',');
		std::string date = line.substr(0, delimiter);
		std::string value = line.substr(delimiter + 1);
		btc_data.insert(std::make_pair(date, std::atof(value.c_str())));
	}
}

void BitcoinExchange::processInput(const std::string &filename) const {
	std::ifstream input_file(filename.c_str());
	if (!input_file.is_open()) {
		throw std::runtime_error("Error: could not open input file.");
	}

	std::string line;
	getline(input_file, line); // ignore header

	while (getline(input_file, line)) {
		if (line.length() < 13 || line[10] != ' ' || line[11] != '|' || line[12] != ' ') {
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		size_t delimiter = line.find('|');
		if (delimiter == std::string::npos) {
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		std::string input_date = line.substr(0, delimiter);
		std::string input_value = line.substr(delimiter + 1);

		// Trim whitespace
		input_date.erase(input_date.find_last_not_of(" \t") + 1);
		input_date.erase(0, input_date.find_first_not_of(" \t"));
		input_value.erase(input_value.find_last_not_of(" \t") + 1);
		input_value.erase(0, input_value.find_first_not_of(" \t"));

		if (!isValidDate(input_date)) {
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		int is_valid_value = isValidValue(input_value);
		if (is_valid_value == 1) {
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		} else if (is_valid_value == 2 || is_valid_value == 3) {
			continue;
		}

		float result = calculateResult(input_date, input_value);
		std::cout << input_date << " => " << input_value << " = " << result << std::endl;
	}
}