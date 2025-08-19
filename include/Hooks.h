#pragma once

namespace Hooks {
    void Install();

    class AttackHook {
    public:
        static void Install();
    private:
        static char thunk(RE::Character* c, RE::TESObjectREFR* ref, float unk);
        static inline REL::Relocation<decltype(thunk)> originalFunction;
    };

}