/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Tanaris
SD%Complete: 80
SDComment: Quest support: 648, 10277, 10279(Special flight path).
SDCategory: Tanaris
EndScriptData */

/* ContentData
npc_custodian_of_time
npc_steward_of_time
npc_OOX17
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "ScriptedFollowerAI.h"

/*######
## npc_custodian_of_time
######*/

enum CustodianOfTime
{
    WHISPER_CUSTODIAN_1     = 0,
    WHISPER_CUSTODIAN_2     = 1,
    WHISPER_CUSTODIAN_3     = 2,
    WHISPER_CUSTODIAN_4     = 3,
    WHISPER_CUSTODIAN_5     = 4,
    WHISPER_CUSTODIAN_6     = 5,
    WHISPER_CUSTODIAN_7     = 6,
    WHISPER_CUSTODIAN_8     = 7,
    WHISPER_CUSTODIAN_9     = 8,
    WHISPER_CUSTODIAN_10    = 9,
    WHISPER_CUSTODIAN_11    = 10,
    WHISPER_CUSTODIAN_12    = 11,
    WHISPER_CUSTODIAN_13    = 12,
    WHISPER_CUSTODIAN_14    = 13
};

class npc_custodian_of_time : public CreatureScript
{
public:
    npc_custodian_of_time() : CreatureScript("npc_custodian_of_time") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_custodian_of_timeAI(creature);
    }

    struct npc_custodian_of_timeAI : public npc_escortAI
    {
        npc_custodian_of_timeAI(Creature* creature) : npc_escortAI(creature) {}

        void WaypointReached(uint32 waypointId)
        {
            if (Player* player = GetPlayerForEscort())
            {
                switch (waypointId)
                {
                case 0:
                    Talk(WHISPER_CUSTODIAN_1, player->GetGUID());
                    break;
                case 1:
                    Talk(WHISPER_CUSTODIAN_2, player->GetGUID());
                    break;
                case 2:
                    Talk(WHISPER_CUSTODIAN_3, player->GetGUID());
                    break;
                case 3:
                    Talk(WHISPER_CUSTODIAN_4, player->GetGUID());
                    break;
                case 5:
                    Talk(WHISPER_CUSTODIAN_5, player->GetGUID());
                    break;
                case 6:
                    Talk(WHISPER_CUSTODIAN_6, player->GetGUID());
                    break;
                case 7:
                    Talk(WHISPER_CUSTODIAN_7, player->GetGUID());
                    break;
                case 8:
                    Talk(WHISPER_CUSTODIAN_8, player->GetGUID());
                    break;
                case 9:
                    Talk(WHISPER_CUSTODIAN_9, player->GetGUID());
                    break;
                case 10:
                    Talk(WHISPER_CUSTODIAN_4, player->GetGUID());
                    break;
                case 13:
                    Talk(WHISPER_CUSTODIAN_10, player->GetGUID());
                    break;
                case 14:
                    Talk(WHISPER_CUSTODIAN_4, player->GetGUID());
                    break;
                case 16:
                    Talk(WHISPER_CUSTODIAN_11, player->GetGUID());
                    break;
                case 17:
                    Talk(WHISPER_CUSTODIAN_12, player->GetGUID());
                    break;
                case 18:
                    Talk(WHISPER_CUSTODIAN_4, player->GetGUID());
                    break;
                case 22:
                    Talk(WHISPER_CUSTODIAN_13, player->GetGUID());
                    break;
                case 23:
                    Talk(WHISPER_CUSTODIAN_4, player->GetGUID());
                    break;
                case 24:
                    Talk(WHISPER_CUSTODIAN_14, player->GetGUID());
                    DoCast(player, 34883);
                    // below here is temporary workaround, to be removed when spell works properly
                    player->AreaExploredOrEventHappens(10277);
                    break;
                }
            }
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (HasEscortState(STATE_ESCORT_ESCORTING))
                return;

            if (auto const player = who->ToPlayer())
            {
                if (player->HasAura(34877) && player->GetQuestStatus(10277) == QUEST_STATUS_INCOMPLETE)
                    if (me->IsWithinDistInMap(player, 10.0f))
                        Start(false, false, player->GetGUID());
            }
        }

        void EnterCombat(Unit* /*who*/) {}
        void Reset() {}

        void UpdateAI(const uint32 diff)
        {
            npc_escortAI::UpdateAI(diff);
        }
    };

};

/*######
## npc_steward_of_time
######*/

#define GOSSIP_ITEM_FLIGHT  "Please take me to the master's lair."

class npc_steward_of_time : public CreatureScript
{
public:
    npc_steward_of_time() : CreatureScript("npc_steward_of_time") { }

    bool OnQuestAccept(Player* player, Creature* /*creature*/, Quest const* quest)
    {
        if (quest->GetQuestId() == 10279)                      //Quest: To The Master's Lair
            player->CastSpell(player, 34891, true);               //(Flight through Caverns)

        return false;
    }

    bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF + 1)
            player->CastSpell(player, 34891, true);               //(Flight through Caverns)

        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestStatus(10279) == QUEST_STATUS_INCOMPLETE || player->GetQuestRewardStatus(10279))
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FLIGHT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(9978, creature->GetGUID());
        }
        else
            player->SEND_GOSSIP_MENU(9977, creature->GetGUID());

        return true;
    }

};

/*######
## npc_OOX17
######*/

enum Npc00X17
{
    //texts are signed for 7806
    SAY_OOX_START           = -1000287,
    SAY_OOX_AGGRO1          = -1000288,
    SAY_OOX_AGGRO2          = -1000289,
    SAY_OOX_AMBUSH          = -1000290,
    SAY_OOX17_AMBUSH_REPLY  = -1000291,
    SAY_OOX_END             = -1000292,

    Q_OOX17                 = 648,
    SPAWN_FIRST             = 7803,
    SPAWN_SECOND_1          = 5617,
    SPAWN_SECOND_2          = 7805
};

class npc_OOX17 : public CreatureScript
{
public:
    npc_OOX17() : CreatureScript("npc_OOX17") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == Q_OOX17)
        {
            creature->setFaction(113);
            creature->SetFullHealth();
            creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
            creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            DoScriptText(SAY_OOX_START, creature);

            if (npc_escortAI* pEscortAI = CAST_AI(npc_OOX17::npc_OOX17AI, creature->AI()))
                pEscortAI->Start(true, false, player->GetGUID());
        }
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_OOX17AI(creature);
    }

    struct npc_OOX17AI : public npc_escortAI
    {
        npc_OOX17AI(Creature* creature) : npc_escortAI(creature) {}

        void WaypointReached(uint32 waypointId)
        {
            if (Player* player = GetPlayerForEscort())
            {
                switch (waypointId)
                {
                    case 23:
                        me->SummonCreature(SPAWN_FIRST, -8350.96f, -4445.79f, 10.10f, 6.20f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                        me->SummonCreature(SPAWN_FIRST, -8355.96f, -4447.79f, 10.10f, 6.27f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                        me->SummonCreature(SPAWN_FIRST, -8353.96f, -4442.79f, 10.10f, 6.08f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                        DoScriptText(SAY_OOX_AMBUSH, me);
                        break;
                    case 56:
                        me->SummonCreature(SPAWN_SECOND_1, -7510.07f, -4795.50f, 9.35f, 6.06f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                        me->SummonCreature(SPAWN_SECOND_2, -7515.07f, -4797.50f, 9.35f, 6.22f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                        me->SummonCreature(SPAWN_SECOND_2, -7518.07f, -4792.50f, 9.35f, 6.22f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                        DoScriptText(SAY_OOX_AMBUSH, me);
                        if (Unit* scoff = me->FindNearestCreature(SPAWN_SECOND_2, 30))
                            DoScriptText(SAY_OOX17_AMBUSH_REPLY, scoff);
                        break;
                    case 86:
                        DoScriptText(SAY_OOX_END, me);
                        player->GroupEventHappens(Q_OOX17, me);
                        break;
                }
            }
        }

        void Reset(){}

        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(RAND(SAY_OOX_AGGRO1, SAY_OOX_AGGRO2), me);
        }

        void JustSummoned(Creature* summoned)
        {
            summoned->AI()->AttackStart(me);
        }
    };
};

enum Butcherbot
{
    SAY_BUTCHING            = 0,
    NPC_GLASSHIDE_BASILISK  = 5419,
    NPC_FIRE_ROC            = 5429,
    NPC_BLISTERPAW_HYENA    = 5426,
    BUTCHERBOT_QUEST_1      = 25112,
    BUTCHERBOT_QUEST_2      = 25111,
    BUTCHERBOT_QUEST_3      = 25115,
    BUTCHERBOT_CREDIT_1     = 39702,
    BUTCHERBOT_CREDIT_2     = 40507,
    BUTCHERBOT_CREDIT_3     = 40509,
    SPELL_BUTCHERING        = 74168
};

class npc_butcherbot : public CreatureScript
{
public:
    npc_butcherbot() : CreatureScript("npc_butcherbot") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_butcherbotAI (creature);
    }

    struct npc_butcherbotAI : public ScriptedAI
    {
        npc_butcherbotAI(Creature* creature) : ScriptedAI(creature) { }

        bool isButchering;
        uint32 butcheringDelay;
        uint64 playerGUID;
        uint32 questId;

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == POINT_MOTION_TYPE && id == 1)
            {
                me->CastSpell(me, SPELL_BUTCHERING, true);
                Talk(SAY_BUTCHING);
                isButchering = true;

                if (Creature * const creature = GetQuestCreature())
                    creature->ForcedDespawn(1000);
            }
        }

        Creature* GetQuestCreature() const
        {
            uint32 entry = NPC_GLASSHIDE_BASILISK;
            if (questId == BUTCHERBOT_QUEST_2)
                entry = NPC_FIRE_ROC;
            else if (questId == BUTCHERBOT_QUEST_3)
                entry = NPC_BLISTERPAW_HYENA;

            return me->FindNearestCreature(entry, 20.f, false);
        }

        void Reset()
        {
            isButchering = false;
            butcheringDelay = 2000;

            if (Player * player = me->SelectNearestPlayer(20.f))
            {
                playerGUID = player->GetGUID();
                questId = BUTCHERBOT_QUEST_1;
                if (player->GetQuestStatus(BUTCHERBOT_QUEST_2) == QUEST_STATUS_INCOMPLETE)
                    questId = BUTCHERBOT_QUEST_2;
                else if (player->GetQuestStatus(BUTCHERBOT_QUEST_3) == QUEST_STATUS_INCOMPLETE)
                    questId = BUTCHERBOT_QUEST_3;
            }

            if (Creature * const creature = GetQuestCreature())
            {
                float x, y, z;
                creature->GetPosition(x,y,z);
                creature->GetNearPoint(creature, x, y, z, 0.5f, 0.5f, creature->GetAngle(me));
                me->GetMotionMaster()->MovePoint(1, x,y,z);
            }
        }

        void UpdateAI(uint32 const diff)
        {
            if (isButchering)
            {
                if (butcheringDelay <= diff)
                {
                    uint32 credit = BUTCHERBOT_CREDIT_1;
                    if (questId == BUTCHERBOT_QUEST_2)
                        credit = BUTCHERBOT_CREDIT_2;
                    else if (questId == BUTCHERBOT_QUEST_3)
                        credit = BUTCHERBOT_CREDIT_3;

                    if (Player * const player = me->GetPlayer(*me, playerGUID))
                        player->KilledMonsterCredit(credit);

                    if (Creature * const creature = GetQuestCreature())
                        creature->ForcedDespawn();

                    me->ForcedDespawn(1000);
                    isButchering = false;
                }
                else
                    butcheringDelay -= diff;
            }
        }
    };
};

void AddSC_tanaris()
{
    new npc_custodian_of_time();
    new npc_steward_of_time();
    new npc_OOX17();
    new npc_butcherbot();
}
