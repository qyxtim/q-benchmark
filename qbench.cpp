#include "runner.hpp"
#include <cstddef>
#include <random>

/*
    Parse the options into char** array
*/
char **parseOptInProgram(char *program)
{
    size_t size = 5;
    size_t cur = 0;

    char **args = new char *[size];

    char *token = strtok(program, " ");
    while (token != nullptr)
    {
        if (cur == size - 1)
        {
            char **tmp = new char *[size * 2];
            for (size_t i = 0; i < size; ++i)
            {
                tmp[i] = args[i];
            }
            delete[] args;
            args = tmp;
            size *= 2;
        }
        args[cur] = token;
        ++cur;
        token = strtok(NULL, " ");
    }
    args[cur] = NULL;

    return args;
}

int main(int argc, char **argv)
{
    size_t warmupTimes = 3;
    size_t MIN = 100;
    size_t MAX = 1000;

    // parse options
    int option;
    while ((option = getopt(argc, argv, "w:l:h:")) != -1)
    {
        switch (option)
        {
        case 'w':
            warmupTimes = std::stoul(optarg);
            break;
        case 'l':
            if (!optarg)
                return 1;
            MIN = std::stoul(optarg);
            break;
        case 'h':
            if (!optarg)
                return 1;
            MAX = std::stoul(optarg);
            break;
        default:
            return 1;
        }
    }

    // ensure the options are valid
    MIN = MIN <= 0 ? 1 : MIN;
    MAX = MAX < MIN ? MIN : MAX;

    // Random Number Generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(MIN, MAX);

    // run benchmark for a list of programs
    for (int i = optind; i < argc; ++i)
    {
        Runner runner(parseOptInProgram(argv[i]), distrib(gen), warmupTimes);
        runner.display();
    }

    return EXIT_SUCCESS;
}