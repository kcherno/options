#include <string_view>
#include <algorithm>
#include <stdexcept>
#include <iterator>

#include <cctype>

#include "options/grammar.hpp"
#include "options/option.hpp"

namespace
{
    inline bool is_long_option_name(std::string_view name) noexcept
    {
        if (name.size() > 3 && name.starts_with("--") && std::isalpha(name[2]))
        {
            constexpr auto predicate = [](char ch)
            {
                return ch == '-' || std::isalnum(ch);
            };

            auto substr = name.substr(3);

            if (std::ranges::all_of(substr, predicate))
            {
                return not substr.contains("--") &&
                    std::isalnum(substr.back());
            }
        }

        return false;
    }

    inline bool is_short_option_name(std::string_view name) noexcept
    {
        return name.size() == 2 && name[0] == '-' && std::isalnum(name[1]);
    }
}

void options::grammar::add_option(const option& option)
{
    verify(option);

    auto iterator_1 = map_.find(option.short_name);
    auto iterator_2 = map_.find(option.long_name);

    if (iterator_1 != map_.end() || iterator_2 != map_.end())
    {
        throw std::logic_error {
            std::string("grammar::add_option: ")
                .append("option [")
                .append(option.representation())
                .append("] has already been added as [")
                .append(iterator_1 == map_.end() ?
                    iterator_2->second->representation() :
                    iterator_1->second->representation())
                .append("]")
        };
    }

    list_.push_back(option);

    if (not option.short_name.empty())
    {
        map_[option.short_name] = std::prev(list_.end());
    }

    if (not option.long_name.empty())
    {
        map_[option.long_name] = std::prev(list_.end());
    }
}

void options::grammar::verify(const option& option)
{
    if (option.short_name.empty() && option.long_name.empty())
    {
        throw std::logic_error {
            "grammar::verify: "
            "the option must have at least a short or long name"
        };
    }

    if ((not option.short_name.empty()) &&
        (not is_short_option_name(option.short_name)))
    {
        throw std::logic_error {
            "grammar::verify: "
            "the short option name must match the pattern ^-[a-zA-Z0-9]$"
        };
    }

    if ((not option.long_name.empty()) &&
        (not is_long_option_name(option.long_name)))
    {
        throw std::logic_error {
            "grammar::verify: "
            "the long option name must match the pattern "
            "^--[a-zA-Z][-a-zA-Z0-9]*[a-zA-Z]$"
        };
    }
}
