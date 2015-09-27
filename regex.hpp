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

#pragma once

/* ************************************************************************ */

/**
 * @brief Regular expression grammar.
 *
 * EBNF
 * RE             = [ "^" ] inner-RE [ "$" ]
 * inner-RE       = simple-RE { "|" simple-RE }
 * simple-RE      = basic-RE { basic-RE }
 * basic-RE       = elementary-RE [ "*" | "+" | "?" ]
 * elementary-RE  = group | any | char | set
 * group          = "(" inner-RE ")"
 * any            = "."
 * char           = any non metacharacter | "\" metacharacter
 * set            = "[" [ "^" ] set-items "]"
 * set-items      = set-item { set-item }
 * set-item       = char [ "-" char ]
 *
 * Based on:
 * @link http://www.cs.sfu.ca/~cameron/Teaching/384/99-3/regexp-plg.html
 *
 */

/* ************************************************************************ */

// Library
#include "rules.hpp"
#include "string.hpp"

/* ************************************************************************ */

namespace template_regex {

/* ************************************************************************ */

/**
 * @brief Invalid type that helps printing types.
 */
template<typename T>
struct _type_printer
{
    typename T::______ a;
};

/* ************************************************************************ */

/**
 * @brief Invalid type that helps printing types.
 */
template<int V, typename T>
struct _value_printer : _type_printer<T> {};

/* ************************************************************************ */

/**
 * @brief Inner regular expression forward declaration.
 *
 * @tparam Str String type.
 */
template<typename Str, bool end = false>
struct regex_parser_inner_re;

/* ************************************************************************ */

/**
 * @brief Parser for any character that match requirement.
 *
 * If the first character match the required character the data members
 * are set to specific values. The `match` is set to true when the
 * character is matched and `rest` is a substring of the source string
 * without the first (matched) character. Otherwise, the `match` is
 * false and `rest` is same as the source string.
 *
 * @tparam Str       Source string.
 * @tparam CharT     Character type.
 * @tparam CharValue Character to match.
 */
template<typename Str, typename CharT, CharT CharValue>
struct regex_parser_next_if
{

// Public Constants
public:


    /// If `CharValue` match the first character of the source string `Str`.
    static constexpr bool match = character_at_str<0, Str>::value == CharValue;


// Public Types
public:


    /// Rest of the source string.
    using rest = typename basic_string_builder_rest_str<
        (match ? 1 : 0),
        typename Str::value_type,
        basic_string<typename Str::value_type>,
        Str
    >::type;

};

/* ************************************************************************ */

/**
 * @brief Parser for REGEX character.
 *
 * char = any non metacharacter | "\" metacharacter
 *
 * @tparam Str Source string.
 */
template<typename Str>
struct regex_parser_char
{

// Public Constants
public:


    /// If character is escaped.
    static constexpr bool escaped = character_at_str<0, Str>::value == '\\';

    // Character value
    static constexpr typename Str::value_type value = character_at_str<escaped ? 1 : 0, Str>::value;


// Public Types
public:


    /// Regex matching rule.
    using rule = rules::val<value>;


    /// Rest of the source string.
    using rest = typename basic_string_builder_rest_str<escaped ? 2 : 1,
        typename Str::value_type,
        basic_string<typename Str::value_type>, Str
    >::type;

};

/* ************************************************************************ */

/**
 * @brief Parser for REGEX set-item.
 *
 * set-item = char [ "-" char ]
 *
 * @tparam Str Source string.
 */
template<typename Str>
struct regex_parser_set_item
{

// Private Types
private:


    /// Parse the first character.
    using _char1 = regex_parser_char<Str>;

    /// Parse dash character.
    using _dash  = regex_parser_next_if<typename _char1::rest, typename Str::value_type, '-'>;

    /// Parse the last character
    using _char2 = regex_parser_char<typename _dash::rest>;


// Public Constants
public:


    /// Character value.
    static constexpr typename Str::value_type value = _char1::value;

    /// If item is a range of characters.
    static constexpr bool is_range = _dash::match;

    /// The first value of the range.
    static constexpr typename Str::value_type value1 = is_range ? value : typename Str::value_type();

    /// The last value of the range.
    static constexpr typename Str::value_type value2 = is_range ? _char2::value : typename Str::value_type();


// Public Types
public:


    /// Regex matching rule.
    using rule = typename std::conditional<is_range,
        rules::range<_char1::value, _char2::value>,
        typename _char1::rule
    >::type;


    /// Rest of the source string.
    using rest = typename std::conditional<is_range,
        typename _char2::rest,
        typename _char1::rest
    >::type;

};

/* ************************************************************************ */

/**
 * @brief Parser for REGEX set-items.
 *
 * set-items = set-item { set-item }
 *
 * FIRST = char
 * FOLLOW = "]"
 *
 * @tparam Str Source string.
 */
template<typename Str, bool end = false>
struct regex_parser_set_items
{

// Private Types
private:


    /// The first parsed item.
    using _item = regex_parser_set_item<Str>;

    /// Following items.
    using _next = regex_parser_set_items<typename _item::rest,
        character_at_str<0, typename _item::rest>::value == ']' ||
        character_at_str<0, typename _item::rest>::value == 0
    >;


// Public Types
public:


    /// Regex matching rule.
    using rule = typename rules::sequence_remove<
        typename rules::sequence_merge<rules::sequence<typename _item::rule>, typename _next::rule>::type
    >::type;

    /// Rest of the regex string.
    using rest = typename _next::rest;

};

/* ************************************************************************ */

/**
 * @brief End specialization of `regex_parser_set_items_inner`.
 */
template<typename Str>
struct regex_parser_set_items<Str, true>
{

// Public Types
public:


    /// Regex matching rule.
    using rule = void;

    /// Rest of the regex string.
    using rest = Str;

};

/* ************************************************************************ */

/**
 * @brief Parser for REGEX set.
 *
 * set = "[" [ "^" ] set-items "]"
 */
template<typename Str>
struct regex_parser_set
{
    // Pre-conditions
    static_assert(character_at_str<0, Str>::value == '[', "Missing '[' character");


// Private Types
private:


    /// Parse open square.
    using _open  = regex_parser_next_if<Str, typename Str::value_type, '['>;

    /// If set is negated.
    using _neg   = regex_parser_next_if<typename _open::rest, typename Str::value_type, '^'>;

    /// Parse set items.
    using _items = regex_parser_set_items<typename _neg::rest>;

    /// Parse close square.
    using _close = regex_parser_next_if<typename _items::rest, typename Str::value_type, ']'>;


    // Post-conditions
    static_assert(_close::match, "Missing ']' character");


// Public Types
public:


    /// Regex matching rule.
    using rule = typename std::conditional<_neg::match,
        typename rules::sequence_remove<rules::alternative_not<typename _items::rule>>::type,
        typename rules::sequence_remove<rules::alternative<typename _items::rule>>::type
    >::type;

    /// Rest of the source string.
    using rest = typename _close::rest;

};

/* ************************************************************************ */

/**
 * @brief Parser for REGEX group.
 *
 * group = "(" inner-RE ")"
 */
template<typename Str>
struct regex_parser_group
{

// Private Types
private:


    using _open  = regex_parser_next_if<Str, typename Str::value_type, '('>;
    using _inner = regex_parser_inner_re<typename _open::rest>;
    using _close = regex_parser_next_if<typename _inner::rest, typename Str::value_type, ')'>;


    // Post-conditions
    static_assert(_open::match, "Group must begins with '('");
    static_assert(_close::match, "Group must ends with ')'");


// Public Types
public:


    /// Regex matching rule.
    using rule = typename _inner::rule;

    /// Rest of the source string.
    using rest = typename _close::rest;

};

/* ************************************************************************ */

/**
 * @brief Parser for REGEX elementary RE.
 *
 * elementary-RE = "." | group | set | char
 */
template<typename Str>
struct regex_parser_elementary_re
{
    using _is_any   = regex_parser_next_if<Str, typename Str::value_type, '.'>;
    using _is_group = regex_parser_next_if<Str, typename Str::value_type, '('>;
    using _is_set   = regex_parser_next_if<Str, typename Str::value_type, '['>;

    using _selected = typename std::conditional<
        _is_group::match,
        regex_parser_group<Str>,
        typename std::conditional<
            _is_set::match,
            regex_parser_set<Str>,
            regex_parser_char<Str>
        >::type
    >::type;


// Public Types
public:


    /// Regex matching rule.
    using rule = typename std::conditional<_is_any::match, rules::any, typename _selected::rule>::type;

    /// Rest of the source string.
    using rest = typename std::conditional<_is_any::match, typename _is_any::rest, typename _selected::rest>::type;

};

/* ************************************************************************ */

/**
 * @brief Parser for REGEX basic RE.
 *
 * basic-RE = elementary-RE [ "*" | "+" | "?" ]
 */
template<typename Str>
struct regex_parser_basic_re
{
    /// Parse elementary regex.
    using _elementary = regex_parser_elementary_re<Str>;

    /// Check if * or + is at the end.
    using _star       = regex_parser_next_if<typename _elementary::rest, typename Str::value_type, '*'>;
    using _plus       = regex_parser_next_if<typename _elementary::rest, typename Str::value_type, '+'>;
    using _question   = regex_parser_next_if<typename _elementary::rest, typename Str::value_type, '?'>;


// Public Types
public:

    // TODO make more readable

    /// Regex matching rule.
    using rule = typename std::conditional<
        _star::match,
        rules::repeat_optional<typename _elementary::rule>,
        typename std::conditional<
            _plus::match,
            rules::repeat<typename _elementary::rule>,
            typename std::conditional<
                _question::match,
                rules::optional<typename _elementary::rule>,
                typename _elementary::rule
            >::type
        >::type
    >::type;

    /// Rest of the source string.
    using rest = typename std::conditional<
        _star::match,
        _star,
        typename std::conditional<
            _plus::match,
            _plus,
            typename std::conditional<
                _question::match,
                _question,
                _elementary
            >::type
        >::type
    >::type::rest;

};

/* ************************************************************************ */

/**
 * @brief Parser for REGEX simple RE.
 *
 * simple-RE = basic-RE { basic-RE }
 *
 * FIRST:  FIRST(basic-RE)
 * FOLLOW: { '|', '$', '\0' }
 */
template<typename Str, bool end = false>
struct regex_parser_simple_re
{

// Private Types
private:


    /// The first parsed item.
    using _item = regex_parser_basic_re<Str>;

    /// Following items.
    using _next = regex_parser_simple_re<typename _item::rest,
        character_at_str<0, typename _item::rest>::value == '|' ||
        character_at_str<0, typename _item::rest>::value == '$' ||
        character_at_str<0, typename _item::rest>::value == ')' ||
        character_at_str<0, typename _item::rest>::value == 0
    >;


// Public Types
public:


    /// Regex matching rule.
    using rule = typename rules::sequence_remove<
        typename rules::sequence_merge<rules::sequence<typename _item::rule>, typename _next::rule>::type
    >::type;

    /// Rest of the regex string.
    using rest = typename _next::rest;

};

/* ************************************************************************ */

/**
 * @brief Specialization of `regex_parser_simple_re`.
 */
template<typename Str>
struct regex_parser_simple_re<Str, true>
{

// Public Types
public:


    /// Regex matching rule.
    using rule = void;

    /// Rest of the source string.
    using rest = Str;

};

/* ************************************************************************ */

/**
 * @brief Parser for REGEX inner RE.
 *
 * inner-RE = simple-RE { "|" simple-RE }
 */
template<typename Str, bool end>
struct regex_parser_inner_re
{

// Private Types
private:


    /// The first parsed item.
    using _item = regex_parser_simple_re<Str>;

    /// Following items.
    using _next = regex_parser_inner_re<typename _item::rest,
        character_at_str<0, typename _item::rest>::value != '|'
    >;


// Public Types
public:


    /// Regex matching rule.
    // TODO change sequence to optional
    using rule = typename rules::sequence_remove<
        typename rules::sequence_merge<rules::sequence<typename _item::rule>, typename _next::rule>::type
    >::type;

    /// Rest of the regex string.
    using rest = typename _next::rest;

};

/* ************************************************************************ */

/**
 * @brief Parser for REGEX inner RE.
 *
 * inner-RE = simple-RE { "|" simple-RE }
 */
template<typename Str>
struct regex_parser_inner_re<Str, true>
{
// Public Types
public:


    using rule = void;
    using rest = Str;

};

/* ************************************************************************ */

/**
 * @brief Parser for REGEX RE.
 *
 * RE = [ "^" ] inner-RE [ "$" ]
 */
template<typename Str>
struct regex_parser_re
{

// Pre-conditions
private:


    static_assert(character_at_str<0, Str>::value != 0, "String cannot be empty");


// Private Types
private:


    /// Parse regex that begins with '^'.
    using _begin = regex_parser_next_if<Str, typename Str::value_type, '^'>;

    /// Parse inner regex.
    using _inner = regex_parser_inner_re<typename _begin::rest>;

    /// Parse end character.
    using _end   = regex_parser_next_if<typename _inner::rest, typename Str::value_type, '$'>;


// Post-conditions
private:

    //_type_printer<typename _end::rest> a;
    //static_assert(character_at_str<0, typename _end::rest>::value == 0, "There are some characters at the end of the regex");


// Public Types
public:


    // RULE
    using rule = typename std::conditional<
        _begin::match,
        typename std::conditional<
            _end::match,
            rules::begin_end<typename _inner::rule>,
            rules::begin<typename _inner::rule>
        >::type,
        typename std::conditional<
            _end::match,
            rules::end<typename _inner::rule>,
            typename _inner::rule
        >::type
    >::type;

};

/* ************************************************************************ */

/**
 * @brief Template regular expression.
 *
 * Structure generates from sequence of characters in template arguments a
 * type constructed from rules.
 *
 * template_regex<'a', '-', 'z'>::type is same as rules::range<'a', 'z'>.
 *
 * @tparam Chars Sequence of regular expression characters.
 */
template<typename CharT, CharT... Chars>
struct basic_regex
{
    //using rule = typename make_simple<typename build_seq<Chars...>::type>::type;
    using rule = typename regex_parser_re<basic_string<CharT, Chars...>>::rule;
};

/* ************************************************************************ */

/**
 * @brief Template regular expression.
 *
 * Structure generates from sequence of characters in template arguments a
 * type constructed from rules.
 *
 * template_regex<'a', '-', 'z'>::type is same as rules::range<'a', 'z'>.
 *
 * @tparam Chars Sequence of regular expression characters.
 */
template<typename CharT, typename Str>
struct basic_regex_str;

/* ************************************************************************ */

/**
 * @brief Template regular expression.
 *
 * Structure generates from sequence of characters in template arguments a
 * type constructed from rules.
 *
 * template_regex<'a', '-', 'z'>::type is same as rules::range<'a', 'z'>.
 *
 * @tparam Chars Sequence of regular expression characters.
 */
template<typename CharT, CharT... Chars>
struct basic_regex_str<CharT, basic_string<CharT, Chars...>>
    : basic_regex<CharT, Chars...>
{

};

/* ************************************************************************ */

/**
 * @brief Perform input range matching.
 *
 * @tparam Rule     Matching rule.
 * @tparam Iterator Source sequence iterator type.
 *
 * @param rule
 * @param first
 * @param last
 *
 * @return If input sequence is matched by rule.
 */
template<typename Regex, typename Iterator>
bool regex_match(const Regex& regex, Iterator first, const Iterator last)
{
    return rules::match<typename Regex::rule>(first, last);
}

/* ************************************************************************ */

/**
 * @brief Perform input range matching.
 *
 * @tparam Rule   Matching rule.
 * @tparam Source Source sequence.
 *
 * @param rule
 * @param source
 *
 * @return If input sequence is matched by rule.
 */
template<typename Regex, typename Source>
bool regex_match(const Regex& regex, Source&& source)
{
    return regex_match(regex, std::begin(source), std::end(source));
}

/* ************************************************************************ */

/**
 * @brief Regular expression for char string.
 *
 * @tparam Chars Regular expression characters.
 */
template<char... Chars>
using regex = basic_regex<char, Chars...>;

/* ************************************************************************ */

/**
 * @brief Regular expression for wchar_t string.
 *
 * @tparam Chars Regular expression characters.
 */
template<wchar_t... Chars>
using wregex = basic_regex<wchar_t, Chars...>;

/* ************************************************************************ */

/**
 * @brief Regular expression for UTF-8 char string.
 *
 * @tparam Chars Regular expression characters.
 */
template<char... Chars>
using u8regex = basic_regex<char, Chars...>;

/* ************************************************************************ */

/**
 * @brief Regular expression for UTF-16 char string.
 *
 * @tparam Chars Regular expression characters.
 */
template<char16_t... Chars>
using u16regex = basic_regex<char16_t, Chars...>;

/* ************************************************************************ */

/**
 * @brief Regular expression for UTF-32 char string.
 *
 * @tparam Chars Regular expression characters.
 */
template<char32_t... Chars>
using u32regex = basic_regex<char32_t, Chars...>;

/* ************************************************************************ */

/**
 * @brief Create template regular expression type.
 *
 * @param str Regular expression string.
 *
 * @return Rules type.
 */
#define make_regex_t(str) \
    basic_regex_str<TEMPLATE_STRING_CHAR_TYPE(str), make_string_t(str)>

/* ************************************************************************ */

/**
 * @brief Create template regular expression object.
 *
 * @param str Regular expression string.
 *
 * @return Rules object.
 */
#define make_regex(str) make_regex_t(str){}

/* ************************************************************************ */

}

/* ************************************************************************ */
