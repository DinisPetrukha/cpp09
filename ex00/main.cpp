#include "BitcoinExchange.hpp"

bool isAllDigits(const std::string &str) {
	if (str.empty())
		return false;

	for (size_t i = 0; i < str.size(); ++i) {
		if (!isdigit(str[i]) && str[i] != '.') {
			return false;
		}
	}
	return true;
}

bool isValidDate(const std::string &date) {
	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		return false;

	std::istringstream iss(date);
	int year, month, day;
	char dash1, dash2;

	if (!(iss >> year >> dash1 >> month >> dash2 >> day) || dash1 != '-' || dash2 != '-')
		return false;

	if (year < 2009 || month < 1 || month > 12 || day < 1 || day > 31)
		return false;

	// Verificação avançada dos dias por mês
	if (month == 4 || month == 6 || month == 9 || month == 11) {
		if (day > 30) return false;
	} else if (month == 2) {
		bool isLeap = (year % 400 == 0) || (year % 100 != 0 && year % 4 == 0);
		if (day > (isLeap ? 29 : 28)) return false;
	}

	return true;
}
//1004 x-12 fsd23x
int isValidValue(const std::string &value)
{
	if (value[0] == '-')
		return std::cout << "Error: not a positive number." << std::endl, 2;

	if (!isAllDigits(value))
		return 1;

	float num = std::atof(value.c_str());
	if (num > 1000)
		return std::cout << "Error: too large a number" << std::endl, 3;
	return (0);
}

float returnBtcResult(std::string &date, std::string &value,
					std::map<std::string, float> &btc_data)
{
	std::map<std::string, float>::iterator it = btc_data.upper_bound(date);
	std::cout << it->second << std::endl;
	float float_value = atof(value.c_str());

	if (it == btc_data.begin())
		return it->second * float_value;

	if (it == btc_data.end()) {
		return btc_data.rbegin()->second * float_value;
	}
	--it;
	return it->second * float_value;
}

int	main(int argc, char* argv[]) {

	std::ifstream	input_file;
	std::ifstream	data_file;

	// Second argument protection
	if (argc < 2) {
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}

	// Opening the input file
	input_file.open(argv[1]);
	if (!input_file.is_open()){
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}

	// Opening the dataset file
	data_file.open("data.csv");
	if (!input_file.is_open()){
		std::cerr << "Error: could not open data file." << std::endl;
		return 1;
	}

	std::string	line;
	std::map<std::string, float> btc_data;

	getline(data_file, line); // ignoring the headers "date,exchange_rate"

	// Inserting all the btc dataset into the btc_data map container
	while (getline(data_file, line)){
		std::string	date, value;

		// Parsing the date and value
		size_t delimiter = line.find(',');
		date = line.substr(0, delimiter);
		value = line.substr(delimiter + 1);

		// Inserting into the container
		btc_data.insert(std::make_pair(date, std::atof(value.c_str()))); // converting string to char* -> to convert to float -> to put on make_pair -> to insert in the container
		// std::cout << btc_data[date] << std::endl;
	}

	getline(input_file, line); // ignoring the headers "date | value"

	// Aqui Começa o Loop Linha a Linha do Input
	while (getline(input_file, line))
	{
		if (line[10] != ' ' || line[11] != '|' || line[12] != ' ')
		{
			std::cout << "1 Error: bad input => " << line << std::endl;
			continue;
		}

		size_t delimiter = line.find('|');

		if (delimiter == std::string::npos){
			std::cout << "2 Error: bad input => " << line << std::endl;
			continue;
		}

		std::string	input_date, input_value;
		input_date = line.substr(0, delimiter);
		input_value = line.substr(delimiter + 1);

		// Clean the spaces
		input_date.erase(input_date.find_last_not_of(" \t") + 1);
		input_date.erase(0, input_date.find_first_not_of(" \t"));
		input_value.erase(input_value.find_last_not_of(" \t") + 1);
		input_value.erase(0, input_value.find_first_not_of(" \t"));

		// Date protection
		if (!isValidDate(input_date))
		{
			std::cout << "4 Error: bad input => " << line << std::endl;
			continue ;
		}

		// Value Protection
		int	is_valid_value = isValidValue(input_value);
		if (is_valid_value == 1)
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue ;
		}
		else if (is_valid_value == 2 || is_valid_value == 3)
			continue ;

		// Find the equivalent date
		float result = returnBtcResult(input_date, input_value, btc_data);
		std::cout << input_date << " => " << input_value << " = " << result << std::endl;
	}

	return 0;
}