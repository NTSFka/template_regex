/* ************************************************************************ */
/*                                                                          */
/* Copyright (C) 2015 Jiří Fatka <ntsfka@gmail.com>                         */
/*                                                                          */
/* This program is free software: you can redistribute it and/or modify     */
/* it under the terms of the GNU Lesser General Public License as published */
/* by the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                      */
/*                                                                          */
/* This program is distributed in the hope that it will be useful,          */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU Lesser General Public License */
/* along with this program. If not, see <http://www.gnu.org/licenses/>.     */
/*                                                                          */
/* ************************************************************************ */

// C++
#include <string>
#include <vector>
#include <tuple>
#include <chrono>
#include <iomanip>
#include <iostream>

/* ************************************************************************ */

/**
 * @brief Read lines from given file.
 *
 * @param filename
 *
 * @return
 */
std::vector<std::string> read_lines(const std::string& filename);

/* ************************************************************************ */

/**
 * @brief Print Regex header.
 *
 * @param name
 */
inline void print_header(const std::string& name) noexcept
{
    std::cout << "---- " << name << " ----" << std::endl;
}

/* ************************************************************************ */

/**
 * @brief Perform speed test.
 *
 * @tparam Fun Testing function type.
 *
 * @param fun  Test function.
 * @param data Test data.
 *
 * @return
 */
template<typename Fun>
void do_test(const std::string& name, Fun fun, const std::vector<std::string>& data)
{
    std::cout << std::setw(20) << name << "... " << std::flush;

    std::size_t count = 0;
    std::string errinput;

    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& input : data)
    {
        if (!fun(input))
        {
            errinput = input;
            break;
        }

        ++count;
    }

    auto end = std::chrono::high_resolution_clock::now();

    // Get elapsed time
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Print result
    std::cout <<
        count << "/" << data.size() << ": " <<
        std::setw(7) << std::right << time.count() << " ms";

    if (!errinput.empty())
    {
        std::cout << " '" << errinput << "'";
    }

    std::cout << std::endl;
}

/* ************************************************************************ */
