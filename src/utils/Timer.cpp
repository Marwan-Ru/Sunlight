#include "Timer.h"

void Timer::start()
{
    m_StartTime = std::chrono::steady_clock::now();
    m_bRunning = true;
}

void Timer::stop()
{
    m_EndTime = std::chrono::steady_clock::now();
    m_bRunning = false;
}

void Timer::restart()
{
    stop();
    start();
}

double Timer::getElapsedInMilliseconds() const
{
    std::chrono::time_point<std::chrono::steady_clock> endTime;

    if (m_bRunning)
    {
        endTime = std::chrono::steady_clock::now();
    }
    else
    {
        endTime = m_EndTime;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
}

double Timer::getElapsedInSeconds() const
{
    return getElapsedInMilliseconds() / 1000.0;
}
