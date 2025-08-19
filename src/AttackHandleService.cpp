#include "AttackHandleService.h"

#undef GetObject;

void AttackHandleService::ResetTime() { lastActivatedTime = 0; }

// TODO: Refactor this code
void AttackHandleService::AttackDetected(RE::Character* attacked, RE::TESObjectREFR* attackerReference) {
    auto attackerActor = attackerReference->As<RE::Actor>();
    if (!attackerActor) return;
    auto attackerActorValue = attackerActor->AsActorValueOwner();
    if (!attackerActorValue) return;
    auto weapon = attackerActor->GetAttackingWeapon();
    if (!weapon) return;
    auto enchantment = weapon->GetEnchantment();
    if (!enchantment) return;
    auto obj = weapon->GetObject();
    if (!obj) return;
    
    auto cost = enchantment->CalculateMagickaCost(attackerActor);

    auto equipedLeft = attackerActor->GetEquippedObject(true);
    auto equipedRight = attackerActor->GetEquippedObject(false);
    auto isLeftWeapon = equipedLeft == obj;
    auto charge = isLeftWeapon
        ? attackerActorValue->GetActorValue(RE::ActorValue::kLeftItemCharge)
        : attackerActorValue->GetActorValue(RE::ActorValue::kRightItemCharge);
    auto now = RE::Calendar::GetSingleton()->GetHoursPassed();
    auto caster = attackerActor->GetMagicCaster(isLeftWeapon ? RE::MagicSystem::CastingSource::kLeftHand
                                                             : RE::MagicSystem::CastingSource::kRightHand);
    if (!caster) return;

    if (!attackerActor->IsPowerAttacking()) return;
    if (charge < cost) return; 
    if (!obj->HasKeywordByEditorID("QTRLightningfangKeyword")) return;
    if ((now - lastActivatedTime) <= 0.005) return;

    const std::lock_guard<std::mutex> lock(mtx);
    lastActivatedTime = now;
    caster->CastSpellImmediate(
        RE::TESForm::LookupByEditorID<RE::MagicItem>("QTRChainLightning"), false,
        attacked, 1.0f, true, 1, attackerActor);
}
