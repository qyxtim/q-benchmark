#include "timer.hpp"
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
    Runner(char **args, size_t numRuns, size_t warmupTimes) : args(args), numRuns(numRuns)
    {
        tests = new double[numRuns];
        run(warmupTimes);
    }

    ~Runner()
    {
        delete[] args;
        delete[] tests;
    }

    void display() const
    {
        if (!hasRunSuccess)
        {
            fprintf(stderr, "Test failed to run");
            return;
        }

        double mean = 0, std = 0, min_v = tests[0], max_v = tests[0];

        for (size_t i = 0; i < numRuns; ++i)
        {
            mean += tests[i];
            min_v = tests[i] < min_v ? tests[i] : min_v;
            max_v = tests[i] > max_v ? tests[i] : max_v;
        }
        mean /= numRuns;

        for (size_t i = 0; i < numRuns; ++i)
        {
            std += (tests[i] - mean) * (tests[i] - mean);
        }
        std = sqrt(std / numRuns);

        fprintf(stdout, STYLE_BOLD);
        fprintf(stdout, "Benchmark: %s (%s%zu%s runs)\n", args[0], BLUE, numRuns, RESET);
        fprintf(stdout, STYLE_NO_BOLD);

        fprintf(stdout, "  Time: %s%f ms%s(mean) ± %s%f ms%s(std)\n", GREEN, mean, RESET, PURPLE, std, RESET);

        fprintf(stdout, "  Range: %s%f ms%s(min) … %s%f ms%s(max)\n",
                GREEN, min_v, RESET, PURPLE, max_v, RESET);

        fprintf(stdout, "\n");
    }

private:
    bool launch()
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
            if (execvp(args[0], args) == -1)
            {
                perror(args[0]);
                exit(3);
            }
        }
        else if (pid < 0)
        {
            perror("Runner");
        }
        else
        {
            do
            {
                // wait for child process to exit
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            return WEXITSTATUS(status) != 3;
        }

        return false;
    }

    // run warmup to minimize the effect of cache
    bool warmup(size_t warmupTimes)
    {
        for (size_t i = 0; i < warmupTimes; ++i)
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
        for (size_t i = 0; i < numRuns; ++i)
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
    void run(size_t warmupTimes)
    {
        if (warmup(warmupTimes))
        {
            hasRunSuccess = runTest();
        }
    }

private:
    // Program Info
    char **args;
    size_t numRuns;

    // Test Result
    double *tests;

    // Flag
    bool hasRunSuccess = false;
};