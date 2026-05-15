#pragma once

#include <string>

namespace options
{
    struct option final
    {
        std::string short_name    = {};
        std::string long_name     = {};
        std::string description   = {};
        bool        is_required   = false;
        bool        has_arguments = false;

        std::string representation() const
        {
            std::string representation;

            representation += short_name;

            representation += short_name.empty() ? "" : ", ";

            representation += long_name;

            if (long_name.empty())
            {
                representation.resize(short_name.size());
            }

            return representation;
        }
    };
}
