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

// Google Test
#include "gtest/gtest.h"

// Template Regex
#include "../rules.hpp"

/* ************************************************************************ */

using namespace template_regex;

/* ************************************************************************ */

TEST(rules, value)
{
    using rule = rules::val<'a'>;

    // Match
    {
        const std::string str = "a";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::end(str), it);
    }

    // No match
    {
        const std::string str = "A";

        EXPECT_FALSE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_FALSE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::begin(str), it);
    }
}

/* ************************************************************************ */

TEST(rules, range)
{
    using rule = rules::range<'a', 'z'>;

    // Match
    {
        const std::string str = "a";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::end(str), it);
    }

    // Match
    {
        const std::string str = "x";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::end(str), it);
    }

    // Match
    {
        const std::string str = "z";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::end(str), it);
    }

    // No match
    {
        const std::string str = "A";

        EXPECT_FALSE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_FALSE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::begin(str), it);
    }

    // No match
    {
        const std::string str = "X";

        EXPECT_FALSE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_FALSE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::begin(str), it);
    }
}

/* ************************************************************************ */

TEST(rules, sequence)
{
    using rule = rules::sequence<
        rules::val<'a'>,
        rules::val<'b'>,
        rules::val<'c'>,
        rules::val<'d'>,
        rules::val<'e'>,
        rules::val<'f'>
    >;

    // Full match
    {
        const std::string str = "abcdef";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::end(str), it);
    }

    // Full match - more input data
    {
        const std::string str = "abcdefghijk";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::next(std::begin(str), 6), it);
    }

    // Partial match
    {
        const std::string str = "abc";

        EXPECT_FALSE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_FALSE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::next(std::begin(str), 3), it);
    }

    // No match
    {
        const std::string str = "defghij";

        EXPECT_FALSE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_FALSE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::begin(str), it);
    }
}

/* ************************************************************************ */

TEST(rules, alternatives)
{
    using rule = rules::alternative<
        rules::val<'a'>,
        rules::val<'b'>,
        rules::val<'c'>
    >;

    // Match
    {
        const std::string str = "a";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::end(str), it);
    }

    // Match
    {
        const std::string str = "b";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::end(str), it);
    }

    // Match
    {
        const std::string str = "c";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::end(str), it);
    }

    // No match
    {
        const std::string str = "d";

        EXPECT_FALSE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_FALSE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::begin(str), it);
    }

    // No match
    {
        const std::string str = "za";

        EXPECT_FALSE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_FALSE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::begin(str), it);
    }
}

/* ************************************************************************ */

TEST(rules, alternatives_not)
{
    using rule = rules::alternative_not<
        rules::val<'a'>,
        rules::val<'b'>,
        rules::val<'c'>
    >;

    // Match
    {
        const std::string str = "d";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::end(str), it);
    }

    // Match
    {
        const std::string str = "e";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::end(str), it);
    }

    // Match
    {
        const std::string str = "x";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::end(str), it);
    }

    // No match
    {
        const std::string str = "a";

        EXPECT_FALSE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_FALSE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::begin(str), it);
    }

    // No match
    {
        const std::string str = "b";

        EXPECT_FALSE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_FALSE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::begin(str), it);
    }

    // No match
    {
        const std::string str = "c";

        EXPECT_FALSE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_FALSE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::begin(str), it);
    }
}

/* ************************************************************************ */

TEST(rules, repeat)
{
    using rule = rules::repeat<
        rules::val<'a'>
    >;

    // Full match
    {
        const std::string str = "aaaaa";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::end(str), it);
    }

    // Partial match
    {
        const std::string str = "aabbc";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::next(std::begin(str), 2), it);
    }

    // No match
    {
        const std::string str = "baaaaa";

        EXPECT_FALSE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_FALSE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::begin(str), it);
    }

    // No match
    {
        const std::string str = "bbbbb";

        EXPECT_FALSE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_FALSE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::begin(str), it);
    }
}

/* ************************************************************************ */

TEST(rules, repeat_optional)
{
    using rule = rules::repeat_optional<
        rules::val<'b'>
    >;

    // In fact, this rule returns always true

    // Full match
    {
        const std::string str = "bbbbb";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::end(str), it);
    }

    // Partial match
    {
        const std::string str = "bbbaaa";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::next(std::begin(str), 3), it);
    }

    // Optional match
    {
        const std::string str = "aaaaa";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rule::match_ref(it, std::end(str)));
        EXPECT_EQ(std::begin(str), it);
    }
}

/* ************************************************************************ */

TEST(rules, optional)
{
    using rule = rules::repeat_optional<
        rules::val<'o'>
    >;

    // In fact, this rule returns always true

    // Full match
    {
        const std::string str = "o";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));
    }

    // "Partial" match
    {
        const std::string str = "c";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str)));
    }

}

/* ************************************************************************ */

TEST(rules, newline)
{
    // Native text
    {
        const std::string str = "\n";

        EXPECT_TRUE(rules::matcher<rules::newline>::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rules::newline::match_ref(it, std::end(str)));
        EXPECT_EQ(std::end(str), it);
    }

    // Windows
    {
        const std::string str = "\r\n";

        EXPECT_TRUE(rules::newline_multiplatform::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rules::newline_multiplatform::match_ref(it, std::end(str)));
        EXPECT_EQ(std::end(str), it);
    }

    // Unix
    {
        const std::string str = "\n";

        EXPECT_TRUE(rules::newline_multiplatform::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rules::newline_multiplatform::match_ref(it, std::end(str)));
        EXPECT_EQ(std::end(str), it);
    }

    // Mac
    {
        const std::string str = "\r";

        EXPECT_TRUE(rules::newline_multiplatform::match(std::begin(str), std::end(str)));

        auto it = std::begin(str);
        EXPECT_TRUE(rules::newline_multiplatform::match_ref(it, std::end(str)));
        EXPECT_EQ(std::end(str), it);
    }
}

/* ************************************************************************ */

TEST(rules, line_comment)
{
    // //[^\n]*\n
    using comment = rules::sequence<
        rules::val<'/'>,
        rules::val<'/'>,
        rules::repeat_optional<rules::val_not<'\n'>>,
        rules::val<'\n'>
    >;

    // Standard
    {
        const std::string str = "// Hello, World!\n";

        EXPECT_TRUE(comment::match(std::begin(str), std::end(str)));
    }

    // Empty
    {
        const std::string str = "//\n";

        EXPECT_TRUE(comment::match(std::begin(str), std::end(str)));
    }

    // Empty with whitespaces
    {
        const std::string str = "//   \n";

        EXPECT_TRUE(comment::match(std::begin(str), std::end(str)));
    }
}

/* ************************************************************************ */

TEST(rules, capture)
{
    // \[([a-z]+)\]
    using rule = rules::sequence<
        rules::val<'['>,
        rules::capture<
            rules::repeat<rules::range<'a', 'z'>>
        >,
        rules::val<']'>
    >;

    // Standard
    {
        const std::string str = "[abcdef]";
        std::string name;

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str), std::back_inserter(name)));
        EXPECT_EQ("abcdef", name);
    }

/* This doesn't work
    // \[([a-z]+)\]
    using rule2 = rules::sequence<
        rules::val<'['>,
        rules::capture<
            rules::repeat<rules::alternative_not<rules::val<']'>>>
        >,
        rules::val<']'>
    >;

    // Standard
    {
        const std::string str = "[abcdef]";
        std::string name;

        EXPECT_TRUE(rule2::match(std::begin(str), std::end(str), std::back_inserter(name)));
        EXPECT_EQ("abcdef", name);
    }
*/
}

/* ************************************************************************ */

/**
 * @brief Test variable storing.
 */
TEST(rules, store)
{
    /*
     * Equivalent code
     *
     * if (*it == '[') {
     *     ++it;
     *     value = 5;
     *     while (*it >= 'a' && *it <= 'z') {
     *         ++it;
     *     }
     *
     *     if (*it == ']')
     *         ++it;
     * }
     */

    // \[([a-z]+)\]
    using rule = rules::sequence<
        rules::val<'['>,
        rules::store<rules::null_rule, rules::val<5>>,
        rules::repeat<rules::range<'a', 'z'>>,
        rules::val<']'>
    >;

    // Standard
    {
        int value = 0;
        const std::string str = "[abcdef]";

        EXPECT_TRUE(rule::match(std::begin(str), std::end(str), std::ref(value)));
        EXPECT_EQ(5, value);
    }
}

/* ************************************************************************ */

TEST(rules, identifier)
{
    // [a-z_][a-z0-9_]*
    using identifier = rules::sequence<
        rules::alternative<
            rules::range<'a', 'z'>,
            rules::val<'_'>
        >,
        rules::repeat_optional<
            rules::alternative<
                rules::range<'a', 'z'>,
                rules::range<'0', '9'>,
                rules::val<'_'>
            >
        >
    >;

    // Standard
    {
        const std::string str = "long_identifier";

        EXPECT_TRUE(identifier::match_all(std::begin(str), std::end(str)));
    }
}

/* ************************************************************************ */
