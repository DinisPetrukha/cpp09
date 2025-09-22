#include "PmergeMe.hpp"

VecSorter::VecSorter()
{
    the_numbers.reserve(50);
    the_pend.reserve(50);
}
VecSorter::VecSorter(const VecSorter& sorter) : the_numbers(sorter.the_numbers),
                                                the_pend(sorter.the_pend),
                                                the_main_chain(sorter.the_main_chain)
{
    jacob_index = 3;
}

VecSorter& VecSorter::operator=(const VecSorter& other)
{
    if (this != &other)
    {
        the_numbers = other.the_numbers;
        the_pend = other.the_pend;
        the_main_chain = other.the_main_chain;
        jacob_index = other.jacob_index;
    }
    return (*this);
}

VecSorter::VecSorter(char* str)
{
    int duplicate;

    the_numbers.reserve(50);
    the_pend.reserve(50);
    the_main_chain.reserve(50);

    try
    {
        splitArg(the_numbers, str);
    }
    catch(const std::exception& e)
    {
        throw (e);
    }

    duplicate = are_duplicate_present(the_numbers);
    if (duplicate != -1)
    {
        throw (std::runtime_error("Error duplicate number present"));
    }
}

VecSorter::VecSorter(char** argv)
{
    int duplicate;

    the_numbers.reserve(50);
    the_pend.reserve(50);
    the_main_chain.reserve(50);

    for (int i = 1; argv[i]; ++i)
    {
        the_numbers.push_back(std::atoi(argv[i]));
    }

    duplicate = are_duplicate_present(the_numbers);
    if (duplicate != -1)
    {
        throw (std::runtime_error("Error duplicate number present"));
    }
}

void VecSorter::sort(size_t depth)
{
    size_t total_numbers;
    size_t chunk_size;
    size_t pairs;
    std::vector<int>::iterator chunk_one_begin;
    std::vector<int>::iterator chunk_two_begin;

    chunk_size = pow(2, depth - 1);
    pairs = the_numbers.size() / (chunk_size * 2);
    if (pairs == 0)
        return;
    chunk_one_begin = the_numbers.begin();
    chunk_two_begin = chunk_one_begin + chunk_size;

    for (size_t to_compare = 0; to_compare < pairs; ++to_compare)
    {
        if (last_element_of_chunk(chunk_one_begin, chunk_size) >
            last_element_of_chunk(chunk_two_begin, chunk_size))
            swap_chunk(chunk_one_begin, chunk_two_begin, chunk_size);
        chunk_one_begin += (chunk_size * 2);
        chunk_two_begin = chunk_one_begin + chunk_size;
    }
    sort(depth + 1); //recursevly sort until there are no pairs left
    total_numbers = the_numbers.size();
    pairs = total_numbers - total_numbers % chunk_size;
    if (pairs > 3 || the_numbers.size() == 3)
    {
        init_the_pend(chunk_size, pairs);
        merge_the_pend();
    }
}

void VecSorter::init_the_pend(size_t chunk_size, size_t chunks)
{
    std::vector<int>::iterator  num_begin;
    VecChunk                    chunk;
    bool                        belongs_to_pend;

    belongs_to_pend = true;
    num_begin = the_numbers.begin();
    for (size_t i = 0; i < chunks; i += chunk_size)
    {
        chunk.begin = num_begin + i;
        chunk.end = chunk.begin + chunk_size;
        if (belongs_to_pend && i)
            the_pend.push_back(chunk);
        else
            the_main_chain.push_back(chunk);
        belongs_to_pend = !belongs_to_pend;
    }
}

inline size_t   calc_jacob_num(size_t n)
{
    return ((((pow(2, n) + (((n - 1) & 1) == 0 ? 1 : -1)) / 3)));
}

inline  size_t  VecSorter::get_search_area() const
{
    return (std::min(static_cast<size_t>(pow(2, jacob_index - 1) - 1),
            the_main_chain.size()));
}

inline  size_t  VecSorter::get_index_to_send(size_t curr_jacob) const
{
    return (std::min(curr_jacob - 2, (the_pend.size() - 1)));
}

void VecSorter::merge_the_pend()
{
    size_t  previous_jacob;
    size_t  current_jacob;
    size_t  chunks_to_send;
    size_t  pend_size;

    jacob_index = 3;
    previous_jacob = 1;
    current_jacob = 3;
    pend_size = the_pend.size();

    while (true)
    {
        chunks_to_send = std::min(current_jacob - previous_jacob, pend_size);
        insert_chunk(chunks_to_send, get_index_to_send(current_jacob), get_search_area());
        pend_size -= chunks_to_send;
        if (pend_size <= 0)
            break;
        jacob_index++;
        previous_jacob = current_jacob;
        current_jacob = calc_jacob_num(jacob_index);
    }
    copy_the_main_chain();
    the_main_chain.clear();
    the_pend.clear();
}

void VecSorter::insert_chunk(size_t chunks_to_send, size_t to_send_index, size_t search_area)
{
    int     max;
    std::vector<VecChunk>::iterator it;

    for (size_t chunks_sent = 0; chunks_sent < chunks_to_send; ++chunks_sent)
    {
        VecChunk& to_insert = the_pend.at(to_send_index);
        max = *(to_insert.end - 1);
        it = binary_search_pos(max, search_area);
        if (it == the_main_chain.end())
            the_main_chain.push_back(to_insert);
        else
            the_main_chain.insert(it, to_insert);
        to_send_index -= 1;
    }
}

std::vector<VecChunk>::iterator VecSorter::binary_search_pos(int max, size_t search_area)
{
    size_t  min_index;
    size_t  max_index;
    size_t  mid;
    size_t  main_chain_size;
    int     chunk_max;
    int     next_chunk_max;

    min_index = 0;
    max_index = (search_area - 1);
    main_chain_size = the_main_chain.size();
    while (max_index >= min_index)
    {
        mid = min_index + (max_index - min_index) / 2;
        if (mid == main_chain_size - 1)
            return (the_main_chain.begin() + (mid + 1));
        chunk_max = *(the_main_chain.at(mid).end - 1);
        if (max > chunk_max)
        {
            next_chunk_max = *(the_main_chain.at(mid + 1).end - 1);
            if (max < next_chunk_max)
                return (the_main_chain.begin() + (mid + 1));
            min_index = mid + 1;
        }
        else
        {
            if (mid == 0)
                return (the_main_chain.begin());
            max_index = mid - 1;
        }
    }
    return (the_main_chain.begin() + (mid + 1));
}

void VecSorter::copy_the_main_chain()
{
    std::vector<int>                sorted_numbers;
    std::vector<VecChunk>::iterator    end;

    sorted_numbers.reserve(the_numbers.size());
    end = the_main_chain.end();
    for (std::vector<VecChunk>::iterator it = the_main_chain.begin(); it != end; ++it)
    {
        for (std::vector<int>::iterator st = it->begin; st != it->end;
                ++st)
        {
            sorted_numbers.push_back(*st);
        }
    }
    push_back_leftovers(sorted_numbers, the_numbers);
    the_numbers = sorted_numbers;
}

void VecSorter::print_numbers() const
{
    std::vector<int>::const_iterator last = the_numbers.end() - 1;

    for (std::vector<int>::const_iterator it = the_numbers.begin(); it != last; ++it)
    {
        std::cout << *it;
        std::cout << " ";
    }
    std::cout << *last << "\n";
}

const std::vector<int>& VecSorter::get_numbers() const
{
    return (the_numbers);
}

std::vector<int>::const_iterator VecSorter::numbers_begin() const
{
    return (the_numbers.begin());
}

std::vector<int>::const_iterator VecSorter::numbers_end() const
{
    return (the_numbers.end());
}

std::vector<VecChunk>::const_iterator VecSorter::pend_begin() const
{
    return(the_pend.begin());
}

std::vector<VecChunk>::const_iterator VecSorter::pend_end() const
{
    return(the_pend.end());
}

std::vector<VecChunk>::const_iterator VecSorter::chain_begin() const
{
    return (the_main_chain.begin());
}

std::vector<VecChunk>::const_iterator VecSorter::chain_end() const
{
    return (the_main_chain.end());
}

std::ostream& operator<<(std::ostream& os, const VecSorter& v)
{
    os << "\t\tNumbers Vector\n";
    os << "****************************************\n";
    for (std::vector<int>::const_iterator it = v.numbers_begin();
            it != v.numbers_end(); ++it)
    {
        os << *it;
        if (it != v.numbers_end() - 1)
            std::cout << " ";
    }
    os << "\n****************************************\n";
    os << "\t\tMain chain\n";
    os << "****************************************\n";
    for (std::vector<VecChunk>::const_iterator it = v.chain_begin();
        it != v.chain_end(); ++it)
    {
        std::cout <<"[";
        for (std::vector<int>::iterator st = it->begin; st != it->end;
            ++st)
        {
            os << *st;
            if (st != (it->end - 1))
                std::cout << " ";
        }
        std::cout <<"]";
    }
    os << "\n****************************************\n";
    os << "\t\tThe pend\n";
    os << "****************************************\n";
    for (std::vector<VecChunk>::const_iterator it = v.pend_begin();
            it != v.pend_end(); ++it)
    {
        std::cout << "[";
        for (std::vector<int>::iterator st = it->begin; st != it->end;
            ++st)
        {
            os << *st;
            if (st != (it->end - 1))
                std::cout << " ";
        }
        std::cout << "]";
    }
    os << "\n****************************************\n";
    return (os << std::endl);
}


// --------------------------------------------------- --------------------------------------------------- ---------------------------------------------------
// --------------------------------------------------- --------------------------------------------------- ---------------------------------------------------
// --------------------------------------------------- --------------------------------------------------- ---------------------------------------------------


ListSorter::ListSorter()
{
}

ListSorter::ListSorter(char* str)
{
    int duplicate;

    try
    {
        splitArg(list, str);
    }
    catch(const std::exception& e)
    {
        throw (e);
    }

    duplicate = are_duplicate_present(list);
    if (duplicate != -1)
    {
        throw (std::runtime_error("Error duplicate number present"));
    }
    jacob_index = 3;
}

ListSorter::ListSorter(char** argv)
{
    int duplicate;

    for (int i = 1; argv[i]; ++i)
    {
        list.push_back(std::atoi(argv[i]));
    }

    duplicate = are_duplicate_present(list);
    if (duplicate != -1)
    {
        throw (std::runtime_error("Error duplicate number present"));
    }
    jacob_index = 3;
}

ListSorter::ListSorter(const ListSorter& sorter) :  list(sorter.list),
                                                    the_pend(sorter.the_pend),
                                                    the_main_chain(sorter.the_main_chain)
{
    jacob_index = sorter.jacob_index;
}

ListSorter& ListSorter::operator=(const ListSorter& other)
{
    if (this != &other)
    {
        list = other.list;
        the_pend = other.the_pend;
        the_main_chain = other.the_main_chain;
        jacob_index = other.jacob_index;
    }
    return (*this);
}

void ListSorter::sort(size_t depth)
{
    size_t  chunk_size;
    size_t  pairs;
    size_t  total_numbers;
    std::list<int>::iterator chunk_one_begin;
    std::list<int>::iterator chunk_two_begin;

    // the list = 3 1 7 8 10 2
    chunk_size = pow(2, depth - 1); // 2 ^ 0 = 1 tamanho chunk
    pairs = list.size() / (chunk_size * 2); // 6 / (1 * 2) = 3 pares
    if (pairs == 0)
        return;
    chunk_one_begin = list.begin(); // (3) 1 7 8 10 2
    chunk_two_begin = chunk_one_begin;
    std::advance(chunk_two_begin, chunk_size); // 3 (1) 7 8 10 2

    for (size_t to_compare = 0; to_compare < pairs; ++to_compare)
    {
        if (last_element_of_chunk(chunk_one_begin, chunk_size) >
            last_element_of_chunk(chunk_two_begin, chunk_size)) // (3) > (1)
            swap_chunk(chunk_one_begin, chunk_two_begin, chunk_size); // swap = 1 3 7 8 10 2
        std::advance(chunk_one_begin, chunk_size * 2); // 1 3 (7) 8 10 2
        std::advance(chunk_two_begin, chunk_size * 2); // 1 3 7 (8) 10 2
    }
    sort(depth + 1); //recursevly sort until there are no pairs left



    total_numbers = list.size();
    pairs = total_numbers - total_numbers % chunk_size;
    if (pairs > 3 || total_numbers == 3)
    {
        init_the_pend(chunk_size, pairs);
        merge_the_pend();
    }
}

void ListSorter::init_the_pend(size_t chunk_size, size_t chunks)
{
    std::list<int>::iterator    num_begin(list.begin());
    std::list<int>::iterator    num_end(num_begin);
    ListChunk                   chunk;
    bool                        belongs_to_pend;

    belongs_to_pend = true;
    std::advance(num_end, chunk_size);
    for (size_t i = 0; i < chunks; i += chunk_size)
    {
        chunk.begin = num_begin;
        chunk.end = num_end;
        if (belongs_to_pend && i)
            the_pend.push_back(chunk);
        else
            the_main_chain.push_back(chunk);
        belongs_to_pend = !belongs_to_pend;
        std::advance(num_begin, chunk_size);
        std::advance(num_end, chunk_size);
    }
}

inline  size_t  ListSorter::get_search_area() const
{
    return (std::min(static_cast<size_t>(pow(2, jacob_index - 1) - 1),
            the_main_chain.size()));
}

inline  size_t  ListSorter::get_index_to_send(size_t curr_jacob) const
{
    return (std::min(curr_jacob - 2, (the_pend.size() - 1)));
}

void ListSorter::merge_the_pend()
{
    size_t  previous_jacob;
    size_t  current_jacob;
    size_t  chunks_to_send;
    size_t  pend_size;

    jacob_index = 3;
    previous_jacob = 1;
    current_jacob = 3;
    pend_size = the_pend.size();

    while (true)
    {
        chunks_to_send = std::min(current_jacob - previous_jacob, pend_size);
        insert_chunk(chunks_to_send, get_index_to_send(current_jacob), get_search_area());
        pend_size -= chunks_to_send;
        if (pend_size <= 0)
            break;
        jacob_index++;
        previous_jacob = current_jacob;
        current_jacob = calc_jacob_num(jacob_index);
    }
    copy_the_main_chain();
    the_main_chain.clear();
    the_pend.clear();
}

void ListSorter::insert_chunk(size_t chunks_to_send, size_t to_send_index, size_t search_area)
{
    int                             max;
    std::list<ListChunk>::iterator  it;
    std::list<ListChunk>::iterator  to_insert_it;
    std::list<int>::iterator        st;

    for (size_t chunks_sent = 0; chunks_sent < chunks_to_send; ++chunks_sent)
    {
        to_insert_it = the_pend.begin();
        std::advance(to_insert_it, to_send_index);
        ListChunk& to_insert = *to_insert_it;
        st = to_insert_it->end;
        std::advance(st, -1);
        max = *(st);
        it = binary_search_pos(max, search_area);
        if (it == the_main_chain.end())
            the_main_chain.push_back(to_insert);
        else
            the_main_chain.insert(it, to_insert);
        to_send_index -= 1;
    }
}

int ListSorter::last_elem_at(size_t index) const
{
    std::list<ListChunk>::const_iterator it(the_main_chain.begin());

    std::advance(it, index);

    std::list<int>::iterator chunk = (it->end);
    --chunk;
    return (*chunk);
}

std::list<ListChunk>::iterator ListSorter::main_chain_iter(size_t index)
{
    std::list<ListChunk>::iterator it(the_main_chain.begin());

    std::advance(it, index);
    return (it);
}

std::list<ListChunk>::iterator ListSorter::binary_search_pos(int max, size_t search_area)
{
    size_t  min_index;
    size_t  max_index;
    size_t  mid;
    size_t  main_chain_size;
    int     chunk_max;
    int     next_chunk_max;

    min_index = 0;
    max_index = (search_area - 1);
    main_chain_size = the_main_chain.size();
    while (max_index >= min_index)
    {
        mid = min_index + (max_index - min_index) / 2;
        if (mid == main_chain_size - 1)
            return (main_chain_iter(mid + 1));
        chunk_max = last_elem_at(mid);
        if (max > chunk_max)
        {
            next_chunk_max = last_elem_at(mid + 1);
            if (max < next_chunk_max)
                return (main_chain_iter(mid + 1));
            min_index = mid + 1;
        }
        else
        {
            if (mid == 0)
                return (the_main_chain.begin());
            max_index = mid - 1;
        }
    }
    return (main_chain_iter(mid + 1));
}

void ListSorter::copy_the_main_chain()
{
    std::list<int>                      sorted_numbers;
    std::list<ListChunk>::iterator      end;

    end = the_main_chain.end();
    for (std::list<ListChunk>::iterator it = the_main_chain.begin(); it != end; ++it)
    {
        for (std::list<int>::iterator st = it->begin; st != it->end;
                ++st)
        {
            sorted_numbers.push_back(*st);
        }
    }
    push_back_leftovers(sorted_numbers, list);
    list = sorted_numbers;
}

void ListSorter::print_numbers() const
{
    std::list<int>::const_iterator last = list.end();

    std::advance(last, -1);

    for (std::list<int>::const_iterator it = list.begin(); it != last; ++it)
    {
        std::cout << *it;
        std::cout << " ";
    }
    std::cout << *last << "\n";
}

const std::list<int>& ListSorter::get_numbers() const
{
    return (list);
}

std::list<int>::const_iterator ListSorter::list_begin() const
{
    return (list.begin());
}

std::list<int>::const_iterator ListSorter::list_end() const
{
    return (list.end());
}

std::list<ListChunk>::const_iterator ListSorter::pend_begin() const
{
    return (the_pend.begin());
}

std::list<ListChunk>::const_iterator ListSorter::pend_end() const
{
    return (the_pend.end());
}

std::list<ListChunk>::const_iterator ListSorter::chain_begin() const
{
    return (the_main_chain.begin());
}

std::list<ListChunk>::const_iterator ListSorter::chain_end() const
{
    return (the_main_chain.begin());
}

std::ostream& operator<<(std::ostream& os, const ListSorter& l)
{
    os << "\t\tNumbers List\n";
    os << "****************************************\n";
    for (std::list<int>::const_iterator it = l.list_begin();
            it != l.list_end();)
    {
        os << *it;
        ++it;
        if (it != l.list_end())
            std::cout << " ";
    }
    os << "\n****************************************\n";
    os << "\t\tMain chain\n";
    os << "****************************************\n";
    for (std::list<ListChunk>::const_iterator it = l.chain_begin();
        it != l.chain_end(); ++it)
    {
        std::cout <<"[";
        for (std::list<int>::iterator st = it->begin; st != it->end;)
        {
            os << *st;
            ++st;
            if (st != (it->end))
                std::cout << " ";
        }
        std::cout <<"]";
    }
    os << "\n****************************************\n";
    os << "\t\tThe pend\n";
    os << "****************************************\n";
    for (std::list<ListChunk>::const_iterator it = l.pend_begin();
            it != l.pend_end(); ++it)
    {
        std::cout << "[";
        for (std::list<int>::iterator st = it->begin; st != it->end;)
        {
            os << *st;
            ++st;
            if (st != (it->end))
                std::cout << " ";
        }
        std::cout << "]";
    }
    os << "\n****************************************\n";
    return (os << std::endl);
}