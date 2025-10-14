#include <SKSE/SKSE.h>
#include <RE/Skyrim.h>
#include "Hooks.h"
#include "LootConfig.h"

namespace {
    void MessageHandler(SKSE::MessagingInterface::Message* a_msg) {
        switch (a_msg->type) {
        case SKSE::MessagingInterface::kDataLoaded:
            LootDrop::Hooks::Install();
            break;
        }
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse) {
    SKSE::Init(a_skse);
    
    SKSE::GetMessagingInterface()->RegisterListener(MessageHandler);
    
    LootDrop::LootConfig::GetSingleton()->Load();
    
    return true;
}