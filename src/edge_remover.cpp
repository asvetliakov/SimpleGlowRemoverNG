#include "edge_remover.h"

#include "config.h"

void
EdgeRemover::Initialize()
{
    if (!Config::removeEffectShaderEdges)
    {
        logger::info("Effect shaders edge removal is disabled, nothing to do");
        return;
    }
    auto dataHandler = RE::TESDataHandler::GetSingleton();
    for (auto effect : dataHandler->GetFormArray<RE::TESEffectShader>())
    {
        if (!effect)
        {
            continue;
        }

        auto file     = effect->GetFile(0);
        auto filename = file ? file->GetFilename() : "";

        if (Config::excludedForms.contains(effect->formID) ||
            (!filename.empty() && Config::excludedMods.contains(filename)))
        {
            logger::info("Excluding effect {}: {} from edge remover",
                         filename,
                         std::format("{:08X}", effect->GetFormID()));
            continue;
        }

        logger::info("Clearing edges from effect shader {}: {}",
                     filename,
                     std::format("{:08X}", effect->GetFormID()));

        effect->data.fillTextureEffectPersistentAlphaRatio = 0.f;
        effect->data.fillTextureEffectFullAlphaRatio       = 0.f;
        effect->data.edgeEffectPersistentAlphaRatio        = 0.f;
        effect->data.edgeEffectFullAlphaRatio              = 0.f;
    }
}
