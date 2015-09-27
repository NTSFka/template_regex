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
#include <iostream>

// Template Regex
#include "functions.hpp"
#include "../regex.hpp"

/* ************************************************************************ */

static bool test_identifiers(const std::string& input)
{
    using namespace template_regex;

    static auto regex = make_regex("^[a-zA-Z_][a-zA-Z0-9_]*$");

    return regex_match(regex, input);
}

/* ************************************************************************ */

static bool test_date(const std::string& input)
{
    using namespace template_regex;

    static auto regex = make_regex("^[0-9][0-9]?/[0-9][0-9]?/[0-9][0-9][0-9][0-9]$");

    return regex_match(regex, input);
}

/* ************************************************************************ */

static bool test_float(const std::string& input)
{
    using namespace template_regex;

    static auto regex = make_regex("^[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?$");

    return regex_match(regex, input);
}

/* ************************************************************************ */

int main(int argc, char** argv)
{
    const std::string dir = (argc > 1) ? (std::string(argv[1]) + "/") : "./";

    print_header("Template Regex");

    const auto identifiers = read_lines(dir + "identifiers.txt");
    const auto dates = read_lines(dir + "dates.txt");
    const auto floats = read_lines(dir + "floats.txt");

    do_test("identifier", test_identifiers, identifiers);
    do_test("date",       test_date,        dates);
    do_test("float",      test_float,       floats);
}

/* ************************************************************************ */
