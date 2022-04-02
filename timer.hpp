#include <chrono>

class Timer
{
public:
    Timer() : m_start(std::chrono::high_resolution_clock::now())
    {
    }

    double stop()
    {
        auto m_end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(m_end - m_start);
        return duration.count() * 0.001;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};