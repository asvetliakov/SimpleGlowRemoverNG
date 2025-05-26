#include "config.h"

#include <SimpleIni.h>

std::pair<std::string, std::string>
splitByPipe(const std::string& input)
{
    size_t pipePos = input.find('|');
    if (pipePos == std::string::npos)
    {
        // No pipe found — return input as first part, second is empty
        return {input, ""};
    }
    else
    {
        std::string part1 = input.substr(0, pipePos);
        std::string part2 = input.substr(pipePos + 1); // May be empty
        return {part1, part2};
    }
}

uint32_t
parseHexFlexible(const std::string& str)
{
    std::string s = str;

    // Remove "0x" or "0X" prefix if present
    if (s.size() >= 2 && (s[0] == '0') && (s[1] == 'x' || s[1] == 'X'))
    {
        s = s.substr(2);
    }

    return static_cast<uint32_t>(std::stoul(s, nullptr, 16));
}

void
Config::ReadConfig()
{
    constexpr auto path = L"Data/SKSE/Plugins/SimpleEdgeRemoverNG.ini";

    CSimpleIniA ini;
    ini.SetUnicode();
    ini.SetMultiKey(true);
    ini.LoadFile(path);

    removeEffectShaderEdges = ini.GetBoolValue("Settings", "removeEffectShaderEdges", true);

    std::list<CSimpleIniA::Entry> exclusions = {};
    if (!ini.GetAllValues("Exclusions", "item", exclusions))
    {
        logger::info("Unable to read exclusion, could be incorrect .ini");
        return;
    }

    auto dataHandler = RE::TESDataHandler::GetSingleton();

    for (auto& exclusion : exclusions)
    {
        std::string val(exclusion.pItem);

        if (val.empty())
        {
            continue;
        }

        auto [modName, formId] = splitByPipe(val);
        if (modName.empty())
        {
            continue;
        }

        // look for form all all forms
        if (!formId.empty() && !modName.empty())
        {
            auto uFormId = parseHexFlexible(formId);
            if (!uFormId)
            {
                logger::info("Exclusion {}: {} has invalid format", modName, formId);
                continue;
            }
            auto form = dataHandler->LookupFormID(uFormId, modName);
            if (form)
            {
                logger::info("Adding exclusion {}: {}", modName, formId);
                excludedForms.insert(form);
            }
        }
        else if (!modName.empty())
        {
            logger::info("Adding exclusion {}", modName);
            excludedMods.insert(std::string(modName));
        }
    }
}
