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

// C++
#include <type_traits>
#include <utility>
#include <iostream>

/* ************************************************************************ */

namespace template_regex {

/* ************************************************************************ */

/**
 * @brief Returns character from string at given position.
 *
 * @tparam CharT Character type.
 * @tparam N     String literal size.
 *
 * @param str String literal.
 * @param i   Required string position. If value is greater than
 *            string length, zero is returned.
 *
 * @return Character at i-th position.
 */
template<typename CharT, unsigned N>
constexpr CharT char_at(const CharT (&str)[N], unsigned i) noexcept
{
    return i < N ? str[i] : CharT{};
}

/* ************************************************************************ */

/**
 * @brief Returns character from string at given position.
 *
 * @tparam I     Character position.
 * @tparam CharT Character type.
 * @tparam N     String literal size.
 *
 * @param str String literal.
 *
 * @return Character at i-th position.
 */
template<unsigned I, typename CharT, unsigned N>
constexpr CharT char_at(const CharT (&str)[N]) noexcept
{
    return I < N ? str[I] : CharT{};
}

/* ************************************************************************ */

/**
 * @brief Returns N-th character from variadic pack.
 *
 * @tparam N      Required character position.
 * @tparam CharT  Character type.
 * @tparam Values A list of characters.
 */
template<unsigned N, typename CharT, CharT... Values>
struct character_at
{
    /// N-th character.
    static constexpr CharT value = std::conditional<
        (sizeof...(Values) > 0),
        std::conditional<true, character_at<N - 1, CharT, Values...>, void>,
        std::integral_constant<CharT, CharT{}>
    >::type::value;
};

/* ************************************************************************ */

/**
 * @brief Returns N-th character from variadic pack (specialization).
 *
 * @tparam N      Required character position.
 * @tparam CharT  Character type.
 * @tparam Value  The first character.
 * @tparam Values A list of characters.
 */
template<unsigned N, typename CharT, CharT Value, CharT... Values>
struct character_at<N, CharT, Value, Values...>
{
    static constexpr CharT value = std::conditional<
        (N > 0),
        std::integral_constant<CharT, character_at<N - 1, CharT, Values...>::value>,
        std::integral_constant<CharT, Value>
    >::type::value;
};

/* ************************************************************************ */

/**
 * @brief Container for character sequence.
 *
 * This structure is able to store variadic sequence of characters and can
 * be used in templates specialization for separation of parameter packs.
 *
 * @tparam CharT Character type.
 * @tparam Chars Sequence of characters.
 */
template<typename CharT, CharT... Chars>
struct basic_string
{
    using value_type = CharT;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;


    /**
     * @brief Checks if the string has no characters.
     *
     * @return true if the string is empty, false otherwise.
     */
    static constexpr bool empty() noexcept
    {
        return size() == 0;
    }


    /**
     * @brief Returns the number of CharT elements in the string.
     *
     * @return The number of CharT elements in the string.
     */
    static constexpr size_type size() noexcept
    {
        return sizeof...(Chars);
    }


    /**
     * @brief Returns the number of CharT elements in the string.
     *
     * @return The number of CharT elements in the string.
     */
    static constexpr size_type length() noexcept
    {
        return size();
    }


    /**
     * @brief Returns character at given position.
     *
     * @tparam I Character position.
     *
     * @return
     */
    template<size_t I>
    static constexpr CharT at() noexcept
    {
        return character_at<I, CharT, Chars...>::value;
    }
};

/* ************************************************************************ */

/**
 * @brief Returns character at given position.
 *
 * @tparam I   Character position.
 * @tparam Str Source string.
 */
template<unsigned I, class Str>
struct character_at_str;

/* ************************************************************************ */

/**
 * @brief Returns character at given position.
 *
 * @tparam I Character position.
 */
template<unsigned I, typename CharT, CharT... Chars>
struct character_at_str<I, basic_string<CharT, Chars...>>
    : character_at<I, CharT, Chars...>
{
    // Nothing
};

/* ************************************************************************ */

/**
 * @brief Appends character to the string.
 *
 * @tparam CharT Character type.
 * @tparam Str   String type.
 * @tparam Char  Character to append.
 */
template<typename CharT, typename Str, CharT Char>
struct basic_string_append;

/* ************************************************************************ */

/**
 * @brief Appends character to the string.
 *
 * @tparam CharT Character type.
 * @tparam Str   String type.
 * @tparam Char  Character to append.
 */
template<typename CharT, CharT... Chars, CharT Char>
struct basic_string_append<CharT, basic_string<CharT, Chars...>, Char>
{
    using type = basic_string<CharT, Chars..., Char>;
};

/* ************************************************************************ */

/**
 * @brief Template string builder.
 *
 * @tparam N     Number of valid characters.
 * @tparam CharT Character type.
 * @tparam Str   String type.
 * @tparam Chars Characters.
 */
template<unsigned N, typename CharT, typename Str, CharT... Chars>
struct basic_string_builder;

/* ************************************************************************ */

/**
 * @brief Template string builder.
 *
 * @tparam N     Number of valid characters.
 * @tparam CharT Character type.
 * @tparam Str   String type.
 * @tparam CharX Transfered character.
 * @tparam Chars Characters.
 */
template<unsigned N, typename CharT, typename Str, CharT CharX, CharT... Chars>
struct basic_string_builder<N, CharT, Str, CharX, Chars...>
{
    static_assert((sizeof...(Chars) + 1 > N), "No more characters");

    /// Trimmed string type.
    /// This is only one solution that I found.
    /// Recursive solution with stop specialization doesn't work due
    /// ambiguity.
    /// Inner string builder cannot have "type" directly because that
    /// causes template instantiation that is not needed (and causes
    /// an error).
    using type = typename std::conditional<
        (N > 0),
        basic_string_builder<N - 1, CharT, typename basic_string_append<CharT, Str, CharX>::type, Chars...>,
        // Yeah ugly, but I needed something that have a member type named 'type'.
        std::conditional<true, Str, void>
    >::type::type;
};

/* ************************************************************************ */

/**
 * @brief Template string builder.
 *
 * @tparam N     Number of valid characters.
 * @tparam CharT Character type.
 * @tparam Str1  String type.
 * @tparam Str2  String type.
 */
template<unsigned N, typename CharT, typename Str1, typename Str2>
struct basic_string_builder_str;

/* ************************************************************************ */

/**
 * @brief Template string builder.
 *
 * @tparam N     Number of valid characters.
 * @tparam CharT Character type.
 * @tparam Str   String type.
 * @tparam CharX Transfered character.
 * @tparam Chars Characters.
 */
template<unsigned N, typename CharT, typename Str1, CharT... Chars>
struct basic_string_builder_str<N, CharT, Str1, basic_string<CharT, Chars...>>
    : basic_string_builder<N, CharT, Str1, Chars...> {};

/* ************************************************************************ */

/**
 * @brief Template string builder (skip N characters).
 *
 * @tparam N     Number of invalid characters.
 * @tparam CharT Character type.
 * @tparam Str   String type.
 * @tparam Chars Characters.
 */
template<unsigned N, typename CharT, typename Str, CharT... Chars>
struct basic_string_builder_rest;

/* ************************************************************************ */

/**
 * @brief Template string builder (skip N characters).
 *
 * @tparam N     Number of invalid characters.
 * @tparam CharT Character type.
 * @tparam Str   String type.
 * @tparam CharX Transfered character.
 * @tparam Chars Characters.
 */
template<unsigned N, typename CharT, typename Str, CharT CharX, CharT... Chars>
struct basic_string_builder_rest<N, CharT, Str, CharX, Chars...>
{
    static_assert((sizeof...(Chars) + 1 >= N), "No more characters");

    /// String type without N character at the begin.
    using type = typename std::conditional<
        (N > 0),
        basic_string_builder_rest<N - 1, CharT, typename basic_string_append<CharT, Str, CharX>::type, Chars...>,
        // Yeah ugly, but I needed something that have a member type named 'type'.
        std::conditional<true, basic_string<CharT, CharX, Chars...>, void>
    >::type::type;
};

/* ************************************************************************ */

/**
 * @brief Template string builder (skip N characters).
 *
 * @tparam N     Number of invalid characters.
 * @tparam CharT Character type.
 * @tparam Str   String type.
 */
template<unsigned N, typename CharT, typename Str>
struct basic_string_builder_rest<N, CharT, Str>
{
    /// No more characters.
    using type = basic_string<CharT>;
};

/* ************************************************************************ */

/**
 * @brief Template string builder (skip N characters).
 *
 * @tparam N     Number of invalid characters.
 * @tparam CharT Character type.
 * @tparam Str   String type.
 * @tparam Chars Characters.
 */
template<unsigned N, typename CharT, typename Str1, typename Str2>
struct basic_string_builder_rest_str;

/* ************************************************************************ */

/**
 * @brief Template string builder (skip N characters).
 *
 * @tparam N     Number of invalid characters.
 * @tparam CharT Character type.
 * @tparam Str   String type.
 * @tparam CharX Transfered character.
 * @tparam Chars Characters.
 */
template<unsigned N, typename CharT, typename Str1, CharT... Chars>
struct basic_string_builder_rest_str<N, CharT, Str1, basic_string<CharT, Chars...>>
    : basic_string_builder_rest<N, CharT, Str1, Chars...> {};

/* ************************************************************************ */

/**
 * @brief Separates string into two parts.
 *
 * @tparam N     Number of characters from string start.
 * @tparam CharT Character type.
 * @tparam Str   Source string.
 */
template<unsigned N, typename CharT, typename Str>
struct basic_string_split;

/* ************************************************************************ */

/**
 * @brief Separates string into two parts.
 *
 * @tparam N     Number of characters from string start.
 * @tparam CharT Character type.
 * @tparam Str   Source string.
 */
template<unsigned N, typename CharT, CharT... Chars>
struct basic_string_split<N, CharT, basic_string<CharT, Chars...>>
{
    /// The first part (N characters) of the string
    using first = typename basic_string_builder<N, CharT, basic_string<CharT>, Chars...>::type;

    // TODO second part
};

/* ************************************************************************ */

/**
 * @brief Finds character in the string and returns it's position.
 */
template<typename CharT, typename Str, CharT Char>
struct basic_string_find
{
    static constexpr unsigned value = 0;
};

/* ************************************************************************ */

/**
 * @brief Alias for char string.
 *
 * @tparam Chars String characters.
 */
template<char... Chars>
using string = basic_string<char, Chars...>;

/* ************************************************************************ */

/**
 * @brief Alias for wchar_t string.
 *
 * @tparam Chars String characters.
 */
template<wchar_t... Chars>
using wstring = basic_string<wchar_t, Chars...>;

/* ************************************************************************ */

/**
 * @brief Alias for UTF-8 char string.
 *
 * @tparam Chars String characters.
 */
template<char... Chars>
using u8string = basic_string<char, Chars...>;

/* ************************************************************************ */

/**
 * @brief Alias for UTF-16 char string.
 *
 * @tparam Chars String characters.
 */
template<char16_t... Chars>
using u16string = basic_string<char16_t, Chars...>;

/* ************************************************************************ */

/**
 * @brief Alias for UTF-32 char string.
 *
 * @tparam Chars String characters.
 */
template<char32_t... Chars>
using u32string = basic_string<char32_t, Chars...>;

/* ************************************************************************ */

/* Helper macros to separate string into characters */
#define TEMPLATE_STRING_GET_C1(str, i) \
    char_at(str, i)
#define TEMPLATE_STRING_GET_C2(str, i) \
    TEMPLATE_STRING_GET_C1(str, i),  TEMPLATE_STRING_GET_C1(str, i + 1)
#define TEMPLATE_STRING_GET_C4(str, i) \
    TEMPLATE_STRING_GET_C2(str, i),  TEMPLATE_STRING_GET_C2(str, i + 2)
#define TEMPLATE_STRING_GET_C8(str, i) \
    TEMPLATE_STRING_GET_C4(str, i),  TEMPLATE_STRING_GET_C4(str, i + 4)
#define TEMPLATE_STRING_GET_C16(str, i) \
    TEMPLATE_STRING_GET_C8(str, i),  TEMPLATE_STRING_GET_C8(str, i + 8)
#define TEMPLATE_STRING_GET_C32(str, i) \
    TEMPLATE_STRING_GET_C16(str, i), TEMPLATE_STRING_GET_C16(str, i + 16)
#define TEMPLATE_STRING_GET_C64(str, i) \
    TEMPLATE_STRING_GET_C32(str, i), TEMPLATE_STRING_GET_C32(str, i + 32)

/* ************************************************************************ */

/**
 * @brief Default string literal expansion macro.
 */
#ifndef TEMPLATE_STRING_EXPAND
#define TEMPLATE_STRING_EXPAND(str) TEMPLATE_STRING_GET_C64(str, 0)
#endif

/* ************************************************************************ */

/**
 * @brief Detect string character type.
 *
 * @param str String literal.
 *
 * @return Character type.
 */
#define TEMPLATE_STRING_CHAR_TYPE(str) decltype(char_at(str, 0))

/* ************************************************************************ */

/**
 * @brief Creates template string object type.
 *
 * This is just a macro that will expand into very ugly code.
 *
 * @param str String literal.
 *
 * @return String type.
 *
 * @note Currently this is based on macro expansion and allows only
 * up to 64 characters strings. If you need create string with more
 * characters define own TEMPLATE_STRING_EXPAND macro.
 */
#define make_string_t(str) \
    basic_string_builder< \
        sizeof(str) / sizeof(TEMPLATE_STRING_CHAR_TYPE(str)) - 1, \
        TEMPLATE_STRING_CHAR_TYPE(str), \
        basic_string<TEMPLATE_STRING_CHAR_TYPE(str)>, \
        TEMPLATE_STRING_EXPAND(str) \
    >::type

/* ************************************************************************ */

/**
 * @brief Creates template string object.
 *
 * @param str String literal.
 *
 * @return String object.
 *
 * @see `make_template_string_type`
 */
#define make_string(str) make_string_t(str){}

/* ************************************************************************ */

// This doesn't work because string literal cannot be forwared :(
#if 0
template<typename CharT, unsigned N, unsigned... Ints>
constexpr auto make_string_apply(const CharT (&str)[N], std::integer_sequence<unsigned, Ints...>)
{
    return basic_string<CharT, str[Ints]...>{};
}

/* ************************************************************************ */

template<typename CharT, unsigned N>
constexpr auto make_string(const CharT (&str)[N])
{
    return make_string_apply(str, std::make_integer_sequence<unsigned, N>{});
}
#endif

/* ************************************************************************ */

/**
 * @brief Prints a template string into stream.
 *
 * @param os  Output stream.
 * @param str Printed string.
 *
 * @return Output stream.
 */
template<typename CharT, CharT... Chars>
std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& os, const basic_string<CharT, Chars...>& str) noexcept
{
    constexpr CharT buf[] = {Chars..., CharT()};
    os << buf;
    return os;
}

/* ************************************************************************ */

}

/* ************************************************************************ */
