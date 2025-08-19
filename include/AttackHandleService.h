#pragma once

class AttackHandleService {
    static inline std::mutex mtx;
    static inline float lastActivatedTime = 0.0;

public:
    static void ResetTime();
    static void AttackDetected(RE::Character* attacked, RE::TESObjectREFR* attackerReference);
};