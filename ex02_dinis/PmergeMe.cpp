#include "PmergeMe.hpp"

SortList::SortList() {}

SortList::SortList(char * argv[], int argc) {
	for (int i = 1; i < argc; ++i) {
			std::string arg = argv[i];
			std::stringstream iss(arg);
			int val;

			if (!(iss >> val) || !iss.eof()) {
				std::cerr << "Error: " << argv[i] << " invalid integer" << std::endl;
				throw std::invalid_argument("Conversion string to int failed");
			}

			if (val < 0)
				throw std::runtime_error("Error: Negative numbers are not allowed.");

			_list.push_back(val);
	}
}

std::list<ListChunk>::iterator SortList::mainChainIter(size_t index)
{
	std::list<ListChunk>::iterator it(_main_chain.begin());

	std::advance(it, index);
	return (it);
}

inline  size_t  SortList::getIndexSend(size_t curr_jacob) const
{
	return (std::min(curr_jacob - 2, (_pend.size() - 1)));
}

int SortList::lastElement(size_t index) const
{
	std::list<ListChunk>::const_iterator it(_main_chain.begin());

	std::advance(it, index);

	std::list<int>::iterator chunk = (it->end);
	--chunk;
	return (*chunk);
}

inline  size_t  SortList::getSearchArea() const
{
    return (std::min(static_cast<size_t>(pow(2, _jacob_index - 1) - 1),
            _main_chain.size()));
}

void SortList::copyMainChain()
{
	std::list<int>                      sorted_numbers;
	std::list<ListChunk>::iterator      end;

	end = _main_chain.end();
	for (std::list<ListChunk>::iterator it = _main_chain.begin(); it != end; ++it)
	{
		for (std::list<int>::iterator st = it->begin; st != it->end;
				++st)
		{
			sorted_numbers.push_back(*st);
		}
	}
	push_back_leftovers(sorted_numbers, _list);
	_list = sorted_numbers;
}

void SortList::sort(size_t depth) {
	size_t chunk_size; // quantidade de numeros em cada par;
	size_t n_pairs;
	std::list<int>::iterator first_chunk;
	std::list<int>::iterator second_chunk;

	std::cout << "Recursion Depth: " << depth << std::endl;
	chunk_size = pow(2, depth - 1);
	n_pairs = _list.size() / (chunk_size * 2);
	if (n_pairs == 0)
		return ;
	first_chunk = _list.begin();
	second_chunk = first_chunk;

	// Moving the position of second chunk
	std::advance(second_chunk, chunk_size);

	for (size_t n_comparisons = 0; n_comparisons < n_pairs ; ++n_comparisons) {
		if (chunkLastElement(first_chunk, chunk_size) > //Comparing last chunk of each element
			chunkLastElement(second_chunk, chunk_size))
				chunkSwap(first_chunk, second_chunk, chunk_size);
		std::advance(first_chunk, chunk_size * 2); //Advancing comparison elements
		std::advance(second_chunk, chunk_size * 2);
	}

	sort(depth + 1); // Creating elements with bigger chunk size comparisons

	size_t total_numbers = _list.size();

	// Extracting how many pairs we have for the main chain subtracting the pend of the current recursion level.
	n_pairs = total_numbers - total_numbers % chunk_size;

	if (n_pairs > 3 || total_numbers == 3) {
		initPend(chunk_size, n_pairs);
		mergePend();
	}
}

void SortList::initPend(size_t chunk_size, size_t n_chunck) {
	std::list<int>::iterator begin = _list.begin();
	std::list<int>::iterator end = begin;
	ListChunk s_chunk;
	bool is_pend;

	is_pend = true;
	std::advance(end, chunk_size);
	for (size_t i = 0; i < n_chunck; i += chunk_size) {
		s_chunk.begin = begin;
		s_chunk.end = end;

		if (is_pend && i)
			_pend.push_back(s_chunk);
		else
			_main_chain.push_back(s_chunk);

		is_pend = !is_pend;
		std::advance(begin, chunk_size);
		std::advance(end, chunk_size);
	}
}

void SortList::insertChunk(size_t chunks_to_send, size_t to_send_index, size_t search_area)
{
	int max;
	std::list<ListChunk>::iterator it;
	std::list<ListChunk>::iterator to_insert_it;
	std::list<int>::iterator st;

	for (size_t chunks_sent = 0; chunks_sent < chunks_to_send; ++chunks_sent)
	{
		to_insert_it = _pend.begin();
		std::advance(to_insert_it, to_send_index);
		ListChunk& to_insert = *to_insert_it;
		st = to_insert_it->end;
		std::advance(st, -1);
		max = *(st);
		it = binarySearch(max, search_area);
		if (it == _main_chain.end())
			_main_chain.push_back(to_insert);
		else
			_main_chain.insert(it, to_insert);
		to_send_index -= 1;
	}
}

inline size_t   calc_jacob_num(size_t n)
{
	return ((((pow(2, n) + (((n - 1) & 1) == 0 ? 1 : -1)) / 3)));
}

std::list<ListChunk>::iterator SortList::binarySearch(int max, size_t search_area)
{
	size_t min_index;
	size_t max_index;
	size_t mid;
	size_t main_chain_size;
	int chunk_max;
	int next_chunk_max;

	min_index = 0;
	max_index = (search_area - 1);
	main_chain_size = _main_chain.size();
	while (max_index >= min_index)
	{
		mid = min_index + (max_index - min_index) / 2;
		if (mid == main_chain_size - 1)
			return (mainChainIter(mid + 1));
		chunk_max = lastElement(mid);
		if (max > chunk_max)
		{
			next_chunk_max = lastElement(mid + 1);
			if (max < next_chunk_max)
				return (mainChainIter(mid + 1));
			min_index = mid + 1;
		}
		else
		{
			if (mid == 0)
				return (_main_chain.begin());
			max_index = mid - 1;
		}
	}
	return (mainChainIter(mid + 1));
}

void SortList::mergePend() {
	size_t previous_jacob;
	size_t current_jacob;
	size_t chunks_to_send;
	size_t pend_size;

	// Starting jacob numbers
	_jacob_index = 3;
	previous_jacob = 1;
	current_jacob = 3;
	pend_size = _pend.size();

	while (true) {
		chunks_to_send = std::min(current_jacob - previous_jacob, pend_size);
		insertChunk(chunks_to_send, getIndexSend(current_jacob), getSearchArea());
		if (pend_size <= 0)
			break;
		_jacob_index++;
		previous_jacob = current_jacob;
		current_jacob = calc_jacob_num(_jacob_index);
	}
	copyMainChain();
	_main_chain.clear();
	_pend.clear();
}

void SortList::printList() {
	std::cout << "-- Current List --" << std::endl;

	for (std::list<int>::iterator it = _list.begin(); it != _list.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

