#include "Hooks.h"
#include "AttackHandleService.h"
void Hooks::AttackHook::Install() {
    SKSE::AllocTrampoline(14);
    auto& trampoline = SKSE::GetTrampoline();
    originalFunction =
        trampoline.write_call<5>(REL::RelocationID(37653, 38606).address() + REL::Relocate(0x4a, 0x4a), thunk);
}

char Hooks::AttackHook::thunk(RE::Character* attacked, RE::TESObjectREFR* attackerReference, float unk) {
    if (attackerReference && attacked) {
        AttackHandleService::AttackDetected(attacked, attackerReference);
    }
    return originalFunction(attacked, attackerReference, unk);
}

void Hooks::Install() {
    AttackHook::Install();
    // SampleVirtualFunctionHook::Install();
    // SampleCallHook::Install();
    // SampleEventSink::Install();
}
