#include "AttackHandleService.h"

#undef GetObject;

void AttackHandleService::ResetTime() { lastActivatedTime = 0; }

void AttackHandleService::AttackDetected(RE::Character* attacked, RE::TESObjectREFR* attackerReference) {
    if (auto attackerActor = attackerReference->As<RE::Actor>()) {
        if (attackerActor->IsPowerAttacking()) {
            auto weapon = attackerActor->GetAttackingWeapon();
            if (weapon) {
                if (weapon->GetEnchantmentCharge() > 0) {
                    if (auto obj = weapon->GetObject()) {
                        if (obj->HasKeywordByEditorID("QTRLightningfangKeyword")) {
                            const std::lock_guard<std::mutex> lock(mtx);
                            auto now = RE::Calendar::GetSingleton()->GetHoursPassed();
                            if ((now - lastActivatedTime) > 0.005) {
                                lastActivatedTime = now;
                                if (auto caster =
                                        attackerActor->GetMagicCaster(attackerActor->GetEquippedObject(true) == obj?RE::MagicSystem::CastingSource::kLeftHand:RE::MagicSystem::CastingSource::kRightHand)) {
                                    caster->CastSpellImmediate(
                                        RE::TESForm::LookupByEditorID<RE::MagicItem>("QTRChainLightning"), false,
                                        attacked, 1.0f, true, 1, attackerActor);

                                    auto fx = RE::TESForm::LookupByID<RE::TESBoundObject>(0x2064A);
                                    auto position = attacked->GetPosition() + RE::NiPoint3{0, 0, 80};
                                    auto angle = attacked->GetAngle();
                                    auto cell = attacked->GetParentCell();
                                    auto worldspace = attacked->GetWorldspace();
                                    RE::TESDataHandler::GetSingleton()->CreateReferenceAtLocation(
                                        fx, position, angle, cell, worldspace, nullptr, nullptr, RE::ObjectRefHandle(),
                                        false, true);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

