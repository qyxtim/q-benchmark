#include "timer.hpp"
#include <cmath>
#include <iostream>
#include <memory>
#include <sys/wait.h>
#include <unistd.h>

#define STYLE_BOLD "\033[1m"
#define STYLE_NO_BOLD "\033[22m"
#define GREEN "\033[32m"
#define PURPLE "\033[35m"
#define BLUE "\x1b[36m"
#define RESET "\033[0m"

class Runner
{
public:
    Runner(std::unique_ptr<char *[]> &&args, std::size_t numRuns, std::size_t warmupTimes) : args(std::move(args)), numRuns(numRuns), tests(std::make_unique<double[]>(numRuns))
    {
        run(warmupTimes);
    }

    ~Runner()
    {
    }

    bool hasTestRunSuccessfully() const{
        return hasRunSuccess;
    }

    void display() const
    {
        if (!hasRunSuccess)
        {
            std::cerr << "Test failed to run" << std::endl;
            return;
        }

        double mean = 0, std = 0, min_v = tests[0], max_v = tests[0];

        for (std::size_t i = 0; i < numRuns; ++i)
        {
            mean += tests[i];
            min_v = tests[i] < min_v ? tests[i] : min_v;
            max_v = tests[i] > max_v ? tests[i] : max_v;
        }
        mean /= numRuns;

        for (std::size_t i = 0; i < numRuns; ++i)
            std += pow(tests[i] - mean, 2);
        std = sqrt(std / numRuns);

        std::cout << STYLE_BOLD << "Benchmark: " << args[0] << " ("
                  << BLUE << numRuns << RESET << " runs)"
                  << STYLE_NO_BOLD << std::endl;

        std::cout << "  Time: " << GREEN << mean << " ms" << RESET << "(mean) ± "
                  << PURPLE << std << " ms" << RESET << "(std)" << std::endl;

        std::cout << "  Range: " << GREEN << min_v << " ms" << RESET << "(min) … "
                  << PURPLE << max_v << " ms" << RESET << "(max)" << std::endl;

        std::cout << std::endl;
    }

private:
    bool launch() const
    {
        pid_t pid;
        int status;

        // fork a child process
        pid = fork();

        if (pid == 0)
        {
            // rewire the file descriptor
            FILE *f = fopen("/dev/null", "w");
            int fd = fileno(f);
            dup2(fd, 1);
            dup2(fd, 2);
            close(fd);

            // run the benchmark program with args
            if (execvp(args[0], args.get()) == -1)
            {
                exit(EXIT_FAILURE);
            }
        }
        else if (pid < 0)
        {
            throw std::runtime_error("Failed to fork() the program");
        }
        else
        {
            do
            {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));

            if(WEXITSTATUS(status) != EXIT_SUCCESS)
                std::cerr << "Failed to run " << args[0] << std::endl;
            return WEXITSTATUS(status) == EXIT_SUCCESS;
        }

        return false;
    }

    // run warmup to minimize the effect of cache
    bool warmup(std::size_t warmupTimes) const
    {
        for (std::size_t i = 0; i < warmupTimes; ++i)
        {
            if (!launch())
            {
                return false;
            }
        }
        return true;
    }

    bool runTest()
    {
        for (std::size_t i = 0; i < numRuns; ++i)
        {
            Timer t;
            bool success = launch();
            tests[i] = t.stop();

            if (!success)
            {
                return false;
            }
        }
        return true;
    }

    // wrapper of warmup and runTest
    void run(std::size_t warmupTimes)
    {
        if (warmup(warmupTimes))
        {
            hasRunSuccess = runTest();
        }
    }

private:
    // Program Info
    const std::unique_ptr<char *[]> args;
    const std::size_t numRuns;

    // Test Result
    const std::unique_ptr<double[]> tests;

    // Flag
    bool hasRunSuccess = false;
};