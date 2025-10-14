#pragma once

namespace LootDrop {
    class Hooks {
    public:
        static void Install();
        
    private:
        static void ProcessDeathLoot(RE::Actor* a_actor, RE::TESObjectREFR* a_container);
        static bool ShouldDropItem(RE::TESBoundObject* a_item, RE::Actor* a_actor);
        
        // Hook target
        static inline REL::Relocation<decltype(&ProcessDeathLoot)> _ProcessDeathLoot;
    };
}