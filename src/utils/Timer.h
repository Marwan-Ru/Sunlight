#pragma once

#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>

/**
 * @brief Timer to benchmark code performance using chrono steady_clock
 * because it's not affected by the system time (which can be adjusted at any time)
 * Code proposed by https://gist.github.com/mcleary/b0bf4fa88830ff7c882d
*/
class Timer
{
public:
    void start();
    void stop();
    void restart();

    long long getElapsedInMilliseconds() const;
    long long getElapsedInSeconds() const;

private:
    std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
    std::chrono::time_point<std::chrono::steady_clock> m_EndTime;
    bool m_bRunning = false;
};
