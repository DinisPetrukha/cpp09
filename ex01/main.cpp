#include "RPN.hpp"

bool	isOperator(const std::string token)
{
	return token == "*" || token == "/" || token == "+" || token == "-";
}

int	calculateExpression(int	a, int b, std::string token)
{
	if (token == "*")
		return a * b;
	else if (token == "/") {
		if (b == 0 || a == 0)
			throw std::runtime_error("Error: divised by zero!");
		return a / b;
	}
	else if (token == "+")
		return a + b;
	else if (token == "-")
		return a - b;
	throw std::runtime_error("Error: Invalid operator!");
}

int	evaluateRPN(const std::string &expression)
{
	std::stack<int> stack;
	std::istringstream iss(expression);
	std::string	token;

	while (iss >> token) {
		if (isdigit(token[0]))
			stack.push(std::atoi(token.c_str()));
		else if (isOperator(token)) {
			if (stack.size() < 2)
				throw std::runtime_error("Error: Not sufficient operands");

			int	b = stack.top(); stack.pop();
			int	a = stack.top(); stack.pop();

			stack.push(calculateExpression(a, b, token));
		}
		else
			throw std::runtime_error("Error");
	}
	if (stack.size() != 1)
		throw std::runtime_error("Error: too many operands");
	return stack.top();
}

int	main(int argc, char *argv[]){
	if (argc != 2)
		std::cerr << "Usage: " << argv[0] << "\"RPN expression\"" << std::endl;
	try {
		int result = evaluateRPN(argv[1]);
		std::cout << result << std::endl;
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}