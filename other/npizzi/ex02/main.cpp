#include "PmergeMe.hpp"
#include <ctime>
#include <sys/time.h>

bool    is_separator(int i, char* str)
{
    if (i == 0)
        return (false);
    if (str[i] == ' ' && (!isdigit(str[i - 1]) || !isdigit(str[i + 1])))
        return (false);
    return (true);
}

bool    validate_arg(int argc, char* argv[])
{
    if (argc == 2)
    {
        for (int i = 0; argv[1][i]; ++i)
        {
            if (!isdigit(argv[1][i])
                && !is_separator(i, argv[1]))
                return (false);
        }
    }
    else
    {
        for (int i = 1; i < argc; ++i)
        {
            for (int x = 0; argv[i][x]; ++x)
            {
                if (!isdigit(argv[i][x]))
                    return (false);
            }
        }
    }
    return (true);
}

void    time_vector(int argc, char* argv[])
{
    try
    {
        std::clock_t start = std::clock();
        VecSorter v;
        if (argc == 2)
            v = VecSorter(argv[1]);
        else
            v = VecSorter(argv);
        std::cout << "Before : ";
        v.print_numbers();
        v.sort(1);
        std::clock_t end = std::clock();
        std::cout << "After : ";
        v.print_numbers();
        double time_total = (static_cast<double>(end - start) / CLOCKS_PER_SEC) * MSECS_IN_SEC;
        if (check_if_sorted(v.get_numbers()))
            std::cout << "Time to process a range of " << v.get_numbers().size()
                        << " elements with std::vector : " << time_total << "ms\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return;
    }
}

void    time_list(int argc, char* argv[])
{
    try
    {
        std::clock_t start = std::clock();
        ListSorter l;
        if (argc == 2)
            l = ListSorter(argv[1]);
        else
            l = ListSorter(argv);
        //std::cout << "Before : ";
        //l.print_numbers();
        l.sort(1);
        std::clock_t end = std::clock();
        //std::cout << "After : ";
        //l.print_numbers();
        double time_total = (static_cast<double>(end - start) / CLOCKS_PER_SEC) * MSECS_IN_SEC;
        if (check_if_sorted(l.get_numbers()))
            std::cout << "Time to process a range of " << l.get_numbers().size()
                     << " elements with std::list   : " << time_total << "ms\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return;
    }
}


int main(int argc, char* argv[])
{
    if (argc < 2 || !validate_arg(argc , argv))
    {
        std::cerr << "Error : params not correct\n";
        return (1);
    }

    time_vector(argc, argv);
    time_list(argc, argv);
    return (0);
}