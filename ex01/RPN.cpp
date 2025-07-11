#include "RPN.hpp"

RPN::RPN() : _stack() {};

RPN::RPN(const RPN& other) {
	*this = other;
}

RPN& RPN::operator=(const RPN& other) {
	if (this != &other)
		this->_stack = other._stack;
	return *this;
}

RPN::~RPN() {}

bool	RPN::isOperator(const std::string token)
{
	return token == "*" || token == "/" || token == "+" || token == "-";
}

int	RPN::calculateExpression(int	a, int b, std::string token)
{
	if (token == "*")
		return a * b;
	else if (token == "/") {
		if (b == 0)
			throw std::runtime_error("Error: divided by zero!");
		return a / b;
	}
	else if (token == "+")
		return a + b;
	else if (token == "-")
		return a - b;
	throw std::runtime_error("Error: Invalid operator!");
}

int	RPN::evaluateRPN(const std::string &expression)
{
	// std::stack<int> stack;
	std::istringstream iss(expression);
	std::string	token;

	while (iss >> token) {
		if (isdigit(token[0]))
			_stack.push(std::atoi(token.c_str()));
		else if (isOperator(token)) {
			if (_stack.size() < 2)
				throw std::runtime_error("Error: Not sufficient operands");

			int	b = _stack.top(); _stack.pop();
			int	a = _stack.top(); _stack.pop();

			_stack.push(calculateExpression(a, b, token));
		}
		else
			throw std::runtime_error("Error");
	}
	if (_stack.size() != 1)
		throw std::runtime_error("Error: too many operands");
	return _stack.top();
}