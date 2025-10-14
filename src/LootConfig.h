#pragma once
#include <random>

namespace LootDrop {
    class LootConfig {
    public:
        static LootConfig* GetSingleton() {
            static LootConfig singleton;
            return &singleton;
        }
        
        void Load();
        
        float GetWeaponDropChance() const { return weaponDropChance; }
        float GetArmorDropChance() const { return armorDropChance; }
        float GetMiscDropChance() const { return miscDropChance; }
        float GetAlchemyDropChance() const { return alchemyDropChance; }
        float GetBookDropChance() const { return bookDropChance; }
        float GetDefaultDropChance() const { return defaultDropChance; }
        bool UseLevelScaling() const { return levelScaling; }
        
        std::mt19937& GetRNG() { return rng; }
        
    private:
        LootConfig() : rng(std::random_device{}()) {}
        
        float weaponDropChance = 0.3f;
        float armorDropChance = 0.25f;
        float miscDropChance = 0.5f;
        float alchemyDropChance = 0.4f;
        float bookDropChance = 0.6f;
        float defaultDropChance = 0.35f;
        bool levelScaling = true;
        
        std::mt19937 rng;
    };
}