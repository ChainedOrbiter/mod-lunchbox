/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "Chat.h"
#include "ConfigValueCache.h"
#include "Player.h"
#include "UnitScript.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"

namespace Acore::Lunchbox
{
    static bool ModuleEnabled = true;
    static float DrinkMultiplier = 1.0f;
    static float FoodMultiplier = 1.0f;
    static bool DebugChatEnabled = false;

    class LunchboxWorldScript : public WorldScript
    {
    public:
        LunchboxWorldScript() : WorldScript("LunchboxWorldScript",
                        {WORLDHOOK_ON_AFTER_CONFIG_LOAD})
        {
        }

        void OnAfterConfigLoad([[maybe_unused]] const bool reload) override
        {
            // Load module settings after world server options are available - also allows for reload config
            ModuleEnabled = sConfigMgr->GetOption<bool>("Lunchbox.Enable", true);
            DrinkMultiplier = sConfigMgr->GetOption<float>("Lunchbox.DrinkMultiplier", true);
            FoodMultiplier = sConfigMgr->GetOption<float>("Lunchbox.FoodMultiplier", true);

            DebugChatEnabled = sConfigMgr->GetOption<bool>("Lunchbox.DebugChatEnabled", true);
        }
    };

    class LunchboxUnitScript : public UnitScript
    {
    public:
        LunchboxUnitScript() : UnitScript("LunchboxUnitScript",
                       true,
                       {UNITHOOK_ON_AURA_APPLY})
        {
        }

        void OnAuraApply(Unit* unit, Aura* aura) override
        {
            // Only applies to players
            if (!unit->IsPlayer())
                return;

            AuraEffect* effect = aura->GetEffect(EFFECT_0);
            if (!effect)
                return;

            // Detect if drinking
            if (effect->GetAuraType() == SPELL_AURA_MOD_POWER_REGEN && effect->GetMiscValue() == POWER_MANA)
            {
                const int32 originalAmount = effect->GetAmount();
                // Calculate & apply the modifier
                const float newAmount = static_cast<float>(originalAmount) * DrinkMultiplier;
                effect->SetAmount(static_cast<int32>(newAmount));

                // Apply the new regen value
                unit->ToPlayer()->UpdateManaRegen();

                // Debug messages
                if (DebugChatEnabled)
                {
                    std::ostringstream msg;
                    msg << "[Drink Booster] Original regen: "<< originalAmount << " per 5 seconds - New regen: " << newAmount << " per 5 seconds" << std::endl;
                    ChatHandler(unit->ToPlayer()->GetSession()).SendSysMessage(msg.str());
                }
            }

            // Detect eating
            if (effect->GetAuraType() == SPELL_AURA_MOD_REGEN)
            {
                const int32 originalAmount = effect->GetAmount();
                // Calculate & apply the modifier
                    // TODO: make a separate config
                const float newAmount = static_cast<float>(originalAmount) * DrinkMultiplier;
                effect->SetAmount(static_cast<int32>(newAmount));

                // Debug messages
                if (DebugChatEnabled)
                {
                    std::ostringstream msg;
                    msg << "[Drink Booster] Original food regen: "<< originalAmount << " health per 5 seconds - New regen: " << newAmount << " health per 5 seconds" << std::endl;
                    ChatHandler(unit->ToPlayer()->GetSession()).SendSysMessage(msg.str());
                }
            }
        }
    };

    class LunchboxPlayerScript : public PlayerScript
    {
    public:
        LunchboxPlayerScript() : PlayerScript("LunchboxPlayerScript",
                                                  {PLAYERHOOK_ON_LOGIN})
        {
        }

        void OnPlayerLogin(Player* player) override
        {
            // Announce mod
            if (ModuleEnabled)
                ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00Lunchbox|r module.");
        }
    };

}

// Add all scripts in one
void AddLunchboxScripts()
{
    new Acore::Lunchbox::LunchboxPlayerScript();
    new Acore::Lunchbox::LunchboxUnitScript();
    new Acore::Lunchbox::LunchboxWorldScript();
}
