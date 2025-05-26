#pragma once

#include <unordered_set>

namespace Config
{

    struct TransparentHash
    {
        using is_transparent = void; // enables heterogeneous lookup
        std::size_t operator()(std::string_view sv) const
        {
            return std::hash<std::string_view>{}(sv);
        }
    };

    struct TransparentEqual
    {
        using is_transparent = void;
        bool operator()(std::string_view lhs, std::string_view rhs) const { return lhs == rhs; }
    };

    void                                  ReadConfig();
    inline bool                           removeEffectShaderEdges                          = true;
    inline std::unordered_set<RE::FormID> excludedForms                                    = {};
    inline std::unordered_set<std::string, TransparentHash, TransparentEqual> excludedMods = {};
} // namespace Config
