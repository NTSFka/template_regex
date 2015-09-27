
# Template Regex

> Library is experimental and shouldn't be used in production code.

Small regular expression library that compile regular expresions during compile time.
Other libraries parse expression string during runtime and create internal structures
that are used to match input string. This library parses expression string during
compile time and create code that is almost same as hand coded code for matching.
This solution allows to overperform other libraries but requires to know the regular
expression during compile time.

The library lacks support for matching results and can be used only checking if string
match given expression.

I've created rules part of this library as replacement for matching code
(hand written) in my tokenizers where were same code patterns. After some testing
I've realized the rules are de facto regular expressions and this offers me
an oportunity to white code that converts regular expression strings to those
rules.

The most of code have origin in 2012 and 2013 but I haven't time to finish and release it.

## Parts

The whole library are split into 3 main parts.

### Template String

This part handle storing string into C++ type.

```cpp
auto str = string<'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'>{};
```

There is a complex macro that create template string type from string literal.

```cpp
auto str = make_string("Hello World!");
```

### Rules

Rules contains most of the library functionality. Contains multiple classes that represent
regular expression rules for matching.

```cpp
val<'a'>                // Matches 'a' character
range<'a', 'z'>         // Matches character from range 'a' - 'z'
sequence<Rules...>      // Matches sequence of rules.
repeat<Rule>            // Matches one or more occurrence of inner rule.
repeat_optional<Rule>   // Matches zero or more occurrence of inner rule.
optional<Rule>          // Matches optional occurence of inner rule.
```

Mixing those types into one type it's possible to express any matching rule.

### Template Regex

Part only translate regular expression string (stored in template string) into rules.

```cpp
using namespace template_regex;
auto regex = make_regex("ab+c*");
```

The `regex` variable has type that represents given regular expression string:

```cpp
using namespace template_regex::rules;
sequence<val<'a'>, repeat<val<'b'>>, repeat_optional<val<'c'>>>
```

```cpp
using namespace template_regex;
std::string input = "abbc";
regex_match(regex, input);
```

## Performance

Because the library generate code during compile time that allows to optimize
source code and doesn't require runtime data to work, library overperform other
regular expression libraries.

This table shows how different regex libraries perform in simple matching test
of 1 000 000 strings read from file (randomly generated). Time is measured without
data loading and regular expression compiling. Source code of those tests
is available in 'perf' project.

| Library                | Identifier | Date   | Float  |
| ---------------------- | ---------- | ------ | ------ |
| Boost Regex            |     221 ms | 270 ms | 334 ms |
| Boost Xpression        |     526 ms | 579 ms | 561 ms |
| POSIX Regex            |     235 ms |  91 ms | 163 ms |
| Standard Library Regex |     284 ms | 211 ms | 292 ms |
| Java Regex             |     348 ms | 164 ms | 192 ms |
| Handwritten            |      41 ms |   9 ms |  11 ms |
| Template Regex         |      30 ms |   9 ms |  11 ms |

> Intel i7-4770S @ 3.1 GHz, Clang-3.6 -O3, Linux Mint 17.1 x64
