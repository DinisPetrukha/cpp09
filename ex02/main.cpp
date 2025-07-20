#include "PmergeMe.hpp"

bool	is_valid_argument(char *argv[], int argc) {
	for (int i = 1; i < argc; ++i) {
		for (char *c = argv[i]; *c; ++c) {
			if (!std::isdigit(*c))
				return false;
		}
	}
	return true;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <numbers>" << std::endl;
		return 1;
	}

	if (!is_valid_argument(argv, argc)) {
		std::cerr << "Error: All arguments must be positive integers." << std::endl;
		return 1;
	}

	PmergeMe container(argv);
	container.print_vector();
}