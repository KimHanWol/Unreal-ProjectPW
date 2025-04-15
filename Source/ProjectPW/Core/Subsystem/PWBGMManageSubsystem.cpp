// LINK

//Default
#include "PWBGMManageSubsystem.h"

//Engine
#include "Components/AudioComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

//Game
#include "Core/PWGameInstance.h"
#include "Data/DataAsset/PWSoundData.h"

UPWBGMManageSubsystem* UPWBGMManageSubsystem::Get(const UObject* WorldContextObj)
{
    UPWGameInstance* PWGameInst = UPWGameInstance::Get(WorldContextObj);
    if (IsValid(PWGameInst) == true)
    {
        return PWGameInst->GetPWBGMManageSubsystem();
    }

    return nullptr;
}

void UPWBGMManageSubsystem::PlayBGM(EBGMType BGMType)
{
    if (CurrentBGMType == BGMType)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (IsValid(World) == false)
    {
        ensure(false);
        return;
    }

    const UPWSoundData* SoundData = UPWSoundData::Get(this);
    if (IsValid(SoundData) == false)
    {
        ensure(false);
        return;
    }

    if (IsValid(BGMComponent) == true && BGMComponent->IsPlaying())
    {
        BGMComponent->Stop();
    }

    if (BGMType == EBGMType::MainMenu)
    {
        BGMComponent = UGameplayStatics::SpawnSound2D(World, SoundData->MainMenuBGM.LoadSynchronous(), 1.f, 1.f, 0.f, nullptr, true);
    }
    else if (BGMType == EBGMType::InGame)
    {
        BGMComponent = UGameplayStatics::SpawnSound2D(World, SoundData->InGameBGM.LoadSynchronous(), 1.f, 1.f, 0.f, nullptr, false);
    }

    CurrentBGMType = BGMType;
}
