// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include "Log.h"
#include <iostream>

Log::Log()
    : m_logFile(true), m_logStdout(true), m_file("log.txt")
{

}

void Log::setLogToFile(bool param)
{
    m_logFile = param;
}

void Log::setLogToStdout(bool param)
{
    m_logStdout = param;
}

Log& Log::operator<<(const char* str)
{
    if (m_logFile) m_file << str;
    if (m_logStdout) std::cout << str;

    return *this;
}

Log& Log::operator<<(const std::string& str)
{
    if (m_logFile) m_file << str;
    if (m_logStdout) std::cout << str;

    return *this;
}

Log& Log::operator<<(int val)
{
    if (m_logFile)
    {
        m_file << val;
        m_file.flush();
    }
    if (m_logStdout)
    {
        std::cout << val;
        std::cout.flush();
    }

    return *this;
}

Log& Log::operator<<(double val)
{
    if (m_logFile) m_file << val;
    if (m_logStdout) std::cout << val;

    return *this;
}