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
        logger::info("LootDrop registered");
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
        auto inventory = actor->GetInventory();
        
        for (const auto& [item, data] : inventory) {
            auto& [count, entry] = data;
            if (!item || count <= 0) continue;
            
            if (!item->IsGold() && !item->IsQuestObject()) {
                float dropChance = GetDropChance(item);
                if (dis(gen) > dropChance) {
                    actor->RemoveItem(item, count, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
                }
            }
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
    SKSE::GetMessagingInterface()->RegisterListener(MessageHandler);
    return true;
}