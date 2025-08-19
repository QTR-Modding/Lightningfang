#include "Logger.h"
#include "UI.h"
#include "Hooks.h"
#include "QTRLib.h"
#include "AttackHandleService.h"
void OnMessage(SKSE::MessagingInterface::Message* message) {
    if (message->type == SKSE::MessagingInterface::kDataLoaded) {
    }
    if (message->type == SKSE::MessagingInterface::kPostLoadGame ||
        message->type == SKSE::MessagingInterface::kNewGame) {
        AttackHandleService::ResetTime();
    }
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);
    SetupLog();
    SKSE::GetMessagingInterface()->RegisterListener(OnMessage);
    logger::info("Plugin loaded");
    UI::Register();
    Hooks::Install();
    QTRLib::Install();
    return true;
}