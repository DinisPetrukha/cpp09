#ifndef RPN_HPP
# define RPN_HPP

#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstdlib>

class RPN {
private:
	std::stack<int> _stack;

	int		calculateExpression(int	a, int b, std::string token);
	bool	isOperator(const std::string token);
public:

	RPN();
	RPN(const RPN& other);
	RPN& operator=(const RPN& other);
	~RPN();

	int	evaluateRPN(const std::string &expression);
};

# endif