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

// Template Regex
#include "functions.hpp"

/* ************************************************************************ */

/**
 * @brief If character is digit.
 *
 * @param c
 *
 * @return
 */
inline bool isDigit(char c) noexcept
{
    return (c >= '0' && c <= '9');
}

/* ************************************************************************ */

/**
 * @brief If character is digit.
 *
 * @param c
 *
 * @return
 */
inline bool isAlpha(char c) noexcept
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

/* ************************************************************************ */

static bool test_identifiers(const std::string& input)
{
    auto it = input.begin();

    if (!isAlpha(*it) && *it != '_')
        return false;

    ++it;

    while (isAlpha(*it) || isDigit(*it) || (*it == '_'))
        ++it;

    return it == input.end();
}

/* ************************************************************************ */

static bool test_date(const std::string& input)
{
    auto it = input.begin();

    if (!isDigit(*it))
        return false;

    ++it;

    if (isDigit(*it))
        ++it;

    if (*it != '/')
        return false;

    ++it;

    // -----------

    if (!isDigit(*it))
        return false;

    ++it;

    if (isDigit(*it))
        ++it;

    if (*it != '/')
        return false;

    ++it;

    // -----------

    if (!isDigit(*it))
        return false;

    ++it;

    if (!isDigit(*it))
        return false;

    ++it;

    if (!isDigit(*it))
        return false;

    ++it;

    if (!isDigit(*it))
        return false;

    ++it;

    return it == input.end();
}

/* ************************************************************************ */

static bool test_float(const std::string& input)
{
    auto it = input.begin();

    if (*it == '-' || *it == '+')
        ++it;

    while (isDigit(*it))
        ++it;

    if (*it == '.')
        ++it;

    if (!isDigit(*it))
        return false;

    while (isDigit(*it))
        ++it;

    if (*it == 'e' || *it == 'E')
    {
        if (*it == '-' || *it == '+')
            ++it;

        if (!isDigit(*it))
            return false;

        while (isDigit(*it))
            ++it;
    }

    return it == input.end();
}

/* ************************************************************************ */

int main(int argc, char** argv)
{
    const std::string dir = (argc > 1) ? (std::string(argv[1]) + "/") : "./";

    print_header("Handwritten");

    const auto identifiers = read_lines(dir + "identifiers.txt");
    const auto dates = read_lines(dir + "dates.txt");
    const auto floats = read_lines(dir + "floats.txt");

    do_test("identifier", test_identifiers, identifiers);
    do_test("date",       test_date,        dates);
    do_test("float",      test_float,       floats);
}

/* ************************************************************************ */
