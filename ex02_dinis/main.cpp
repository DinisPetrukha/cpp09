#include "PmergeMe.hpp"
#include <ctime>
#include <sys/time.h>

int main(int argc, char* argv[]) {
	if (argc <= 2) {
		std::cerr << "Usage: " << argv[0] << " <values>" << std::endl;
		return 1;
	}

	SortList pList(argv, argc);

	pList.printList();

	pList.sort(1);

	pList.printList();
};