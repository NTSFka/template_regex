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

// Posix
#include <regex.h>

// Template Regex
#include "functions.hpp"
#include "../regex.hpp"

/* ************************************************************************ */

/**
 * @brief RAII wrapper for posix regex.
 */
class regex
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param expr Regular expression.
     *
     * @return
     */
    explicit regex(const char* expr) noexcept
    {
        regcomp(&m_regex, expr, REG_EXTENDED);
    }


    /**
     * @brief Destructor.
     */
    ~regex()
    {
        regfree(&m_regex);
    }


// Public Operations
public:


    /**
     * @brief Evaluate expression.
     *
     * @param input
     *
     * @return
     */
    bool operator()(const std::string& input) const noexcept
    {
        return (regexec(&m_regex, input.c_str(), 0, NULL, 0) != REG_NOMATCH);
    }


// Private Data Members
private:

    /// Regex
    regex_t m_regex;
};

/* ************************************************************************ */

static bool test_identifiers(const std::string& input)
{
    static regex re("^[a-zA-Z_][a-zA-Z0-9_]*$");

    return re(input);
}

/* ************************************************************************ */

static bool test_date(const std::string& input)
{
    static regex re("^[0-9][0-9]?/[0-9][0-9]?/[0-9][0-9][0-9][0-9]$");

    return re(input);
}

/* ************************************************************************ */

static bool test_float(const std::string& input)
{
    static regex re("^[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?$");

    return re(input);
}

/* ************************************************************************ */

int main(int argc, char** argv)
{
    const std::string dir = (argc > 1) ? (std::string(argv[1]) + "/") : "./";

    print_header("POSIX Regex");

    const auto identifiers = read_lines(dir + "identifiers.txt");
    const auto dates = read_lines(dir + "dates.txt");
    const auto floats = read_lines(dir + "floats.txt");

    do_test("identifier", test_identifiers, identifiers);
    do_test("date",       test_date,        dates);
    do_test("float",      test_float,       floats);
}

/* ************************************************************************ */
