#include "LootConfig.h"
#include <SimpleIni.h>
#include <SKSE/SKSE.h>

namespace LootDrop {
    void LootConfig::Load() {
        constexpr auto path = L"Data/SKSE/Plugins/LootDrop.ini";
        
        CSimpleIniA ini;
        ini.SetUnicode();
        
        if (ini.LoadFile(path) < 0) {
            SKSE::log::info("Config file not found, using defaults");
            
            // Create default config
            ini.SetValue("General", "; Drop chances range from 0.0 to 1.0", nullptr);
            ini.SetDoubleValue("General", "WeaponDropChance", weaponDropChance);
            ini.SetDoubleValue("General", "ArmorDropChance", armorDropChance);
            ini.SetDoubleValue("General", "MiscDropChance", miscDropChance);
            ini.SetDoubleValue("General", "AlchemyDropChance", alchemyDropChance);
            ini.SetDoubleValue("General", "BookDropChance", bookDropChance);
            ini.SetDoubleValue("General", "DefaultDropChance", defaultDropChance);
            ini.SetBoolValue("General", "LevelScaling", levelScaling);
            
            ini.SaveFile(path);
            return;
        }
        
        weaponDropChance = static_cast<float>(ini.GetDoubleValue("General", "WeaponDropChance", weaponDropChance));
        armorDropChance = static_cast<float>(ini.GetDoubleValue("General", "ArmorDropChance", armorDropChance));
        miscDropChance = static_cast<float>(ini.GetDoubleValue("General", "MiscDropChance", miscDropChance));
        alchemyDropChance = static_cast<float>(ini.GetDoubleValue("General", "AlchemyDropChance", alchemyDropChance));
        bookDropChance = static_cast<float>(ini.GetDoubleValue("General", "BookDropChance", bookDropChance));
        defaultDropChance = static_cast<float>(ini.GetDoubleValue("General", "DefaultDropChance", defaultDropChance));
        levelScaling = ini.GetBoolValue("General", "LevelScaling", levelScaling);
        
        SKSE::log::info("Config loaded - Weapon: {:.0%}, Armor: {:.0%}", weaponDropChance, armorDropChance);
    }
}