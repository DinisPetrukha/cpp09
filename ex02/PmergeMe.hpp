#ifndef PMERGEME_HPP
# define RPNPMERGEME_HPP_HPP

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstdlib>

class PmergeMe {
private:
	std::vector<int>	_vector;

	std::vector<int>	argv_to_vector(char *argv[]);
	void				vector_sort();
	template <typename T> void _merge_insertion_sort(T& container, int pair_level);

public:
	PmergeMe();
	PmergeMe(char *argv[]);
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);
	~PmergeMe();

	std::vector<int> get_vector() const { return _vector; };
	std::size_t size() const { return _vector.size(); };

	void	print_vector() const;
	void	sort_vector(std::vector<int>& vec) { _merge_insertion_sort<std::vector<int> >(vec, 1);}
};

template <typename T> void PmergeMe::_merge_insertion_sort(T& container, int pair_level)
{
	typedef typename T::iterator Iterator;

	int pair_units_nbr = container.size() / pair_level;
	if (pair_units_nbr <= 1)
		return;

	bool is_odd = pair_units_nbr % 2 == 1;
	Iterator start = container.begin();
	//next returns an iterator pointing to the element after being advanced by certain no. of positions
	Iterator last = next(container.begin(), pair_level * (pair_units_nbr));
	Iterator end = next(last, -(is_odd * pair_level));

	int jump = 2 * pair_level;
	for (Iterator it = start; it != end; std::advance(it, jump))
	{
		Iterator this_pair = next(it, pair_level - 1);
		Iterator next_pair = next(it, pair_level * 2 - 1);
		if (_comp(next_pair, this_pair))
			_swap_pair(this_pair, next_pair);
	}
	_merge_insertion_sort(container, pair_level * 2);
}

# endif