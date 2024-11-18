// LINK

//Default
#include "CharacterInGameHUD.h"

//Engine
#include "Camera/CameraComponent.h"
#include "Components/ProgressBar.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWEventManager.h"

UCharacterInGameHUD::UCharacterInGameHUD()
{
}

void UCharacterInGameHUD::NativeConstruct()
{
	Super::NativeConstruct();

	UPWEventManager* EventManager = UPWEventManager::Get(this);
	if (IsValid(EventManager) == false)
	{
		ensure(false);
		return;
	}

	EventManager->HealthChangedDelegate.AddUObject(this, &UCharacterInGameHUD::OnHealthChanged);
}

void UCharacterInGameHUD::InitializeHUDWidget(class APWPlayerCharacter* InHUDOwnerCharacter)
{
	HUDOwnerCharacter = InHUDOwnerCharacter;
}

void UCharacterInGameHUD::OnHealthChanged(AActor* TargetActor, float MaxHealth, float CurrentHealth)
{
	if (IsValid(HUDOwnerCharacter) == false || HUDOwnerCharacter != TargetActor)
	{
		return;
	}

	if (IsValid(ProgressBar_Health) == true)
	{
		ProgressBar_Health->SetPercent(CurrentHealth / MaxHealth);
	}
}
