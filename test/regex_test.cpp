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

// Google Test
#include "gtest/gtest.h"

// Template Regex
#include "../regex.hpp"

/* ************************************************************************ */

using namespace template_regex;

/* ************************************************************************ */

TEST(regex, rules)
{
    // Simple
    ::testing::StaticAssertTypeEq<
        make_regex_t("a")::rule,
        rules::val<'a'>
    >();

    ::testing::StaticAssertTypeEq<
        make_regex_t("ab")::rule,
        rules::sequence<rules::val<'a'>, rules::val<'b'>>
    >();

    ::testing::StaticAssertTypeEq<
        make_regex_t("a?")::rule,
        rules::optional<rules::val<'a'>>
    >();

    ::testing::StaticAssertTypeEq<
        make_regex_t("a+")::rule,
        rules::repeat<rules::val<'a'>>
    >();

    ::testing::StaticAssertTypeEq<
        make_regex_t("a*")::rule,
        rules::repeat_optional<rules::val<'a'>>
    >();

    ::testing::StaticAssertTypeEq<
        make_regex_t("a?")::rule,
        rules::optional<rules::val<'a'>>
    >();

    ::testing::StaticAssertTypeEq<
        make_regex_t("[a-z]")::rule,
        rules::alternative<rules::range<'a', 'z'>>
    >();

    ::testing::StaticAssertTypeEq<
        make_regex_t("abcd")::rule,
        rules::sequence<rules::val<'a'>, rules::val<'b'>, rules::val<'c'>, rules::val<'d'>>
    >();

    ::testing::StaticAssertTypeEq<
        make_regex_t("ab+")::rule,
        rules::sequence<rules::val<'a'>, rules::repeat<rules::val<'b'>>>
    >();

    ::testing::StaticAssertTypeEq<
        make_regex_t("abc+")::rule,
        rules::sequence<rules::val<'a'>, rules::val<'b'>, rules::repeat<rules::val<'c'>>>
    >();

    // Complex
    ::testing::StaticAssertTypeEq<
        make_regex_t("a+z?")::rule,
        rules::sequence<rules::repeat<rules::val<'a'>>, rules::optional<rules::val<'z'>>>
    >();

    ::testing::StaticAssertTypeEq<
        make_regex_t("[a-z0-9]")::rule,
        rules::alternative<rules::range<'a', 'z'>, rules::range<'0', '9'>>
    >();

    ::testing::StaticAssertTypeEq<
        make_regex_t("[a-z0-9]+")::rule,
        rules::repeat<rules::alternative<rules::range<'a', 'z'>, rules::range<'0', '9'>>>
    >();

    ::testing::StaticAssertTypeEq<
        make_regex_t("[a-z_][a-z0-9_]*")::rule,
        rules::sequence<
            rules::alternative<rules::range<'a', 'z'>, rules::val<'_'>>,
            rules::repeat_optional<rules::alternative<
                rules::range<'a', 'z'>,
                rules::range<'0', '9'>,
                rules::val<'_'>
            >>
        >
    >();

    ::testing::StaticAssertTypeEq<
        make_regex_t("[0-9][0-9]?/[0-9][0-9]?/[0-9][0-9][0-9][0-9]")::rule,
        rules::sequence<
            rules::alternative<rules::range<'0', '9'>>,
            rules::optional<rules::alternative<rules::range<'0', '9'>>>,
            rules::val<'/'>,
            rules::alternative<rules::range<'0', '9'>>,
            rules::optional<rules::alternative<rules::range<'0', '9'>>>,
            rules::val<'/'>,
            rules::alternative<rules::range<'0', '9'>>,
            rules::alternative<rules::range<'0', '9'>>,
            rules::alternative<rules::range<'0', '9'>>,
            rules::alternative<rules::range<'0', '9'>>
        >
    >();
}

/* ************************************************************************ */

TEST(regex, regex1)
{
    auto regex = make_regex("^abcd$");

    const std::string str = "abcd";
    EXPECT_TRUE(regex_match(regex, str));
}

/* ************************************************************************ */

TEST(regex, regex2)
{
    auto regex = make_regex("^a*$");

    const std::string str = "aaaaa";
    EXPECT_TRUE(regex_match(regex, str));
}

/* ************************************************************************ */

TEST(regex, regex3)
{
    auto regex = make_regex("^a*b+c?$");

    const std::string str = "aaaaabc";
    EXPECT_TRUE(regex_match(regex, str));
}

/* ************************************************************************ */

TEST(regex, regex4)
{
    auto regex = make_regex("^x[a-z0-9][a-z0-9]$");

    const std::string str = "xd5";
    EXPECT_TRUE(regex_match(regex, str));
}

/* ************************************************************************ */

TEST(regex, regex5)
{
    auto regex = make_regex("[a-z0-9]+");

    const std::string str = "abcd584cd";
    EXPECT_TRUE(regex_match(regex, str));
}

/* ************************************************************************ */

TEST(regex, regex6)
{
    auto regex = make_regex("^a?[a-z]+T?$");

    {
        const std::string str = "aaaaa";
        EXPECT_TRUE(regex_match(regex, str));
    }

    {
        const std::string str = "azzxxxzzzT";
        EXPECT_TRUE(regex_match(regex, str));
    }
}

/* ************************************************************************ */
