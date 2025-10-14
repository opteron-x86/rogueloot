#include "Hooks.h"
#include "LootConfig.h"
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

namespace LootDrop {
    
    void Hooks::Install() {
        REL::Relocation<std::uintptr_t> target{ REL::RelocationID(36326, 37317) };
        
        auto& trampoline = SKSE::GetTrampoline();
        trampoline.create(64);
        
        _ProcessDeathLoot = trampoline.write_call<5>(target.address() + 0x3A8, ProcessDeathLoot);
        
        SKSE::log::info("Loot drop hooks installed");
    }
    
    void Hooks::ProcessDeathLoot(RE::Actor* a_actor, RE::TESObjectREFR* a_container) {
        if (!a_actor || !a_container) {
            return _ProcessDeathLoot(a_actor, a_container);
        }
        
        auto inv = a_actor->GetInventory();
        auto config = LootConfig::GetSingleton();
        
        for (const auto& [item, data] : inv) {
            auto& [count, entry] = data;
            
            if (!item || count <= 0) continue;
            
            // Always drop quest items and gold
            if (item->IsGold() || item->IsQuestObject()) {
                a_container->AddObjectToContainer(item, entry.get(), count, nullptr);
                continue;
            }
            
            // Check if item should drop
            if (ShouldDropItem(item, a_actor)) {
                // Determine quantity to drop
                std::int32_t dropCount = count;
                
                if (item->IsArmor() || item->IsWeapon()) {
                    dropCount = 1;
                } else if (count > 1) {
                    std::uniform_int_distribution<> dist(1, count);
                    dropCount = dist(config->GetRNG());
                }
                
                a_container->AddObjectToContainer(item, entry.get(), dropCount, nullptr);
            }
        }
    }
    
    bool Hooks::ShouldDropItem(RE::TESBoundObject* a_item, RE::Actor* a_actor) {
        auto config = LootConfig::GetSingleton();
        
        // Get drop chance based on item type
        float dropChance = config->GetDefaultDropChance();
        
        if (a_item->IsWeapon()) {
            dropChance = config->GetWeaponDropChance();
        } else if (a_item->IsArmor()) {
            dropChance = config->GetArmorDropChance();
        } else if (a_item->formType == RE::FormType::Misc) {
            dropChance = config->GetMiscDropChance();
        } else if (a_item->formType == RE::FormType::Ingredient || 
                   a_item->formType == RE::FormType::AlchemyItem) {
            dropChance = config->GetAlchemyDropChance();
        } else if (a_item->formType == RE::FormType::Book) {
            dropChance = config->GetBookDropChance();
        }
        
        // Adjust based on actor level difference if enabled
        if (config->UseLevelScaling()) {
            auto player = RE::PlayerCharacter::GetSingleton();
            if (player && a_actor) {
                std::int16_t levelDiff = player->GetLevel() - a_actor->GetLevel();
                if (levelDiff > 10) {
                    dropChance *= 0.5f;  // Halve drop rate for low level enemies
                } else if (levelDiff < -10) {
                    dropChance *= 1.5f;  // Increase drop rate for high level enemies
                }
            }
        }
        
        // Roll for drop
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(config->GetRNG()) <= dropChance;
    }
}