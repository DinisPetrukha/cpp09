#include "PmergeMe.hpp"

int main(int argc, char *argv[]) {
	if (argc <= 2) {
		std::cerr << "Usage: " << argv[0] << " <values>" << std::endl;
		return 1;
	}
	try {
		PmergeMe sorter(argc, argv);
		sorter.sort();
	} catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return 0;
}