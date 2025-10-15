#include <SKSE/SKSE.h>
#include <RE/Skyrim.h>
#include <random>

class LootDropManager : public RE::BSTEventSink<RE::TESDeathEvent> {
public:
    static LootDropManager* GetSingleton() {
        static LootDropManager singleton;
        return &singleton;
    }

    void Register() {
        RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(this);
        SKSE::log::info("LootDrop registered for death events");
    }

    RE::BSEventNotifyControl ProcessEvent(const RE::TESDeathEvent* a_event, RE::BSTEventSource<RE::TESDeathEvent>*) override {
        if (!a_event || !a_event->actorDying) return RE::BSEventNotifyControl::kContinue;
        
        auto actor = a_event->actorDying->As<RE::Actor>();
        if (!actor || actor->IsPlayerRef()) return RE::BSEventNotifyControl::kContinue;

        SKSE::GetTaskInterface()->AddTask([actor]() {
            ProcessActorInventory(actor);
        });

        return RE::BSEventNotifyControl::kContinue;
    }

private:
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<> dis{0.0, 1.0};

    void ProcessActorInventory(RE::Actor* actor) {
        auto container = actor->As<RE::TESContainer>();
        if (!container) return;

        std::vector<RE::TESBoundObject*> itemsToRemove;
        
        container->ForEachContainerObject([&](RE::TESBoundObject* obj) {
            if (!obj->IsGold() && !obj->HasKeyword(0x0A0E5D)) {  // 0x0A0E5D = VendorNoSale (quest items)
                float dropChance = GetDropChance(obj);
                if (dis(gen) > dropChance) {
                    itemsToRemove.push_back(obj);
                }
            }
            return RE::BSContainer::ForEachResult::kContinue;
        });

        // Remove items that didn't pass the drop check
        for (auto* item : itemsToRemove) {
            actor->RemoveItem(item, 99999, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
        }
    }

    float GetDropChance(RE::TESBoundObject* item) {
        if (item->IsWeapon()) return 0.3f;
        if (item->IsArmor()) return 0.25f;
        if (item->IsBook()) return 0.6f;
        if (item->formType == RE::FormType::AlchemyItem) return 0.4f;
        return 0.35f;
    }
};

void MessageHandler(SKSE::MessagingInterface::Message* a_msg) {
    if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
        LootDropManager::GetSingleton()->Register();
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse) {
    SKSE::Init(a_skse);
    SKSE::log::info("LootDrop loading...");
    
    SKSE::GetMessagingInterface()->RegisterListener(MessageHandler);
    
    return true;
}