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
#include <type_traits>
#include <sstream>

// Google Test
#include "gtest/gtest.h"

// Template Regex
#include "../string.hpp"

/* ************************************************************************ */

using namespace template_regex;

/* ************************************************************************ */

TEST(string, empty)
{
    // Test empty string
    constexpr auto empty_string = string<>{};
    EXPECT_TRUE(empty_string.empty());

    // Test non-empty string
    constexpr auto non_empty_string = string<'a', 'b', 'c'>{};
    EXPECT_FALSE(non_empty_string.empty());

    // string can be used just as a type

    // Test empty string type
    using empty_string_t = string<>;
    EXPECT_TRUE(empty_string_t::empty());

    // Test non-empty string
    using non_empty_string_t = string<'a', 'b', 'c'>;
    EXPECT_FALSE(non_empty_string_t::empty());

}

/* ************************************************************************ */

TEST(string, size)
{
    // Test empty string
    constexpr auto empty_string = string<>{};
    EXPECT_EQ(0, empty_string.size());
    EXPECT_EQ(0, empty_string.length());

    // Test non-empty string
    constexpr auto non_empty_string = string<'a', 'b', 'c'>{};
    EXPECT_EQ(3, non_empty_string.size());
    EXPECT_EQ(3, non_empty_string.length());

    // string can be used just as a type

    // Test empty string type
    using empty_string_t = string<>;
    EXPECT_EQ(0, empty_string_t::size());
    EXPECT_EQ(0, empty_string_t::length());

    // Test non-empty string
    using non_empty_string_t = string<'a', 'b', 'c'>;
    EXPECT_EQ(3, non_empty_string_t::size());
    EXPECT_EQ(3, non_empty_string_t::length());
}

/* ************************************************************************ */

TEST(string, split)
{
    //template_assert
}

/* ************************************************************************ */

TEST(string, find)
{
    using str_t = basic_string<char, 'a', 'b', 'c', 'd', 'e'>;

    ::testing::StaticAssertTypeEq<
        basic_string_split<2, char, str_t>::first,
        basic_string<char, 'a', 'b'>
    >();
}

/* ************************************************************************ */

TEST(string, make)
{
    // Test empty string
    using empty_string = make_string_t("");
    ::testing::StaticAssertTypeEq<
        empty_string,
        basic_string<char>
    >();

    using non_empty_string = make_string_t("abcd");
    ::testing::StaticAssertTypeEq<
        non_empty_string,
        basic_string<char, 'a', 'b', 'c', 'd'>
    >();
}

/* ************************************************************************ */

TEST(string, operators)
{

}

/* ************************************************************************ */

TEST(string, print)
{
    {
        constexpr auto str = make_string("Hello World!");

        std::ostringstream oss;
        oss << str;

        EXPECT_EQ(oss.str(), "Hello World!");
    }
}

/* ************************************************************************ */
