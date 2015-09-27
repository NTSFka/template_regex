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
 * @file rules.hpp
 *
 * This header contains multiple structures that can be used to match input
 * within iterator range. Those rules can be aggregate into more complex
 * rules that are equivalent to regular expressions (for characters) or
 * EBNF rules (for grammar).
 *
 * Rules are constructed in compile-time and they do not have any runtime
 * footprint. All rules' functions must be called statically.
 *
 * @todo Test for out-of-range in `val` and `range`.
 * @todo Allow to matching not from input begin (i.e. remove implicit ^...)
 */

/* ************************************************************************ */

// C++
#include <iterator>
#include <type_traits>
#include <tuple>
#include <utility>
#include <functional>

/* ************************************************************************ */

namespace template_regex {
namespace rules {

/* ************************************************************************ */

/**
 * @brief Sequence of integers.
 *
 * @tparam I A list of integers.
 *
 * @todo Replace with std::integer_sequence in C++14
 */
template<int... I>
struct int_seq { };

/* ************************************************************************ */

/**
 * @brief Sequence number range generator. The generated range is [S, E - 1].
 *
 * @tparam S Starting number.
 * @tparam E Number after last generated.
 * @tparam I... Helper list for recursive generating numbers.
 *
 * @see make_int_seq
 *
 * The idea is based on link below.
 *
 * @link http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer/7858971#7858971
 */
template<int S, int E, int... I>
struct sequence_generator
{
    static_assert(S <= E, "S <= E !!!");

    /// Generated `int_seq` type.
    using type = typename std::conditional<
        S >= E,
        // This is required for prevent instantization of the second type
        // that causes recursive instantizations
        std::conditional<true, int_seq<I...>, void>,
        sequence_generator<S, E - 1, E - 1, I...>
    >::type::type;
};

/* ************************************************************************ */

// Just some tests
static_assert(std::is_same<typename sequence_generator<0, 0>::type, int_seq<>>::value, "Fail [0, 0]");
static_assert(std::is_same<typename sequence_generator<0, 1>::type, int_seq<0>>::value, "Fail [0, 1]");
static_assert(std::is_same<typename sequence_generator<5, 10>::type, int_seq<5, 6, 7, 8, 9>>::value, "Fail [5, 10]");

/* ************************************************************************ */

/**
 * @brief Alias type for generating numbers range.
 *
 * @tparam S Starting number.
 * @tparam E Number after last generated.
 */
template<int S, int E>
using make_int_seq = typename sequence_generator<S, E>::type;

/* ************************************************************************ */

/**
 * @brief Base class for all rules.
 *
 * @tparam Rule Matching rule or derived class.
 */
template<typename Rule>
struct matcher
{

    /**
     * @brief Match the input range with Rule.
     *
     * @tparam Iterator Type of the input iterator. Requires InputIterator.
     * @tparam Outputs A list of output iterators used by `capture`.
     *
     * @param it An iterator to the first value.
     * @param end An iterator to the value following the last valid
     * value.
     * @param out An optional list of output iterators.
     *
     * @return If input was matched.
     */
    template<typename Iterator, typename... Outputs>
    static bool match(Iterator it, const Iterator end, Outputs... outs)
    {
        static_assert(Rule::output_count == sizeof...(Outputs),
            "Outputs count must match with output_count");

        return match_ref(it, end, outs...);
    }


    /**
     * @brief Match the input range with Rule.
     *
     * @tparam Iterator Type of the input iterator. Requires InputIterator.
     * @tparam Outputs A list of output iterators used by `capture`.
     *
     * @param it An iterator to the first value.
     * @param end An iterator to the value following the last valid
     * value.
     * @param out An optional list of output iterators.
     *
     * @return If input was matched.
     */
    template<typename Range, typename... Outputs>
    static bool match(Range&& r, Outputs... outs)
    {
        auto it = std::begin(r);
        return match_ref(it, std::end(r), outs...);
    }


    /**
     * @brief Match the input range with Rule.
     *
     * @tparam Iterator Type of the input iterator. Requires InputIterator.
     * @tparam Outputs A list of output iterators used by `capture`.
     *
     * @param it A reference to starting iterator. At the end this iterator
     * refers to the last unmatched value or is equivalent to the end
     * iterator.
     * @param end An iterator to the value following the last valid
     * value.
     * @param out An optional list of output iterators.
     *
     * @return If input was matched.
     */
    template<typename Iterator, typename... Outputs>
    static bool match_ref(Iterator& it, const Iterator end, Outputs... outs)
    {
        static_assert(Rule::output_count == sizeof...(Outputs),
            "Outputs count must match with output_count");

        return Rule::match_impl(it, end, outs...);
    }


    /**
     * @brief Match the whole input range with Rule.
     *
     * @tparam Iterator Type of the input iterator. Requires InputIterator.
     * @tparam Outputs A list of output iterators used by `capture`.
     *
     * @param it An iterator to the first value.
     * @param end An iterator to the value following the last valid
     * value.
     * @param out An optional list of output iterators.
     *
     * @return If the whole input was matched.
     */
    template<typename Iterator, typename... Outputs>
    static bool match_all(Iterator it, const Iterator end, Outputs... outs)
    {
        return match_all_ref(it, end, outs...);
    }


    /**
     * @brief Match the whole input range with Rule.
     *
     * @tparam Iterator Type of the input iterator. Requires InputIterator.
     * @tparam Outputs A list of output iterators used by `capture`.
     *
     * @param it An iterator to the first value.
     * @param end An iterator to the value following the last valid
     * value.
     * @param out An optional list of output iterators.
     *
     * @return If the whole input was matched.
     */
    template<typename Range, typename... Outputs>
    static bool match_all(Range&& r, Outputs... outs)
    {
        auto it = std::begin(r);
        return match_all_ref(it, std::end(r), outs...);
    }


    /**
     * @brief Match the whole input range with Rule.
     *
     * @tparam Iterator Type of the input iterator. Requires InputIterator.
     * @tparam Output A list of output iterators used by `capture`.
     *
     * @param it A reference to starting iterator. At the end this iterator
     * refers to the last unmatched value or is equivalent to the end
     * iterator.
     * @param end An iterator to the value following the last valid
     * value.
     * @param out An optional list of output iterators.
     *
     * @return If the whole input was matched.
     */
    template<typename Iterator, typename... Outputs>
    static bool match_all_ref(Iterator& it, const Iterator end, Outputs... outs)
    {
        static_assert(Rule::output_count == sizeof...(Outputs),
            "Outputs count must match with output_count");

        return Rule::match_impl(it, end, outs...) && (it == end);
    }

};

/* ************************************************************************ */

/**
 * @brief List of rules.
 */
template<typename Rule, typename... Rules>
struct list
{
    template<typename Out, typename Iterator>
    static Out match(Iterator& it, const Iterator end, Out def)
    {
        if (Rule::match(it, end))
        {
            return static_cast<Out>(Rule::value);
        }

        return list<Rules...>::match(it, end, def);
    }
};

/* ************************************************************************ */

/**
 * @brief List of rules.
 */
template<typename Rule>
struct list<Rule>
{
    template<typename Out, typename Iterator>
    static Out match(Iterator& it, const Iterator end, Out def)
    {
        if (Rule::match(it, end))
        {
            return static_cast<Out>(Rule::value);
        }

        return def;
    }
};

/* ************************************************************************ */

/**
 * @brief Rule list item.
 *
 * @param Rule Matching rule.
 * @param Value Return value.
 */
template<typename Rule, int Value>
struct item
{
    /// Return value.
    static constexpr int value = Value;

    template<typename Iterator>
    static bool match(Iterator& it, const Iterator end)
    {
        return Rule::match(it, end);
    }
};

/* ************************************************************************ */

template<typename InputIterator, typename OutputIterator>
struct capture_iterator
{
    using difference_type = typename std::iterator_traits<InputIterator>::difference_type;
    using value_type = typename std::iterator_traits<InputIterator>::value_type;
    using pointer = typename std::iterator_traits<InputIterator>::pointer;
    using reference = typename std::iterator_traits<InputIterator>::reference;
    using iterator_category = typename std::iterator_traits<InputIterator>::iterator_category;


    capture_iterator(InputIterator& in, OutputIterator out)
        : in_(in), out_(out), ref_(in)
    {}

    capture_iterator(const capture_iterator& rhs)
        : in_(rhs.ref_), out_(rhs.out_), ref_(in_)
    {}

    capture_iterator operator=(const capture_iterator& rhs)
    {
        in_ = rhs.ref_;
        out_ = rhs.out_;
        ref_ = in_;
    }

    capture_iterator(capture_iterator&& rhs)
        : in_(std::move(rhs.ref_)), out_(std::move(rhs.out_)), ref_(std::move(in_))
    {}

    capture_iterator operator=(capture_iterator&& rhs)
    {
        in_ = std::move(rhs.ref_);
        out_ = std::move(rhs.out_);
        ref_ = std::move(in_);
    }

    bool operator==(const capture_iterator& rhs) const
    {
        return ref_ == rhs.ref_;
    }

    bool operator!=(const capture_iterator& rhs) const
    {
        return ref_ != rhs.ref_;
    }

    reference operator*() const
    {
        return ref_.operator*();
    }

    pointer operator->() const
    {
        return ref_.operator->();
    }

    capture_iterator& operator++()
    {
        *out_ = *ref_;
        ++ref_;
        ++out_;

        return *this;
    }

    InputIterator in_;
    OutputIterator out_;
    InputIterator& ref_;
};

/* ************************************************************************ */

/**
 * @brief Captures content matched by given rule.
 *
 * Regular Expression: "(Rule)"
 */
template<typename Rule>
struct capture : matcher<capture<Rule>>
{
    static const unsigned output_count = Rule::output_count + 1;

    template<typename Iterator, typename OutputIterator, typename... Output>
    static bool match_impl(Iterator& it, Iterator end, OutputIterator out_it, Output... out)
    {
        capture_iterator<Iterator, OutputIterator> cit{it, out_it};
        const capture_iterator<Iterator, OutputIterator> cend{end, out_it};

        return Rule::match_ref(cit, cend, out...);
    }
};

/* ************************************************************************ */

/**
 * @brief Usefull struct for rule specialization (recursive stop rule).
 */
template<typename Rule>
struct forward
{

    /// A number of outputs in the rule.
    static const unsigned output_count = Rule::output_count;


    template<typename Iterator, typename... Output>
    static bool match_impl(Iterator& it, const Iterator end, Output... out)
    {
        return Rule::match_impl(it, end, out...);
    }
};

/* ************************************************************************ */

template<typename Parent>
struct value_matcher : matcher<Parent>
{

    /// A number of outputs in the rule.
    static const unsigned output_count = 0;


    template<typename Iterator, typename... Output>
    static bool match_impl(Iterator& it, const Iterator end, Output... out)
    {
        return Parent::is(*it) ? (++it, true) : false;
    }
};

/* ************************************************************************ */

/**
 * @brief Value matching.
 *
 * @tparam Value Matched integer value constant.
 */
template<int Value>
struct val : value_matcher<val<Value>>
{

    /// Stored value.
    static const int value = Value;


    /**
     * @brief Check if given value match class rule.
     *
     * @tparam Val Value type.
     *
     * @param val Tested value.
     * @param pos Tested value position - must be 0.
     */
    template<typename Val>
    static constexpr bool is(Val value)
    {
        return static_cast<int>(value) == Value;
    }

};

/* ************************************************************************ */

/**
 * @brief Value matching not.
 *
 * @param Value
 */
template<int Value>
struct val_not : value_matcher<val_not<Value>>
{

    /**
     * @brief Check if given value match class rule.
     *
     * @tparam Val Value type.
     *
     * @param val Tested value.
     */
    template<typename Val>
    static constexpr bool is(Val value)
    {
        return static_cast<int>(value) != Value;
    }

};

/* ************************************************************************ */

/**
 * @brief Match value in range: "{Low}-{High}"
 *
 * @param Low Lower value.
 * @param High High value.
 */
template<int Low, int High>
struct range  : value_matcher<range<Low, High>>
{

    /**
     * @brief Check if given value match class rule.
     *
     * @tparam Val Value type.
     *
     * @param val Tested value.
     */
    template<typename Val>
    static constexpr bool is(Val value)
    {
        return
            (static_cast<int>(value) >= Low) &&
            (static_cast<int>(value) <= High)
        ;
    }

};

/* ************************************************************************ */

/**
 * @brief Rule that does nothing.
 */
struct null_rule : matcher<null_rule>
{

    /// A number of outputs in the rule.
    static const unsigned output_count = 0;


    template<typename Iterator, typename... Output>
    static bool match_impl(Iterator& it, const Iterator end, Output... out)
    {
        return true;
    }
};

/* ************************************************************************ */

/**
 * @brief Match any value.
 */
struct any  : value_matcher<any>
{

    /**
     * @brief Check if given value match class rule.
     *
     * @tparam Val Value type.
     *
     * @param val Tested value.
     */
    template<typename Val>
    static constexpr bool is(Val value)
    {
        return true;
    }

};

/* ************************************************************************ */

/**
 * @brief Selects from multiple values {Rule1}|{Rule2}|{Rules...}
 *
 * @param Rule1 First rule.
 * @param Rules Next rules.
 */
template<typename Rule, typename... Rules>
struct alternative : matcher<alternative<Rule, Rules...>>
{

    /// A number of outputs in the rule.
    static const unsigned output_count = Rule::output_count;


    template<typename Iterator, typename... Output>
    static bool match_impl(Iterator& it, const Iterator end, Output... out)
    {
        return
            Rule::match_impl(it, end, out...) ||
            alternative<Rules...>::match_impl(it, end, out...)
        ;
    }
};

/* ************************************************************************ */

/**
 * @brief Stop specialization for alternative.
 *
 * @tparam Rule
 */
template<typename Rule>
struct alternative<Rule> : forward<Rule>, matcher<alternative<Rule>> { };

/* ************************************************************************ */

/**
 * @brief Selects from multiple values {Rule1}|{Rule2}|{Rules...}
 *
 * @param Rule1 First rule.
 * @param Rules Next rules.
 */
template<typename Rule, typename... Rules>
struct alternative_not : matcher<alternative_not<Rule, Rules...>>
{

    /// A number of outputs in the rule.
    static const unsigned output_count = Rule::output_count;


    template<typename Iterator, typename... Output>
    static bool match_impl(Iterator& it, const Iterator end, Output... out)
    {
        static_assert(!std::is_same<
            typename std::iterator_traits<Iterator>::iterator_category,
            std::input_iterator_tag
        >::value, "alternative_not rule require forward_iterator at least");

        // Store current position for ability to return
        Iterator tmp = it;

        // Rule matched, so result is: not matched.
        if (Rule::match_impl(tmp, end, out...))
            return false;

        return alternative_not<Rules...>::match_impl(it, end, out...);
    }
};

/* ************************************************************************ */

/**
 * @brief Stop specialization for alternative.
 *
 * @tparam Rule
 */
template<typename Rule>
struct alternative_not<Rule> : matcher<alternative_not<Rule>>
{

    /// A number of outputs in the rule.
    static const unsigned output_count = Rule::output_count;


    template<typename Iterator, typename... Output>
    static bool match_impl(Iterator& it, const Iterator end, Output... out)
    {
        static_assert(!std::is_same<
            typename std::iterator_traits<Iterator>::iterator_category,
            std::input_iterator_tag
        >::value, "alternative_not rule require forward_iterator at least");

        // Store current position for ability to return
        Iterator tmp = it;

        // Use temporary iterator
        return Rule::match_impl(tmp, end, out...) ? false : (++it, true);
    }
};

/* ************************************************************************ */

/**
 * @brief Optional repeat - none or more.
 *
 *  - Regular expressions: {Rule}*
 *  - EBNF: { <Rule> }
 *
 * @param Rule Base rule
 */
template<typename Rule>
struct repeat_optional : matcher<repeat_optional<Rule>>
{

    /// A number of outputs in the rule.
    static const unsigned output_count = Rule::output_count;


    template<typename Iterator, typename... Output>
    static bool match_impl(Iterator& it, const Iterator end, Output... out)
    {
        while (Rule::match_ref(it, end, out...))
            continue;

        return true;
    }
};

/* ************************************************************************ */

/**
 * @brief Repeat rule - one or more.
 *
 * - Regular expressions: {Rule}+
 * - EBNF: <Rule> { <Rule> }
 *
 * @param Rule Base rule
 */
template<typename Rule>
struct repeat : matcher<repeat<Rule>>
{

    /// A number of outputs in the rule.
    static const unsigned output_count = Rule::output_count;


    template<typename Iterator, typename... Output>
    static bool match_impl(Iterator& it, const Iterator end, Output... out)
    {
        unsigned int count = 0;

        while ((it != end) && Rule::match_impl(it, end, out...))
            ++count;

        return (count != 0);
    }
};

/* ************************************************************************ */

/**
 * @brief Optional rule: {Rule}?
 *
 * @param Rule Base rule
 */
template<typename Rule>
struct optional : matcher<optional<Rule>>
{

    /// A number of outputs in the rule.
    static const unsigned output_count = Rule::output_count;


    template<typename Iterator, typename... Output>
    static bool match_impl(Iterator& it, const Iterator end, Output... out)
    {
        Rule::match_ref(it, end, out...);

        return true;
    }
};

/* ************************************************************************ */

template<typename... Rules>
struct output_counter { };

/* ************************************************************************ */

template<typename Rule, typename... Rules>
struct output_counter<Rule, Rules...>
{
    /// A number of outputs in the rule.
    static const unsigned value = Rule::output_count + output_counter<Rules...>::value;
};

/* ************************************************************************ */

template<typename Rule>
struct output_counter<Rule>
{
    static const unsigned value = Rule::output_count;
};

/* ************************************************************************ */

template<typename... Rules>
struct sequence;

/* ************************************************************************ */

/**
 * @brief Match sequence of rules: {Rule1}{Rule2}{Rules...}
 *
 * @param Rule1 First rule.
 * @param Rules Next rules.
 */
template<typename Rule, typename... Rules>
struct sequence<Rule, Rules...> : matcher<sequence<Rule, Rules...>>
{

    /// A number of outputs in the rule.
    static const unsigned output_count = output_counter<Rule, Rules...>::value;


    template<typename Iterator, typename... Outputs>
    static bool match_impl(Iterator& it, const Iterator end, Outputs... outs)
    {
        // Split output by numbers of outputs
        // Make a tuple
        auto outputs = std::tuple<Outputs...>{outs...};

        return
            apply<Rule>(it, end, outputs, make_int_seq<0, Rule::output_count>()) &&
            apply<sequence<Rules...>>(it, end, outputs, make_int_seq<Rule::output_count, sizeof...(Outputs)>())
        //    Rule::match_impl(it, end, outs...) &&
        //    sequence<Rules...>::match_impl(it, end, outs...)
        ;
    }


    template<typename IRule, typename Iterator, typename Tuple, int... I>
    static auto apply(Iterator& it, const Iterator end, Tuple&& args, int_seq<I...>)
        -> decltype(IRule::match_impl(it, end, std::get<I>(std::forward<Tuple>(args))...))
    {
        return IRule::match_impl(it, end, std::get<I>(std::forward<Tuple>(args))...);
    }

};

/* ************************************************************************ */

/**
 * @brief Match sequence of rules: {Rule1}{Rule2}
 *
 * @param Rule2 Second rule.
 */
template<typename Rule>
struct sequence<Rule> : forward<Rule>, matcher<sequence<Rule>> { };

/* ************************************************************************ */

/**
 * @brief Structure used for merging `sequence`.
 *
 * @tparam Seq...
 */
template<typename... Seq>
struct sequence_merge;

/* ************************************************************************ */

/**
 * @brief Structure used for merging `sequence`.
 *
 * @tparam Seq1...
 * @tparam Seq2...
 */
template<typename... Seq1, typename... Seq2>
struct sequence_merge<sequence<Seq1...>, sequence<Seq2...>>
{
    using type = sequence<Seq1..., Seq2...>;
};

/* ************************************************************************ */

/**
 * @brief Structure used for merging `sequence`.
 *
 * @tparam Seq...
 */
template<typename... Seq1>
struct sequence_merge<void, sequence<Seq1...>>
{
    using type = sequence<Seq1...>;
};

/* ************************************************************************ */

/**
 * @brief Structure used for merging `sequence`.
 *
 * @tparam Seq...
 */
template<typename... Seq1>
struct sequence_merge<sequence<Seq1...>, void>
{
    using type = sequence<Seq1...>;
};

/* ************************************************************************ */

/**
 * @brief Structure used for merging `sequence`.
 *
 * @tparam Rule
 * @tparam Seq...
 */
template<typename Rule, typename... Seq1>
struct sequence_merge<Rule, sequence<Seq1...>>
{
    using type = sequence<Rule, Seq1...>;
};

/* ************************************************************************ */

/**
 * @brief Structure used for merging `sequence`.
 *
 * @tparam Rule
 * @tparam Seq...
 */
template<typename Rule, typename... Seq1>
struct sequence_merge<sequence<Seq1...>, Rule>
{
    using type = sequence<Seq1..., Rule>;
};

/* ************************************************************************ */

/**
 * @brief Removes sequences that contains only one rule.
 *
 * @param Seq Sequence.
 */
template<typename Seq>
struct sequence_remove
{
    using type = Seq;
};

/* ************************************************************************ */

/**
 * @brief Removes sequences that contains only one rule.
 *
 * @param Seq Sequence.
 */
template<typename Rule>
struct sequence_remove<sequence<Rule>>
{
    using type = Rule;
};

/* ************************************************************************ */

/**
 * @brief Removes `sequence` from alternative list.
 *
 * @param Rules A list of rules.
 */
template<typename... Rules>
struct sequence_remove<alternative<sequence<Rules...>>>
{
    using type = alternative<Rules...>;
};

/* ************************************************************************ */

/**
 * @brief Removes sequence from alternative list.
 *
 * @param Rules A list of rules.
 */
template<typename... Rules>
struct sequence_remove<alternative_not<sequence<Rules...>>>
{
    using type = alternative_not<Rules...>;
};

/* ************************************************************************ */

/**
 * @brief Special type for ignoring store rules.
 */
struct null_variable { };

/* ************************************************************************ */

/**
 * @brief Tests if T is variable type.
 *
 * @tparam T Tested type.
 */
template<typename T>
struct is_variable : std::false_type {};

/* ************************************************************************ */

/**
 * @brief Specialization for std::reference_wrapper.
 *
 * @tparam T Tested type.
 */
template<typename T>
struct is_variable<std::reference_wrapper<T>> : std::true_type {};

static_assert(is_variable<std::reference_wrapper<int>>::value, "Nop");

/* ************************************************************************ */

/**
 * @brief Tests if T is variable type.
 *
 * @tparam T Tested type.
 */
template<>
struct is_variable<null_variable> : std::true_type {};

/* ************************************************************************ */

template<typename Output, typename Value>
struct store_helper
{
    static void set(Output& out)
    {
        out = Value::val;
    }
};

/* ************************************************************************ */

template<typename T, typename Value>
struct store_helper<std::reference_wrapper<T>, Value>
{
    static void set(std::reference_wrapper<T> out)
    {
        out.get() = Value::value;
    }
};

/* ************************************************************************ */

template<typename Value>
struct store_helper<null_variable, Value>
{
    static void set(null_variable& out)
    {
        // Nothing
    }
};

/* ************************************************************************ */

/**
 * @brief Special rule that allows to store a special value to variable when
 * the matcher goes throught.
 *
 * @tparam Rule  Inner rule.
 * @tparam Value Value set to variable. Its not directly a value but a type
 * that must contains a member `value` with result value.
 */
template<typename Rule, typename Value>
struct store : matcher<store<Rule, Value>>
{

    /// A number of outputs in the rule.
    static const unsigned output_count = Rule::output_count + 1;


    template<typename Iterator, typename Output, typename... Outputs>
    static bool match_impl(Iterator& it, const Iterator end, Output out,
        Outputs... outs)
    {
        // Parameter type must be a variable
        static_assert(is_variable<Output>::value,
            "The first output must be a variable");

        // Store value into output
        store_helper<Output, Value>::set(out);

        // Just forward
        return Rule::match_impl(it, end, outs...);
    }
};

/* ************************************************************************ */

/**
 * @brief Match beginning of the source.
 *
 * @tparam Rule Inner rule.
 */
template<typename Rule>
struct begin : matcher<begin<Rule>>
{
    /// A number of outputs in the rule.
    static const unsigned output_count = Rule::output_count;


    template<typename Iterator, typename... Output>
    static bool match_impl(Iterator& it, const Iterator end, Output... out)
    {
        return Rule::match_impl(it, end);
    }
};

/* ************************************************************************ */

/**
 * @brief Match end of the source.
 *
 * @tparam Rule Inner rule.
 */
template<typename Rule>
struct end : matcher<end<Rule>>
{
    /// A number of outputs in the rule.
    static const unsigned output_count = Rule::output_count;


    template<typename Iterator, typename... Output>
    static bool match_impl(Iterator& it, const Iterator end, Output... out)
    {
        return Rule::match_impl(it, end) && (it == end);
    }
};

/* ************************************************************************ */

/**
 * @brief Exact match of the source.
 *
 * @tparam Rule Inner rule.
 */
template<typename Rule>
struct begin_end : matcher<begin_end<Rule>>
{
    /// A number of outputs in the rule.
    static const unsigned output_count = Rule::output_count;


    template<typename Iterator, typename... Output>
    static bool match_impl(Iterator& it, const Iterator end, Output... out)
    {
        return Rule::match_impl(it, end) && (it == end);
    }
};

/* ************************************************************************ */

/**
 * @brief Perform input range matching.
 *
 * @tparam Rule     Matching rule.
 * @tparam Iterator Source sequence iterator type.
 *
 * @param first
 * @param last
 *
 * @return If input sequence is matched by rule.
 */
template<typename Rule, typename Iterator>
bool match(Iterator first, const Iterator last)
{
    return Rule::match(first, last);
}

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
template<typename Rule, typename Iterator>
bool match(const Rule& rule, Iterator first, const Iterator last)
{
    return Rule::match(first, last);
}

/* ************************************************************************ */

/**
 * @brief Predefined type for whitespace matching.
 *
 * Matched values: ' ', '\t', '\n', '\r', '\v', '\f'.
 */
using whitespace = alternative<
    val<' '>,
    val<'\t'>,
    val<'\n'>,
    val<'\r'>,
    val<'\v'>,
    val<'\f'>
>;

/* ************************************************************************ */

/**
 * @brief Predefined type for matching new line.
 *
 * Matched value: '\n'.
 *
 * @note For matching new line independent on platform (or in binary mode),
 * use `newline_multiplatform`
 */
using newline = val<'\n'>;

/* ************************************************************************ */

/**
 * @brief Predefined type for matching new line independent on platform.
 *
 * Matched value: '\n', '\r', "\r\n"
 *
 * Matching regular expression: "\n|\r\n?"
 */
using newline_multiplatform = alternative<
    val<'\n'>,
    sequence<
        val<'\r'>,
        optional<val<'\n'>>
    >
>;

/* ************************************************************************ */

}
}

/* ************************************************************************ */
